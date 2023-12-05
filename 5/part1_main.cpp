#include <algorithm>
#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <vector>
namespace rng = std::ranges;

class line : public std::string {};
std::istream &operator>>(std::istream &is, line &l) {
  std::getline(is, l);
  return is;
}
int main() {
  // Open the file
  std::ifstream inputFile("input.txt"); // ADD ADDITIONAL NEWLINE TO THE INPUT!!
  if (!inputFile.is_open()) {
    std::cerr << "Error opening the file." << std::endl;
    return 1;
  }

  // get seeds line
  std::string seeds;
  std::getline(inputFile, seeds);
  seeds = seeds.substr(seeds.find(' '));

  // parse seeds line
  std::stringstream ss(seeds);
  std::vector<long int> seeds_vec;
  long int seed;
  while (ss >> seed) {
    seeds_vec.push_back(seed);
  }
  std::cout << "seeds:" << std::endl;

  for (long int num : seeds_vec) {
    std::cout << num << ":";
  }
  std::cout << std::endl;

  // parsing maps
  std::getline(inputFile, seeds);

  std::cout << "parsing maps:" << std::endl << std::endl;
  // Define a lambda function to process each line
  auto processLine = [&](std::string &line) {
    static std::vector<std::string> current_map;
    if (line.contains(':')) {
      std::cout << line << std::endl;
      return;
    }

    auto process_map = [&](std::vector<std::string> &map) {
      static std::istringstream ss;
      // for each seed from seed vec check the range and apply the mapping
      auto process_seed = [&](auto &seed) {
        std::cout << "Checking seed: " << seed << std::endl;
        for (auto &range_str : map) {
          // convert line to array of long ints
          std::array<long int, 3> range_long_int;
          ss.clear();
          ss.str(range_str);
          for (long int temp, i = 0; ss >> temp; ++i) {
            range_long_int[i] = temp;
          }
          for (auto &map : range_long_int) {
            std::cout << map << "-";
          }
          std::cout << std::endl;
          // check if we can apply the range
          long int min = range_long_int[1],
                   max = range_long_int[1] + range_long_int[2];
          // std::cout << "min:" << min << " max:" << max << std::endl;
          if (seed >= min && seed < max) {
            long int new_seed_value =
                seed - range_long_int[1] + range_long_int[0];
            std::cout << "YES! we can apply this map " << seed << "->"
                      << new_seed_value << std::endl;
            seed = new_seed_value;
            return;
          };
        }
      };
      rng::for_each(seeds_vec, process_seed);
    };
    if (line.empty()) {
      for (auto &map : current_map) {
        std::cout << map << ":";
      }
      std::cout << "<processing the map start>" << std::endl;
      process_map(current_map);
      std::cout << "<processing the map stop>" << std::endl;
      std::cout << "seeds:" << std::endl;

      for (long int num : seeds_vec) {
        std::cout << num << ":";
      }
      std::cout << std::endl << std::endl;
      current_map = {};
      return;
    }
    current_map.push_back(line);
  };

  // Read file line by line using ranges and apply the lambda function
  rng::for_each(rng::istream_view<line>(inputFile), processLine);

  std::cout << "Lowest location is: " << rng::min(seeds_vec) << std::endl;

  inputFile.close();

  return 0;
}