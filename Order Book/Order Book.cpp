// Order Book.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <sstream>
#include <ctime> 
#include <chrono>
#include <queue>
#include <set>
#include <iomanip>
#include <climits>
#include <cstdlib>


class Order {
public:
	enum BuySellFlag { BUY, SELL };
	Order(std::string&& orderId, unsigned int price, unsigned long quantity, BuySellFlag buysell, int64_t time)
		: m_buysell(buysell), m_price(price), m_quantity(quantity), m_time(time)
	{
		m_orderId = orderId;
		orderId = "";
	}
	BuySellFlag getBuySellFlag() const { return m_buysell; }
	void setPrice(int price)  { m_price = price; }
	unsigned int getPrice() const { return m_price; } 
	int64_t  getTime() const { return m_time; }
	unsigned long getQuantity() const { return m_quantity; }
	std::string getOrderId() const { return m_orderId; }
	std::string getFormatedOrder() const;
	void setQuantity(unsigned long quantity) { m_quantity = quantity; }
private:
	std::string m_orderId;
	BuySellFlag m_buysell;
	unsigned int m_price;
	unsigned long m_quantity;
	int64_t m_time;
};

// less<> - less will invoke it operator() where lhs and rhs are of type T which in this case
// is std::shared_ptr<Order> 
// The max heap to sort elements in an descending order so that the top element has the max price
 bool operator<(const std::shared_ptr<Order>& left, const std::shared_ptr<Order>& right) 
{
	//for min_heaps using less<>
	// highest price comes first (so when right larger than left, right is first)
	//if (left->getPrice() < right->getPrice())
		//return true;

	//if (left->getPrice() == right->getPrice())
		//return left->getTime() > right->getTime();//// older deal comes first (so when right smaller than left, right is first)
	
	// for sets 
	if (left->getBuySellFlag() == Order::BUY)
	{
		if (left->getPrice() > right->getPrice())// left larger than right, left comes first - a true val causes left to come first
			return true;

		if (left->getPrice() == right->getPrice())
			return left->getTime() < right->getTime(); // left smaller than right - so left comes first (caused a true evaluation) 

		return false;
	}
	else // sell
	{
		if (left->getPrice() < right->getPrice())// left smaller than right, left comes first - a true val causes left to come first
			return true;

		if (left->getPrice() == right->getPrice())
			return left->getQuantity() < right->getQuantity(); // left smaller than right - so left comes first (caused a true evaluation) 

		return false;
	}
}

struct group_thousands : std::numpunct<char>
{
	std::string do_grouping() const override { return "\3"; }
};

std::string Order::getFormatedOrder() const
{
	std::ostringstream stm;
	//buy                   sell
	// quantity   price     price quantity
	//000,000,000 000000 | 000000 000,000,000
	//     50,000     99 |    105      14,600
	unsigned long arg1 =  m_buysell == BuySellFlag::BUY ? m_quantity : m_price;
	unsigned long arg2 =  m_buysell ==  BuySellFlag::BUY ?  m_price : m_quantity;
	int  width1 = m_buysell == BuySellFlag::BUY ? 11 : 6;
	int  width2 = m_buysell == BuySellFlag::BUY ? 6 : 11;

	stm.imbue(std::locale(std::cout.getloc(), new group_thousands));
	stm << std::setfill(' ') << std::setw(width1) << arg1 << " " << std::setfill(' ') << std::setw(width2) << arg2;

	return stm.str();
}

class OrderBook{
public:
	void Execute(std::shared_ptr<Order> newOrder);	
	std::string GetRestingAndTradedOrders() const;
	void AddToTradedHistory(const std::string& trade) { m_tradingHistory.push_back(trade); };
private:
	//typedef std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, std::less<std::shared_ptr<Order>> > Orders;
	typedef std::set<std::shared_ptr<Order>, std::less<std::shared_ptr<Order>>> Orders;
	Orders m_buyOrders;
	Orders m_sellOrders;
	std::vector<std::string>  m_tradingHistory;
	bool doOrdersMatch(const std::shared_ptr<Order>& order, const std::shared_ptr<Order>& newOrder, int& matchedPrice) const;
	void Match(std::shared_ptr<Order> newOrder, Orders& opoositeSide, Orders& OrderSide);
	std::string formatTradedOrder(const std::shared_ptr<Order>& order, const std::shared_ptr<Order>& newOrder, int price, int traedQuantity) const;
};

