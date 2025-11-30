// matching-engine.cpp : Defines the entry point for the application.
//

#include "matching-engine.h"
#include <iostream>
#include <map>
#include <deque>
#include <vector>
#include <cstdint>
#include <algorithm>

// ================================
//  Tipos básicos
// ================================

enum class Side { Buy, Sell };
enum class OrderType { Limit, Market };

using OrderId = std::uint64_t;
using Price = std::int64_t;   // por simplicidad, precio en "ticks" (centavos)
using Quantity = std::int64_t;

struct Trade {
    OrderId buyId;
    OrderId sellId;
    Price   price;
    Quantity quantity;
};

// ================================
//  Orden
// ================================

struct Order {
    OrderId     id;
    Side        side;
    OrderType   type;
    Price       price;      // solo relevante para LIMIT
    Quantity    quantity;   // cantidad restante
    std::uint64_t timestamp{ 0 }; // para prioridad (price-time)
};

// ================================
//  Libro de órdenes
// ================================
//
// bids_: compras, mejor precio = mayor precio (orden descendente)
// asks_: ventas, mejor precio = menor precio (orden ascendente)
//

class OrderBook {
public:
    void addOrder(Order order) {
        order.timestamp = ++nextTimestamp_;

        if (order.type == OrderType::Limit) {
            matchLimit(order);
        }
        else {
            matchMarket(order);
        }
    }

    const std::vector<Trade>& trades() const { return trades_; }

    void printOrderBook() const {
        std::cout << "\n=== ORDER BOOK ===\n";

        std::cout << "ASKS (ventas):\n";
        for (const auto& [price, queue] : asks_) {
            Quantity totalQty = 0;
            for (const auto& o : queue) {
                totalQty += o.quantity;
            }
            std::cout << "  P=" << price << "  Q=" << totalQty << '\n';
        }

        std::cout << "BIDS (compras):\n";
        for (const auto& [price, queue] : bids_) {
            Quantity totalQty = 0;
            for (const auto& o : queue) {
                totalQty += o.quantity;
            }
            std::cout << "  P=" << price << "  Q=" << totalQty << '\n';
        }
        std::cout << "===================\n\n";
    }

private:
    // Bids: mayor precio primero
    std::map<Price, std::deque<Order>, std::greater<Price>> bids_;
    // Asks: menor precio primero
    std::map<Price, std::deque<Order>> asks_;

    std::uint64_t nextTimestamp_{ 0 };
    std::vector<Trade> trades_;

    void recordTrade(const Order& buy, const Order& sell, Price price, Quantity qty) {
        trades_.push_back(Trade{ buy.id, sell.id, price, qty });

        std::cout << "TRADE: "
            << "Buy#" << buy.id
            << " <-> Sell#" << sell.id
            << " @ " << price
            << " x " << qty
            << '\n';
    }

    void matchLimit(Order& order) {
        if (order.side == Side::Buy) {
            matchLimitBuy(order);
        }
        else {
            matchLimitSell(order);
        }
    }

    void matchMarket(Order& order) {
        if (order.side == Side::Buy) {
            matchMarketBuy(order);
        }
        else {
            matchMarketSell(order);
        }
    }

    // ==============
    // LIMIT BUY
    // ==============
    void matchLimitBuy(Order& order) {
        auto it = asks_.begin(); // mejor ask (menor precio)

        while (order.quantity > 0 &&
            it != asks_.end() &&
            it->first <= order.price) {

            auto& queue = it->second; // deque<Order>

            while (!queue.empty() && order.quantity > 0) {
                Order& bestSell = queue.front();

                Quantity tradedQty = std::min(order.quantity, bestSell.quantity);
                recordTrade(order, bestSell, it->first, tradedQty);

                order.quantity -= tradedQty;
                bestSell.quantity -= tradedQty;

                if (bestSell.quantity == 0) {
                    queue.pop_front();
                }
            }

            if (queue.empty()) {
                it = asks_.erase(it);
            }
            else {
                ++it;
            }
        }

        // Si queda remanente, se queda en el libro como BID
        if (order.quantity > 0) {
            bids_[order.price].push_back(order);
        }
    }

