#pragma once
template<typename T>
struct message_header
{
    T id{};
    uint32_t size = 0;
};

template<typename T>
struct message
{
    message_header<T> header{};
    std::vector<uint8_t> body;

    size_t size() const
    {
        return sizeof(message_header<T>) + body.size();
    }

    friend std::ostream& operator << (std::ostream& os, const message<T>& msg)
    {
        os << "ID: " << int(msg.header.id) << " Size:" << msg.header.size;
    }

    // pipe x and y into message
    // msg << x << y
    // push any POD type like data into the message buffer
    // push a float DataType will be float etc
    template<typename DataType>
    friend message<T>& operator << (message<T>& msg, const DataType& data)
    {
        // check that data type being pushed is trivially copyable
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to push");

        // get current size
        size_t i = msg.body.size();

        // resize vector to accomodate data being pushed in
        msg.body.resize(i + sizeof(DataType));

        // copy data into the newly allocated vector space - data() returns adddress of memory array used by the vector
        // add i to move to address we want to copy to
        std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

        msg.header.size = msg.size();

        return msg;
    }

    template<typename DataType>
    friend message<T>& operator >> (message<T>& msg, const DataType& data)
    {
        // check that data type being read is trivially copyable
        static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to push");

        // get size/position where the extraction of data starts
        size_t i = msg.body.size() - sizeof(DataType);

        // copy data from vector at position i to data 
        // add i to move to address we want to copy to
        std::memcpy(&data, msg.body.data() + i, &data, sizeof(DataType));

        // making vector smaller has no pef overhead as no reallocation is involved
        msg.body.resize(i);

        return msg;
    }
};
