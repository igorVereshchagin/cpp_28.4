#include <iostream>
#include <queue>
#include <mutex>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <string>
#include <cassert>

enum OrderType
{
  OT_min = 0,
  OT_pizza = OT_min,
  OT_soup,
  OT_steak,
  OT_salad,
  OT_sushi,
  OT_max = OT_sushi
};

struct Order
{
  OrderType type;
  std::string name;
};

void waiterTask();
void kitchenTask();
void courierTask();

std::queue<Order*> newOrders;
std::queue<Order*> completedOrders;
std::mutex newOrdersAccess;
std::mutex completedOrdersAccess;
std::mutex consoleAccess;
std::mutex kitchenSync;
std::mutex courierSync;
int main()
{
  std::srand(std::time(nullptr));
  std::thread waiter(waiterTask);
  std::this_thread::sleep_for(std::chrono::microseconds (1));
  std::thread kitchen(kitchenTask);
  std::this_thread::sleep_for(std::chrono::microseconds (1));
  std::thread courier(courierTask);
  waiter.join();
  kitchen.join();
  courier.join();
  return 0;
}

void waiterTask()
{
  newOrdersAccess.lock();
  int ordersCounter = 0;
  int pizzaCounter = 0;
  int soupCounter = 0;
  int steakCounter = 0;
  int saladCounter = 0;
  int sushiCounter = 0;
  while (ordersCounter < 10)
  {
    std::this_thread::sleep_for(std::chrono::seconds((std::rand() % 5) + 5));
    Order* newOrder = new Order();
    newOrder->type = OrderType(std::rand() % OT_max);
    switch (newOrder->type)
    {
      case OT_pizza:
        newOrder->name = "Pizza #" + std::to_string(++pizzaCounter);
        break;
      case OT_soup:
        newOrder->name = "Soup #" + std::to_string(++soupCounter);
        break;
      case OT_steak:
        newOrder->name = "Steak #" + std::to_string(++steakCounter);
        break;
      case OT_salad:
        newOrder->name = "Salad #" + std::to_string(++saladCounter);
        break;
      case OT_sushi:
        newOrder->name = "Sushi #" + std::to_string(++sushiCounter);
        break;
    }
    consoleAccess.lock();
    std::cout << "The order: " << newOrder->name << " is accepted" << std::endl;
    consoleAccess.unlock();
    newOrders.push(newOrder);
    newOrder = nullptr;
    ordersCounter++;
    newOrdersAccess.unlock();
    kitchenSync.lock();
    newOrdersAccess.lock();
    kitchenSync.unlock();
  }
}

void kitchenTask()
{
  completedOrdersAccess.lock();
  kitchenSync.lock();
  int ordersCounter = 0;
  Order* myOrder;
  while (ordersCounter < 10)
  {
    newOrdersAccess.lock();
    kitchenSync.unlock();
    assert(!newOrders.empty());
    myOrder = newOrders.front();
    newOrders.pop();
    newOrdersAccess.unlock();
    kitchenSync.lock();
    consoleAccess.lock();
    std::cout << "The order: " << myOrder->name << " is on the kitchen" << std::endl;
    consoleAccess.unlock();
    std::this_thread::sleep_for(std::chrono::seconds((std::rand() % 10) + 5));
    consoleAccess.lock();
    std::cout << "The order: " << myOrder->name << " is complete" << std::endl;
    consoleAccess.unlock();
    completedOrders.push(myOrder);
    myOrder = nullptr;
    ordersCounter++;
    completedOrdersAccess.unlock();
    courierSync.lock();
    completedOrdersAccess.lock();
    courierSync.unlock();
  }
}

void courierTask()
{
  courierSync.lock();
  int ordersCounter = 0;
  std::queue<Order*> myOrders;
  while (ordersCounter < 10)
  {
    std::this_thread::sleep_for(std::chrono::seconds(30));
    completedOrdersAccess.lock();
    courierSync.unlock();
    while (!completedOrders.empty())
    {
      myOrders.push(completedOrders.front());
      completedOrders.pop();
    }
    completedOrdersAccess.unlock();
    courierSync.lock();

    while (!myOrders.empty())
    {
      Order* order = myOrders.front();
      myOrders.pop();
      consoleAccess.lock();
      std::cout << "The order: " << order->name << " is delivered" << std::endl;
      consoleAccess.unlock();
      delete order;
      ordersCounter++;
    }
  }
}