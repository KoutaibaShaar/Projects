// OrcerBook_V2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "OrderBook_V2.h"

#include <iostream>
#include <string>
#include <sstream>
#include <ctime> 
#include <chrono>
#include <utility>
#include <iomanip>
#include <climits>
#include <cstdlib>
#include <cassert>
#include <numeric> 
#include <vector>
#include <array>
#include <algorithm>


std::string Trim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) { return !std::isspace(ch); }));
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
	return s;
}

class Order {
public:
	enum Action { ADD, DELETE, EXECUTE, EMPTY };
	Order(Action action, signed int price, uint32_t queuePosition, uint32_t amount, const std::string& orderId)
		: m_time(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()), m_action(action), m_price(price)
		, m_queuePosition(queuePosition), m_amount(amount)
	{
		strncpy_s(m_orderId.data(), sizeof(m_orderId) + 1, orderId.c_str(), m_orderId.size());
	}
	~Order() = default;
	Action getAction() const noexcept { return m_action; }
	signed int getPrice() const noexcept { return m_price; }
	uint32_t getQueuePosition() const noexcept { return m_queuePosition; }
	int64_t  getTime() const noexcept { return m_time; }
	uint32_t getAmount() const noexcept { return m_amount; }
	std::array<char, OrderBook::size> getOrderId() const noexcept { return m_orderId; }
	std::string geFormatedOrder(const std::string& sep) const noexcept;
	void  setAmount(uint32_t amount)  noexcept { m_amount = amount; }
	void  setAction(Action action)  noexcept { m_action = action; }
private:
	int64_t m_time;
	Action m_action;
	signed int m_price;
	uint32_t m_queuePosition;
	uint32_t m_amount;
	std::array<char, OrderBook::size> m_orderId;
};

static const char enumToChar[] = { 'A', 'D', 'E' };
static Order::Action const table[] = {Order::ADD, Order::EMPTY, Order::EMPTY, Order::DELETE, Order::EXECUTE};

bool comparator(const std::shared_ptr<Order>& left, const std::shared_ptr<Order>& right)
{
	// sort price in descending order - for upperbound: get an iter to pos where left/new val > right/existing is true
	if (left->getPrice() > right->getPrice())
		return true;

	if (left->getPrice() == right->getPrice())
		return left->getTime() > right->getTime();

	return false;
}


bool isIdInCorrectFormat(const std::string& Id)
{
	if (Id.size() != 15) return false;

	auto it = std::find_if_not(std::begin(Id), std::end(Id), [](unsigned char c) { return std::isalnum(c); });
	if (it != Id.end()) return false;

	return true;
}

std::string Order::geFormatedOrder(const std::string& sep) const noexcept
{
	std::ostringstream stm;
	stm << std::setw(11)
		<< getTime() << sep
		<< enumToChar[getAction()] << sep
		<< getQueuePosition() << sep
		<< std::setw(6)
		<< getPrice() << sep
		<< std::setw(9)
		<< getAmount() << sep
		<< std::setw(16)
		<< getOrderId().data();

	return stm.str();
}

enum positions { action = 0, price = 1, queuePosition = 2, amount = 3, uniqueId = 4 };

void OrderBook::insertSorted(const std::shared_ptr<Order>& order)
{
    /*
	Lower: Returns an iterator pointing to the first element in the range [first, last) such that comp(element, value) or element < value
	is false. basically where the existing elements become no longer less than new element
	
	Upper: Returns an iterator pointing to the first element in the range [first, last) such that value < element 
	(or comp(value, element)) is true. so when existing value becomes less than new value
	*/
	Orders::iterator position = std::upper_bound(m_Orders.begin(), m_Orders.end(), order, comparator);
	Orders::iterator inserted =  m_Orders.insert(position, order);

	OrdersById::iterator positionById = std::lower_bound(m_OrdersById.begin(), m_OrdersById.end(), order->getOrderId()
		, [](const OrderBook::Ids& a, const std::array<char, size>& b) {return std::lexicographical_compare(a.first.begin(), a.first.end(), b.begin(), b.end()); });
	//insert unique id and an iterator to the order object to orders vector
	m_OrdersById.insert(positionById, std::make_pair(order->getOrderId(), inserted));
	std::cout << (*inserted)->getPrice() << std::endl;
}

