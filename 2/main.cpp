#include <iostream>
#include <string>
#include <thread>
#include <mutex>

std::mutex stationAccess;
std::mutex consoleAccess;

class Train
{
  std::string name;
  int travelTime;
public:
  Train(const std::string &inName,const int inTravelTime) : name(inName), travelTime(inTravelTime) {}
  void task();
};

void Train::task()
{
  std::this_thread::sleep_for(std::chrono::seconds(travelTime));
  if (!stationAccess.try_lock())
  {
    consoleAccess.lock();
    std::cout << "Train " << name << " is waiting..." << std::endl;
    consoleAccess.unlock();
  }
  else
    stationAccess.lock();
  consoleAccess.lock();
  std::cout << "Train " << name << " has arrived the station." << std::endl;
  std::string cmd;
  do
  {
    std::cin >> cmd;
  }while (cmd != "depart");
  std::cout << "Train " << name << " has departed." << std::endl;
  consoleAccess.unlock();
  stationAccess.unlock();
}

int main()
{
  Train *trains[3];
  for (int i = 0; i < 3; i++)
  {
    std::string name = "";
    int travelTime;
    name.push_back('A' + i);
    std::cout << "Input train "<< name << " travel time: " << std::endl;
    std::cin >> travelTime;
    trains[i] = new Train(name, travelTime);
  }
  std::thread *trainsThread[3];
  for (int i = 0; i < 3; i++)
    trainsThread[i]=]]]]]]]]]]]]]]]  = new std::thread(&trains[i]->task);

  for (int i = 0; i < 3; i++)
    delete trains[i];
  return 0;
}
