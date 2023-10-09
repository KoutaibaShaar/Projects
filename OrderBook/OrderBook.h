#pragma once
#include <string>
#include <set>
#include<vector>
#include<memory>
#include <map>

class Order;

class OrderBook {
public:
	OrderBook() = default;
	~OrderBook() = default;
	using  PriceAndAmount = std::pair<signed int, uint32_t>;
	uint32_t QueuePosition(const std::string& orderId) const noexcept;
	std::vector<OrderBook::PriceAndAmount> TopOfBook() const noexcept;
	int processIncomingUpdate(const std::string& update) noexcept;
	// for testing purposes
	void ClearBook() noexcept;
	std::shared_ptr<Order> getOrderAtPosition(size_t i) const noexcept;
	std::shared_ptr<Order> getOrderById(const std::string& orderId) const noexcept;
	size_t getNumberofOrders() const noexcept { return m_Orders.size(); }
	friend std::ostream& operator<<(std::ostream& out, const OrderBook& orderBook);
	// orders are sorted by price with orders (in descending order) and then by arrival time (ascending order)
	typedef std::multiset<std::shared_ptr<Order>, std::less<std::shared_ptr<Order>>> Orders;
	Orders m_Orders;
	// to enable quicklook up for Delete updates (which use the UniqueOrder ID to identify order to be deleted)
	typedef std::map<std::string, std::shared_ptr<Order>> OrdersById;
	OrdersById m_OrdersById;
private:
	

	auto cbeginOrders() const { return m_Orders.cbegin(); }
	auto cendOrders()   const { return m_Orders.cend(); }
	// to enable retrieving top prices - the requirement is book is sorted by largest price and then time of arrival so best prices will be last 
	auto crbeginOrders() const { return m_Orders.crbegin(); }
	auto crendOrders()   const { return m_Orders.crend(); }

	auto cbeginOrdersById() const { return m_OrdersById.cbegin(); }
	auto cendOrdersById()   const { return m_OrdersById.cend(); }
};