int OrderBook::processIncomingUpdate(const std::string& update) noexcept
{
	std::stringstream ss(update);
	std::string field;
	std::vector<std::string> update_fields;
	while (std::getline(ss, field, ','))
	{
		if (ss.fail()) {
			std::cerr << "Incorrect format for update " << update << "\n";
			ss.clear();
			return -1;
		}
		Trim(field);
		update_fields.push_back(field);
	}

	int index = toupper(update_fields[positions::action][0]);
	Order::Action action = table[index - 65];
	if (action == Order::EMPTY)
	{
		return -1;
	}

	switch (action)
	{
	case Order::ADD:
	{
		if (!isIdInCorrectFormat(update_fields[(positions::uniqueId)]))
		{
			std::cerr << "UniqueId " << update_fields[(positions::uniqueId)] << " is in the incorrect fomrat so wont be added to book\n";
			return -1;
		}
		std::shared_ptr<Order> order = std::make_shared<Order>(	Order::ADD, atoi(update_fields[positions::price].c_str()), 
																atoi(update_fields[positions::queuePosition].c_str()) , 
																atoi(update_fields[positions::amount].c_str()),
																update_fields[positions::uniqueId]);
		insertSorted(order);
		break;
	}
	case Order::DELETE:
	{
		const std::string& orderUniqueId = update_fields[positions::uniqueId];
		if (!isIdInCorrectFormat(orderUniqueId))
			return -1;

		auto iter = std::lower_bound(m_OrdersById.begin(), m_OrdersById.end(), orderUniqueId,
			[](const OrderBook::Ids& a, const std::string& b) {return std::lexicographical_compare(a.first.begin(), a.first.end(), b.begin(), b.end()); });
		
		if (iter != m_OrdersById.end() && strcmp(iter->first.data(), orderUniqueId.c_str()) == 0)
		{
			std::cout << iter->first.data() << std::endl;
			m_Orders.erase(iter->second);
			m_OrdersById.erase(iter);
			break;
		}
		else
		{
			std::cerr << "Order with UniqueId " << update_fields[(positions::uniqueId)] << " has not been found\n";
			return -1;
		}
	}
	case Order::EXECUTE:
	{
		/*
		Execution - indicates that an existing order has been executed and has
		been removed from the market. It has two fields, the order's unique id and its execution size.
		messages, price, que position, size, orderID

		Form the above I assume that it will remain in the book but marked as EXECUTED - to distinguish it from a Delete event
		which causes the relevant order entry to be removed from the book
		*/
		const std::string& orderUniqueId = update_fields[positions::uniqueId];
		if (!isIdInCorrectFormat(orderUniqueId))
			return -1;

		uint32_t amount = atoi(update_fields[positions::amount].c_str());
		//auto iter = m_OrdersById.find(orderUniqueId);
		//if (iter == m_OrdersById.end())
		//{
			//std::cerr << "Order with UniqueId " << update_fields[(positions::uniqueId)] << " has not been found\n";
			//return -1;
		//}
		//iter->second->setAction(Order::EXECUTE);
		//iter->second->setAmount(amount);
		break;
	}
	default:
		return -1;
	}
	return 0;
}


// What are the four best prices in the book and what is the total amount available at each of those prices
/*
std::vector<OrderBook::PriceAndAmount> OrderBook::TopOfBook() const noexcept
{
	std::vector<OrderBook::PriceAndAmount> result;
	int count = 0;
	for (auto it = crbeginOrders(); it != crendOrders(); ++it)
	{
		result.emplace_back((*it)->getPrice(), (*it)->getAmount());
		count++;
		if (count == 4) break;
	}

	return result;
}
*/

