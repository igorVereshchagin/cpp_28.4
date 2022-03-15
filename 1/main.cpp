#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <string>

int main()
{
  std::map<std::string, double> competitors;
  for (int i = 0; i < 6; i++)
  {
    std::string name;
    double speed;
    bool goodInput;
    do
    {
      std::cout << "Competitor " << i + 1 << std::endl << "Input name: " << std::endl;
      std::cin >> std::ws;
      std::getline(std::cin, name);
      if (competitors.end() != competitors.find(name))
      {
        std::cout << "The name is used. Try again" << std::endl;
        goodInput = false;
        continue;
      }
      else
        goodInput = true;
      std::cout << "Input speed: " << std::endl;
      std::cin >> speed;
    } while (!goodInput);
    competitors[name] = speed;

  }
  std::cout << "Hello, World!" << std::endl;
  return 0;
}
