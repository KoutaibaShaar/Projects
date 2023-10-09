// LockFreeQueue.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>     
#include <mutex>
#include <condition_variable>
#include <type_traits>
#include <new>
#include <memory>


//#####################################################
/*
At first glance, this doesn’t seem too bad, and if there’s only one thread calling push()
at a time, and only one thread calling pop(), then this is actually perfectly fine.The
important thing in that case is the happens - before relationship between the push()
and the pop() to ensure that it’s safe to retrieve the data.The store to tail #7 is synchronized with the load 
from tail #1.the store to the preceding node’s data pointer #5 
is sequenced before the store to tail; and the load from tail is sequenced before
the load from the data pointer #2, so the store to data happens before the load, and
everything is OK.This is therefore a perfectly serviceable single - producer, single - consumer
(SPSC) queue.
*/
template<typename T>
class lock_free_queue
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        node* next;
        node() :
            next(nullptr)
        {}
    };
    std::atomic<node*> head;
    std::atomic<node*> tail;
    
    node * pop_head()
    {
        node* const old_head = head.load();
        if (old_head == tail.load()) //1
        {
            return nullptr;
        }
        head.store(old_head->next);
        return old_head;
    }
public:
    lock_free_queue() :
        head(new node), tail(head.load())
    {}
    lock_free_queue(const lock_free_queue& other) = delete;
    lock_free_queue& operator=(const lock_free_queue& other) = delete;
    ~lock_free_queue()
    {
        while (node* const old_head = head.load())
        {
            head.store(old_head->next);
            delete old_head;
        }
    }
    std::shared_ptr<T> pop()
    {
        node* old_head = pop_head();
        if (!old_head)
        {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> const res(old_head->data);//2
        delete old_head;
        return res;
    }
    void push(T new_value)
    {
        std::shared_ptr<T> new_data(std::make_shared<T>(new_value));
        node* p = new node;//3
        node* const old_tail = tail.load();//4
        old_tail->data.swap(new_data);//5
        old_tail->next = p;//6
        tail.store(p);//7
    }
};

//#####################################################

// havent tested it thoroughly
// what you mentioned about writes and reads from the same location in the q being inerleaved - consider this scenario
// push thread writes to location 0, then it increments the write index to position 1
// pop thread, reads the read index which contains teh value 0, the it fetches the write index (using a aquire barrier - this ensures teh write index fetched is teh latest
// and that all side-effect of instructions preceeding the releaes barrier (e.g.  _buffer[current_write] = item) in the push thread are visisble at the point of    
// of "const auto current_read = _read.load(std::memory_order_acquire);"
// pop fetches the value 1 from _write and proceeds to read form teh buffer at position 0, then increments _read to 1
// now the push thread will attempt to write to position 1 and succeeds, say the push thread is now context switched. the pop thread is in the cpu and its read index is also 1
// the current_read == _write.load(std::memory_order_acquire) will evaluate to true as the push thread is yet to write the new write index.... 
template<class T>
class SPSCQueueOverWrite {
public:
    SPSCQueueOverWrite(size_t size) : _write(0), _read(0), _size(size) { _buffer = new T[_size]; }
    ~SPSCQueueOverWrite() { delete[] _buffer; }
    bool push(const T& item)
    {
        const auto current_write = _write.load(std::memory_order_relaxed);  // a relaxed barrier ensures orderng for this identifier only within this thread and access atomicity 
        const auto next_write = (current_write + 1) % _size;
        // the following checks if the next write posuition is equal to the read positon - i.e writer has caught up with reader, then the queue is full (writer has wrapped)
        // so increment the reader position so that we overwrite the oldest data by moving the _read index forward allowing uncontested write to the oldest _read position
        // (what the _write index now points to) 
        // the aquire barrier esnures that the most up-to-date value of _read is fetched  (one stored by the pop )                         
        if (next_write == _read.load(std::memory_order_acquire))   
        {   
            // move read index forward
            _read.store((_read.load(std::memory_order_acquire) + 1) % _size, std::memory_order_release);
        }
        _buffer[current_write] = item;
        // pop will see the latest value of _write using the coresponding aquire_barrier (all writes will be propagated to the relevant thread on whichever core they maybe running on)
        _write.store(next_write, std::memory_order_release);
        return true; 
    }
    bool pop(T& item)
    {
        const auto current_read = _read.load(std::memory_order_acquire);
        if (current_read == _write.load(std::memory_order_acquire))// get latest _write from push thread and ensures all instrcutions preceeding the releae barrier in push are visisble here
        {
            return false; // empty queue
        }
        item = _buffer[current_read];                                       
        _read.store((current_read + 1) % _size, std::memory_order_release);  // this ensures the push sees the update-to-date value of _read
        return true;
    }
private:
    T* _buffer;
    std::atomic<uint32_t> _read;
    std::atomic<uint32_t> _write;
    size_t _size;
};



//######################################################

using BlockVersion = uint32_t;
using MessageSize  = uint32_t;

// the atomic is on the version of the given block
struct SeqLockedData {
    std::atomic<unsigned> sequence;
    int data;
};

class SPMCQueue {
public:
    void seqlock_write(SeqLockedData& data, int new_value);
private:
    struct Block {
        std::atomic<BlockVersion> mVersion;
        std::atomic<MessageSize> mSize;
        uint8_t* mData;
    } block;

    struct Header {
        alignas(64) std::atomic<uint64_t> mBlockCounter{ 0 };
        alignas(64) Block* mBlock;
    } header;
};