// Queue Position - Given an order's unique id, what amount needs to be traded
// before it is at the front of the queue
// assumption is the trades with a higher price so those orders ahead of the order presented
// will be filled first (filling/matching nor part of this exercise)
// so add up the amount of the all the orders that preceed this one 

/*
uint32_t OrderBook::QueuePosition(const std::string& orderId) const noexcept
{
	auto iter = m_OrdersById.find(orderId);
	if (iter == m_OrdersById.end())
	{
		std::cerr << "Order with UniqueId " << orderId << " has not been found\n";
		return -1;
	}

	std::shared_ptr<Order> order = iter->second;
	std::pair<OrderBook::Orders::iterator, OrderBook::Orders::iterator> range = m_Orders.equal_range(order);
	auto i = range.first;
	for (; i != range.second; ++i)
	{
		if ((*i)->getOrderId() == "orderId")
		{
			break;
		}
	}

	int sum = 0;
	auto add = [&sum](std::shared_ptr<Order> x) { sum += x->getAmount(); };
	OrderBook::Orders::reverse_iterator r_iter = std::make_reverse_iterator(i);
	std::for_each(std::next(r_iter), m_Orders.rend(), add);

	return sum;
}
*/

std::ostream& operator<<(std::ostream& out, const OrderBook& orderBook)
{
	std::for_each(orderBook.cbeginOrders(), orderBook.cendOrders(), [&out](auto const& i) {out << i->geFormatedOrder(" , ") << "\n"; });
	
	out << "print content of order book by Id\n";
	std::for_each(orderBook.cbeginOrdersById(), orderBook.cendOrdersById(),
		[&out](auto const& i) {out << i.first.data() << "\n"; });
	return out;
}

void OrderBook::ClearBook() noexcept
{
	m_Orders.clear();
	m_OrdersById.clear();
}

std::shared_ptr<Order> OrderBook::getOrderAtPosition(size_t i) const noexcept
{
	assert(m_Orders.size() > i);
	return *std::next(m_Orders.begin(), i);
}

std::shared_ptr<Order> OrderBook::getOrderById(const std::string& orderId) const noexcept
{
	// its a sorted vector - get poistion of first element thats equal or less than orderId
	auto iter = std::lower_bound(m_OrdersById.begin(), m_OrdersById.end(), orderId,
		[](const OrderBook::Ids& a, const std::string& b){return std::lexicographical_compare(a.first.begin(), a.first.end(), b.begin(), b.end()); });
	assert(iter != m_OrdersById.end() && iter->first.data() == orderId);
	return *(iter->second);
}

void testCorrectNumberofOrders(OrderBook& orderBook)
{
	std::cout << "Run testCorrectNumberofOrders\n";
	// assumimng input arrives as a stream of strings with comma separated entries (irespective of source: CSV, socket, file)
	std::vector<std::string> vec = {
													{"A, 98, 7, 25500, 742760292142740"},//"Add, price, queue pos, volumne and uniqueID"
													{"A,105, 1, 20000,742760292142726"},
													{"A, 100, 4 , 10000, 742760292142728"},
													{"A, 104, 4 , 10000, 742760292142741"},
													{"A, 99,  6,  50000, 742760292142729"},
													{"A, 103, 3,  100, 742760292142730"},
													{"A, 100, 5 , 50000000,   742760292142727"},
													{"A, 105, 2,  16000, 742760292142731"},
													{"A, 305, 2,  16000, 7427602921427231111"}
	};

	for (std::string updateStr : vec)
	{
		if (orderBook.processIncomingUpdate(updateStr) != 0)
		{
			std::cerr << "Error processing update: " << updateStr << std::endl;
		}
	}

	assert(orderBook.getNumberofOrders() == 8);// its 8 as one order has an incorrect order id and thus wont be added to book
	// get top postion in book and ensure its the order with the highest price
	std::shared_ptr<Order> order = orderBook.getOrderAtPosition(0);
	assert(strcmp(order->getOrderId().data(), "742760292142726") == 0);

	std::cout << "Run testCorrectNumberofOrders - Ok\n";
}

