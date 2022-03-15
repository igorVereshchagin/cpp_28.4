#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <string>

void inputCompetitors(int n, std::vector<std::pair<std::string, double>> &outCompetitors);
void someFunc(std::pair<std::string, double> &inPair);
void race(const std::pair<std::string, double> &competitor, );


int main()
{
  std::vector<std::pair<std::string, double>> competitors;

  inputCompetitors(6, competitors);

  for (int i = 0; i < 6; i++)
    someFunc(competitors[i]);

  return 0;
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

void someFunc(std::pair<std::string, double> &inPair)
{
  std::cout << inPair.first << " " << inPair.second << std::endl;
}