    // ==============
    // LIMIT SELL
    // ==============
    void matchLimitSell(Order& order) {
        auto it = bids_.begin(); // mejor bid (mayor precio)

        while (order.quantity > 0 &&
            it != bids_.end() &&
            it->first >= order.price) {

            auto& queue = it->second; // deque<Order>

            while (!queue.empty() && order.quantity > 0) {
                Order& bestBuy = queue.front();

                Quantity tradedQty = std::min(order.quantity, bestBuy.quantity);
                recordTrade(bestBuy, order, it->first, tradedQty);

                order.quantity -= tradedQty;
                bestBuy.quantity -= tradedQty;

                if (bestBuy.quantity == 0) {
                    queue.pop_front();
                }
            }

            if (queue.empty()) {
                it = bids_.erase(it);
            }
            else {
                ++it;
            }
        }

        // Si queda remanente, se queda en el libro como ASK
        if (order.quantity > 0) {
            asks_[order.price].push_back(order);
        }
    }

    // ==============
    // MARKET BUY
    // ==============
    void matchMarketBuy(Order& order) {
        auto it = asks_.begin(); // mejor ask

        while (order.quantity > 0 && it != asks_.end()) {
            auto& queue = it->second;

            while (!queue.empty() && order.quantity > 0) {
                Order& bestSell = queue.front();

                Quantity tradedQty = std::min(order.quantity, bestSell.quantity);
                recordTrade(order, bestSell, it->first, tradedQty);

                order.quantity -= tradedQty;
                bestSell.quantity -= tradedQty;

                if (bestSell.quantity == 0) {
                    queue.pop_front();
                }
            }

            if (queue.empty()) {
                it = asks_.erase(it);
            }
            else {
                ++it;
            }
        }

        // Nota: una MARKET que no se llena del todo simplemente se ejecuta hasta donde haya liquidez
    }

    // ==============
    // MARKET SELL
    // ==============
    void matchMarketSell(Order& order) {
        auto it = bids_.begin(); // mejor bid

        while (order.quantity > 0 && it != bids_.end()) {
            auto& queue = it->second;

            while (!queue.empty() && order.quantity > 0) {
                Order& bestBuy = queue.front();

                Quantity tradedQty = std::min(order.quantity, bestBuy.quantity);
                recordTrade(bestBuy, order, it->first, tradedQty);

                order.quantity -= tradedQty;
                bestBuy.quantity -= tradedQty;

                if (bestBuy.quantity == 0) {
                    queue.pop_front();
                }
            }

            if (queue.empty()) {
                it = bids_.erase(it);
            }
            else {
                ++it;
            }
        }
    }
};

// ================================
//  Ejemplo de uso
// ================================

int main() {
    OrderBook book;

    // 1) dos ventas limit
    book.addOrder(Order{ 1, Side::Sell, OrderType::Limit, 101, 50 });
    book.addOrder(Order{ 2, Side::Sell, OrderType::Limit, 102, 100 });

    // 2) dos compras limit
    book.addOrder(Order{ 3, Side::Buy,  OrderType::Limit, 100, 70 });
    book.addOrder(Order{ 4, Side::Buy,  OrderType::Limit,  99, 80 });

    book.printOrderBook();

    // 3) compra limit que cruza (precio 101)
    std::cout << "== Nueva orden LIMIT BUY 101 x 100 ==\n";
    book.addOrder(Order{ 5, Side::Buy, OrderType::Limit, 101, 100 });
    book.printOrderBook();

    // 4) venta MARKET que barre todo lo que pueda
    std::cout << "== Nueva orden MARKET SELL x 120 ==\n";
    book.addOrder(Order{ 6, Side::Sell, OrderType::Market, 0, 120 });
    book.printOrderBook();

    return 0;
}