/*
Write Operation : Writing to the seqlock - protected data involves updating both the data and the sequence number.
You must ensure that updates to the data are atomic and visible to other threads.
After updating the data, increment the sequence number.

void SPMCQueue(MessageSize size, WriteCallBack writeCallBack)
{
    mVersion += 1
}

*/
void SPMCQueue::seqlock_write(SeqLockedData & data, int new_value) {
    unsigned seq;
    do {
        seq = data.sequence.load(std::memory_order_relaxed);
        seq &= ~1; // Clear the odd bit to indicate a write in progress
    } while (!data.sequence.compare_exchange_weak(seq, seq + 2, std::memory_order_relaxed));
    data.data = new_value;
    std::atomic_thread_fence(std::memory_order_release); // Ensure data consistency
    data.sequence.store(seq + 1, std::memory_order_relaxed); // Mark write completion
}

/*
To read the data protected by the seqlock, you can use a simple load operation on the data. 
You should also read the sequence number both before and after reading the data to check for concurrent updates. 
If the sequence number changes during this process, you should retry the read.
*/
int seqlock_read(SeqLockedData& data) {
    unsigned seq;
    int value;
    do {
        seq = data.sequence.load(std::memory_order_relaxed);
        value = data.data;
        std::atomic_thread_fence(std::memory_order_acquire);
    } while (seq & 1 || seq != data.sequence.load(std::memory_order_relaxed));
    return value;
}


//=============================================================

template<class T>
class SPSCQueue {
public:
    SPSCQueue(size_t size): _write(-1), _read(-1), _size(size), _writeCounter(0), _readCounter(0) { _buffer = new T[_size]; }
    ~SPSCQueue() { delete[] _buffer; }
    void Add(T v) 
    { 
        if (_write == -1)
            _write = 0;
        else
            _write = (_write + 1) % _size;
        _buffer[_write] = v;
        if (_read == -1)
            _read = 0;
        _writeCounter++;
        if (_writeCounter % _size == 0)
            std::cout << "Writer has wrapped" << std::endl;

    }
    bool Read(T& v)
    {
        if (IsEmpty())
            return false;
        if (_writeCounter == _readCounter)
        {
            std::cout << "Reader has read ALL messages" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            return false;
        }
        v = _buffer[_read];
        _readCounter++;
        _read = (_read + 1) % _size;
        return true;
    }
private:
    bool IsEmpty()
    {
        return _write == -1 || _read == -1;
    }
    T* _buffer;
    int _read;
    int _write;
    size_t _size;
    unsigned int _readCounter;
    unsigned int _writeCounter;
};



void function2()
{

    std::cout << "sinister!!!!!" << std::endl;
}

void function1()
{
    char foo[8];
    //                            overwrite ebp     overwrite ret address
    strncpy(foo, "12345678" "\x01\x02\x03\x04" "\x00\x4a\x84\x00", 16);

}

SPSCQueue<int> rq(5);

void Write()
{
    int val = 0, count = 0;
    while (1)
    {
        rq.Add(val++);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ++count;
        if (count == 5)
        {
            //pause wrting to check that reader pauses too (as a result of having caught up)
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            count = 0;
        }
    }
}

void Read()
{
    int val = 0;
    while (1)
    {
        if (rq.Read(val))
            std::cout << "Reader: " << val << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));//slower than writer
    }

}

#include <type_traits>

template<class T, std::size_t N>
class static_vector
{
    // properly aligned uninitialized storage for N T's
    std::aligned_storage_t<sizeof(T), alignof(T)> data[N];
    std::size_t m_size = 0;

public:
    // Create an object in aligned storage
    template<typename ...Args> void emplace_back(Args&&... args)
    {
        if (m_size >= N) // possible error handling
            throw std::bad_alloc{};

        // construct value in memory of aligned storage
        // using inplace operator new
        ::new(&data[m_size]) T(std::forward<Args>(args)...);
        ++m_size;
    }

    // Access an object in aligned storage
    const T& operator[](std::size_t pos) const
    {
        // Note: std::launder is needed after the change of object model in P0137R1
        return *launder(reinterpret_cast<const T*>(&data[pos]));
    }

    // Destroy objects from aligned storage
    ~static_vector()
    {
        for (std::size_t pos = 0; pos < m_size; ++pos)
            // Note: std::launder is needed after the change of object model in P0137R1
            destroy_at(launder(reinterpret_cast<T*>(&data[pos])));
    }
};

/*
    cwd xor ax, dx
    sub ax, dx
    Convert word in AX to double-word in DX:AX. Sign is preserved, value is preserved. So if AX >= 0, DX = 0 and if AX < 0, DX = -1.

XOR does nothing if AX == 0.

If AX < 0, the XOR reverses all bits of AX. Then the SUB adds 1 (or subtracts -1, whatever :P) to AX. This is the way to compute 2's complement of a binary number.

All in all, that sequence of instructions places the absolute value of AX into AX and sets DX according to sign.

The msb has a special meaning (1, 0) in this notation allowing negatives and positives to be processed
to negate a number in 2's complement (gor form negative to positive), we reverse the bits and 

*/
int main()
{
    std::atomic<int*> ptr{ nullptr };
    ptr.store(new int{ 0 }, std::memory_order_release);
    std::cout << ptr.load() << "\n";
    return 0;

    function1();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 10));
    return 0;

    std::thread Writer(Write);
    std::thread Reader(Read);

    Writer.join();
    Reader.join();

    return 0;
}
