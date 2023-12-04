#include <algorithm>
#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <vector>
namespace rng = std::ranges;

// Used when converting ascii digit to integer
constexpr int ZERO_ASCII_OFFSET = 48;
class line : public std::string {};
std::istream &operator>>(std::istream &is, line &l) {
  std::getline(is, l);
  return is;
}
int main() {
  // Open the file
  std::ifstream inputFile("input.txt");
  if (!inputFile.is_open()) {
    std::cerr << "Error opening the file." << std::endl;
    return 1;
  }

  int sum = 0;
  // Define a lambda function to process each line
  auto processLine = [&](std::string_view line) {
    static int cardid = 1;
    line.remove_prefix(std::min(line.find(':') + 1, line.size()));
    std::cout << " cardid " << cardid;
    std::cout << ":" << line << ":" << std::endl;

    auto pipe_pos = line.find('|');
    auto winning = std::string(line.begin(), line.begin() + pipe_pos);
    auto my_nums = std::string(line.begin() + pipe_pos + 1, line.end());
    std::cout << ":" << winning << ":" << std::endl;
    std::cout << ":" << my_nums << ":" << std::endl;

    // parse winning
    std::stringstream ss(winning);

    std::vector<int> winning_vec;
    int number;
    while (ss >> number) {
      winning_vec.push_back(number);
    }

    // parse my numbers
    ss.clear();
    ss.str(my_nums);

    std::vector<int> my_nums_vec;
    while (ss >> number) {
      my_nums_vec.push_back(number);
    }

    // for (int num : winning_vec) {
    //   std::cout << num << " ";
    // }
    // std::cout << std::endl;

    // for (int num : my_nums_vec) {
    //   std::cout << num << " ";
    // }
    // std::cout << std::endl;

    // check how many numbers won
    int points = 0;
    for (auto my_num : my_nums_vec) {
      if (rng::find(winning_vec, my_num) != winning_vec.end()) {
        std::cout << "contains " << my_num << std::endl;
        if (points == 0) {
          points = 1;
        } else {
          points *= 2;
        }
      }
    }

    std::cout << " points: " << points << std::endl;

    std::cout << std::endl;
    cardid++;
    sum += points;
  };

  // Read file line by line using ranges and apply the lambda function
  rng::for_each(rng::istream_view<line>(inputFile), processLine);

  std::cout << "Total points: " << sum << std::endl;

  inputFile.close();

  return 0;
}