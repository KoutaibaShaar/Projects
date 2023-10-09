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
	typedef std::vector<std::shared_ptr<Order>> Orders;
	static const int size = 17;
	typedef std::pair<std::array<char, size>, Orders::iterator> Ids;
private:
	Orders m_Orders;
	typedef std::vector<Ids> OrdersById;
	OrdersById m_OrdersById;
	void insertSorted(const std::shared_ptr<Order>& order);

	auto cbeginOrders() const { return m_Orders.cbegin(); }
	auto cendOrders()   const { return m_Orders.cend(); }

	auto cbeginOrdersById() const { return m_OrdersById.cbegin(); }
	auto cendOrdersById()   const { return m_OrdersById.cend(); }

};