bool OrderBook:: doOrdersMatch(const std::shared_ptr<Order>& order, const std::shared_ptr<Order>& newOrder, int& matchedPrice) const
{
	// prices match:
	// buy vs sell <= if existing buy price is larger or equal to new order sell price then its a match
	// sell vs buy >= if existing sell price is smaller or equal to new order buy price then its a match
	if (order->getBuySellFlag() == Order::BUY)
	{
		if (order->getPrice() >= newOrder->getPrice())
		{
			matchedPrice = order->getPrice();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (order->getPrice() <= newOrder->getPrice())
		{
			matchedPrice = order->getPrice();
			return true;
		}
		else
		{
			return false;
		}
	}
}

std::string OrderBook::formatTradedOrder(const std::shared_ptr<Order>& order, const std::shared_ptr<Order>& newOrder, int price, int tradedQuantity) const
{
	std::ostringstream stm;
	// trade 10006,10001,100,500
	//       buyId sellId  price quantity
	std::string buyOrderId, sellOrderId;
	buyOrderId  = order->getBuySellFlag() == Order::BUY  ?  order->getOrderId()   : newOrder->getOrderId();
	sellOrderId = newOrder->getBuySellFlag() == Order::SELL ? newOrder->getOrderId() : order->getOrderId();
	stm << "trade " << buyOrderId << "," << sellOrderId << ","  << price << "," << tradedQuantity;
	return stm.str();
}

void OrderBook::Match(std::shared_ptr<Order> newOrder, Orders& opoositeSide, Orders& orderSide)
{

	if (opoositeSide.empty())
	{
		orderSide.insert(newOrder);
		return;
	}
	orderSide.insert(newOrder);
	// iterate over opposite orders
	for (auto order = opoositeSide.begin(); order != opoositeSide.end(); )
	{
		int matchedPrice = -1;
		if (doOrdersMatch((*order), newOrder, matchedPrice))
		{
			// we have a match
			signed int res = static_cast<signed int>((*order)->getQuantity() - newOrder->getQuantity());
			if (res < 0)
			{
				// if the result of subtraction is less than 0 - it means the exiting order in the book has been filled
				// (e.g. 300 (existing) - 500 (new) == -200 - so existing can be removed and remainder (-200) 
				// used as the new quantity of the new order. (new order object will die when ref count drops 
				// so create a new order object and add it to list)
				AddToTradedHistory(formatTradedOrder((*order), newOrder, matchedPrice, (*order)->getQuantity()));
				order = opoositeSide.erase(order);// erase inavlidates iterator so get next valid one through teh return of erase
				newOrder->setQuantity(std::abs(res));
			}
			else if (res > 0)
			{
				AddToTradedHistory(formatTradedOrder((*order), newOrder, matchedPrice, newOrder->getQuantity()));
				// positive means new order has been filled and existing one must be adjusted 
				orderSide.erase(newOrder);
				(*order)->setQuantity(res);
				return;//new order has been fully filled
			}
			else
			{
				AddToTradedHistory(formatTradedOrder((*order), newOrder, matchedPrice, newOrder->getQuantity()));
				// both fully filled - simply remove both order and new order form their lists
				order = opoositeSide.erase(order);
				orderSide.erase(newOrder);
				return;
			}
		}
		else
		{
			++order;
		}
	}
}

std::string OrderBook::GetRestingAndTradedOrders() const
{
	std::ostringstream stm;;
	for (std::string trade : m_tradingHistory)
	{
		stm << trade << "\n";
	}

	//to stdout
	//000,000,000 000000 | 000000 000,000,000
	//    50,000      99 |    105      14,600
	//	  m_buyOrders       m_sellOrders;

	Orders::const_iterator buy = m_buyOrders.begin();
	Orders::const_iterator sell = m_sellOrders.begin();
	while (buy != m_buyOrders.end() && sell != m_sellOrders.end())
	{
		stm << (*buy)->getFormatedOrder() << " | " << (*sell)->getFormatedOrder() << "\n";
		++buy; ++sell;
	}
	// display any residuals
	while (buy != m_buyOrders.end())
	{
		stm << (*buy)->getFormatedOrder() << " |\n";
		++buy;
	}
	while (sell != m_sellOrders.end())
	{
		stm << std::setw(21) << " | " << (*sell)->getFormatedOrder() << "\n";
		++sell;
	}

	return stm.str();
}

void OrderBook::Execute(std::shared_ptr<Order> newOrder)
{
	if (newOrder->getBuySellFlag() == Order::BuySellFlag::BUY)
	{
		Match(newOrder, m_sellOrders, m_buyOrders);
	}
	else
	{
		Match(newOrder, m_buyOrders, m_sellOrders);
	}

}

void Trim(std::string& s)
{
	// ----abcd
	// find return position of a which is teh first non space char
	size_t found = s.find_first_not_of(" ");
	s = s.substr(found);

	// find the last character that is NOT a space
	// abcdef__
	// found will point to the first space at the end of the string
	found = s.find_last_not_of(" ");
	if (found != std::string::npos)
		s.erase(found + 1);
}



/*
  std::string str() const
    {
        std::ostringstream stm ;

        stm << std::left
            << std::setw(15) << _person.name()
            << std::setw(15) << _person.surname()
            << std::setw(8) << _person.gender() ;

        return stm.str() ;
    }

	// this allows you to say std::cout << student which then calls
	// str() which is defined in student and this function streams student data memebers to a stringstream and returns a string  
	// which cout can that output to std out

    friend std::ostream& operator<<( std::ostream& stm, const Student& s )
    { return stm << s.str() ; }
*/


int main()
{
	OrderBook orderBook;
	
	std::vector<std::vector<std::string>> vec = { 
													{"10000", "B", "98",  "25500"},
													{"10005", "S", "105", "20000"},
													{"10001", "S", "100", "500"},
													{"10002", "S", "100", "10000"},
													{"10003", "B", "99",  "50000"},
													{"10004", "S", "103", "100"},
													{"10006", "B", "105", "16000"}
												};
    
	const std::string expected = "trade 10006,10001,100,500\n"
						"trade 10006, 10002, 100, 10000\n"
						"trade 10006, 10004, 103, 100\n"
						"trade 10006, 10005, 105, 5400"
						"50, 000     99 | 105      14, 600\n"
						"25, 500     98 |\n";
	for (std::vector<std::string> fields : vec)
	{
		unsigned int price		= atoi(fields[2].c_str());
		unsigned long quantity	= atoi(fields[3].c_str());
		std::shared_ptr<Order> order = std::make_shared<Order>(std::move(fields[0]),
			price, quantity, fields[1] == "B" ? Order::BuySellFlag::BUY : Order::BuySellFlag::SELL,
			std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
		
		orderBook.Execute(order);
	}
	std::cout << orderBook.GetRestingAndTradedOrders();

	if (orderBook.GetRestingAndTradedOrders().compare(expected) == 0)
	{
		std::cout << "PASS\n";
	}

	/*
	while (1) {
		std::string line;
		std::cout << "Enter the order in the format (order Id, Buy/Sell, price, quantity) 10000, B, 98, 2500\n";
		std::getline(std::cin, line);
		std::stringstream ss(line);
		std::string field;
		std::vector<std::string> fields;
		while (std::getline(ss, field, ',')) {
			if (ss.fail()) {
				std::cout << "Incorrect format - please enter in the format (order Id, Buy/Sell, price, quantity) e.g.  10000, B, 98, 2500 \n";
				ss.clear();
				break;
			}
			Trim(field);
			fields.push_back(field);
		}

		unsigned int price  = atoi(fields[2].c_str());
		if (price > 999999)
		{
			std::cout << "Please ensure price is in the permitted range up to 999999, please try again" << std::endl;
			continue;
		}
		unsigned long quantity = atoi(fields[3].c_str());
		if (quantity > 999999999)
		{
			std::cout << "Please ensure quantity  is in the permitted range up to 999999999, please try again" << std::endl;
			continue;
		}
		if (fields[1] !=  "B" && fields[1] != "S")
		{
			std::cout << "Please ensure direction is B or S, please try again" << std::endl;
			continue;
		}
	
		std::shared_ptr<Order> order = std::make_shared<Order>(std::move(fields[0]), 
															   price, quantity, fields[1] == "B" ? Order::BuySellFlag::BUY : Order::BuySellFlag::SELL,
				std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());

		orderBook.Execute(order);
	}
	*/
}