void testDisplayBook(OrderBook& orderBook)
{
	std::cout << "Run testDisplayBook\n";

	std::cout << orderBook;

	std::cout << "Run testDisplayBook - Ok\n";
}

void testDeleteUpdate(OrderBook& orderBook)
{
	std::cout << "Run testDeleteUpdate\n";

	orderBook.processIncomingUpdate("D, 0,  0,   0,     742760292142726");
	assert(orderBook.getNumberofOrders() == 7);
	// ensure the right order is placed at the top now that previous highest has been removed
	std::shared_ptr<Order> order = orderBook.getOrderAtPosition(0);
	assert(strcmp( order->getOrderId().data(), "742760292142731" ) == 0);

	std::cout << "Run testDeleteUpdate - Ok\n";
}

void testExecuteUpdate(OrderBook& orderBook)
{
	std::cout << "Run testExecuteUpdate\n";

	orderBook.processIncomingUpdate("E, 0, 0, 20000, 742760292142725");
	std::shared_ptr<Order> order = orderBook.getOrderById("742760292142725");
	assert(order->getAction() == Order::Action::EXECUTE);

	std::cout << "Run testExecuteUpdate - Ok\n";
}

void testTopOfBook(OrderBook& orderBook)
{
	std::cout << "Run testTopOfBook\n";

	//std::vector<OrderBook::PriceAndAmount> results = orderBook.TopOfBook();
	//assert(results.size() == 4);
	//assert(results[0].first == 98 && results[1].first == 99 && results[2].first == 100 && results[3].first == 100);

	std::cout << "Run testTopOfBook - Ok\n";
}


void testQueuePosition(OrderBook& orderBook)
{
	std::cout << "Run testQueuePosition\n";

	//orderBook.QueuePosition("742760292142727");
	//assert(orderBook.QueuePosition("742760292142727") == 26100);

	std::cout << "Run testQueuePosition - Ok\n";
}

void testDeleteUpdate_2(OrderBook& orderBook)
{
	std::cout << "Run testDeleteUpdate_2\n";

	orderBook.processIncomingUpdate("D, 0,  0,   0,     742760292142729");
	assert(orderBook.getNumberofOrders() == 5);
	// ensure the right order is placed at the top now that previous highest has been removed
	std::shared_ptr<Order> order = orderBook.getOrderAtPosition(0);
	assert(order->getOrderId().data() == "742760292142731");

	std::cout << "Run testDeleteUpdate_2 - Ok\n";
}

void testDeleteUpdate_3(OrderBook& orderBook)
{
	std::cout << "Run testDeleteUpdate_3\n";

	orderBook.processIncomingUpdate("D, 0,  0,   0,     742760292142731");
	assert(orderBook.getNumberofOrders() == 4);
	// ensure the right order is placed at the top now that previous highest has been removed
	std::shared_ptr<Order> order = orderBook.getOrderAtPosition(0);
	assert(order->getOrderId().data() == "742760292142730");

	std::cout << "Run testDeleteUpdate_3 - Ok\n";
}

void runTestCases(OrderBook& orderBook)
{
	testCorrectNumberofOrders(orderBook);
	testDisplayBook(orderBook);
	//testExecuteUpdate(orderBook);
	testDeleteUpdate(orderBook);
	testDisplayBook(orderBook);
	//testTopOfBook(orderBook);
	//testQueuePosition(orderBook);
	//testDeleteUpdate_2(orderBook);
	//testDisplayBook(orderBook);
	//testDeleteUpdate_3(orderBook);
	//testDisplayBook(orderBook);
}


int main()
{
	OrderBook orderBook;
	runTestCases(orderBook);

	return 0;

}

