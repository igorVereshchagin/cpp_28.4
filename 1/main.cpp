#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <string>
#include <mutex>

void inputCompetitors(int n, std::vector<std::pair<std::string, double>> &outCompetitors);
void swimming(const std::pair<std::string, double> &competitor);

std::mutex cout_access;
std::mutex results_access;

std::map<double, std::vector<std::string>> results;

int main()
{
  std::vector<std::pair<std::string, double>> competitors;

  inputCompetitors(6, competitors);

  std::thread *race[6];

  for (int i = 0; i < 6; i++)
    race[i] = new std::thread(swimming, competitors[i]);

  for (int i = 0; i < 6; i++)
    race[i]->join();
  std::cout << "======== Results =========" << std::endl;
  for (std::map<double, std::vector<std::string>>::iterator it = results.begin(); it != results.end(); it ++)
  {
    for (int i = 0; i < it->second.size(); i++)
      std::cout << it->second[i] << ": " << it->first / 1000000 << std::endl;
  }
  return 0;
}

void swimming(const std::pair<std::string, double> &competitor)
{
  double distance = 100;
  double ksi = 0.0001;
  double result = 0;
  while(distance > 0 && fabs(distance) > ksi)
  {
    double speed = competitor.second;
    int sleepTimeUs = (distance > speed) ? 1000000 : floor((distance * 1000000) / speed + 0.5);
    std::this_thread::sleep_for(std::chrono::microseconds (sleepTimeUs));
    distance -= sleepTimeUs * speed / 1000000;
    result += sleepTimeUs;
    cout_access.lock();
    std::cout << "Competitor " << competitor.first << " swam " << 100.0 - distance << " meters" << std::endl;
    cout_access.unlock();
  }
  results_access.lock();
  if (results.find(result) == results.end())
    results[result] = std::vector<std::string>();
  results[result].push_back(competitor.first);
  results_access.unlock();
}

void inputCompetitors(int n, std::vector<std::pair<std::string, double>> &outCompetitors)
{
  for (int i = 0; i < n; i++)
  {
    std::string name;
    double speed;
    std::cout << "Competitor " << i + 1 << std::endl << "Input name: " << std::endl;
    std::cin >> std::ws;
    std::getline(std::cin, name);
    std::cout << "Input speed: " << std::endl;
    std::cin >> speed;
    outCompetitors.push_back(std::make_pair(name, speed));
  }
}
