#include <algorithm>
#include <array>
#include <cctype>
#include <charconv>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <ranges>
#include <sstream>
#include <string_view>
#include <variant>
#include <vector>
namespace rng = std::ranges;

namespace detail {
class line : public std::string {};
std::istream &operator>>(std::istream &is, line &l) {
  std::getline(is, l);
  return is;
}

struct colour_base {
  int val;
};
struct red : colour_base {};
struct green : colour_base {};
struct blue : colour_base {};
using colour = std::variant<red, green, blue>;

std::istream &operator>>(std::istream &is, colour &c) {
  std::string str;
  int tmp;
  is >> tmp;
  is >> str;
  switch (str[0]) {
  case 'r':
    c = red{tmp};
    break;
  case 'g':
    c = green{tmp};
    break;
  case 'b':
    c = blue{tmp};
    break;
  default:
    c = blue{0};
    break;
  }
  return is;
}

} // namespace detail

auto parse_group_helper(std::string_view group) -> std::tuple<int, int, int> {
  int red = 0, green = 0, blue = 0, tmp;

  while (!group.empty()) {
    auto delimiter_index = group.find(",");
    std::string pair = std::string(group.substr(0, delimiter_index));
    std::cout << " PARSING PAIR:'" << pair << "'" << '\n';
    detail::colour col;
    std::stringstream(pair) >> col;
    if (const auto pval = std::get_if<detail::red>(&col)) {

      //   std::cout << "variant red value: " << (*pval).val << '\n';
      red += (*pval).val;
    } else if (const auto pval = std::get_if<detail::green>(&col)) {
      //   std::cout << "variant green value: " << (*pval).val << '\n';
      green += (*pval).val;
    } else if (const auto pval = std::get_if<detail::blue>(&col)) {
      //   std::cout << "variant blue value: " << (*pval).val << '\n';
      blue += (*pval).val;
    } else
      std::cout << "failed to get value!" << '\n';

    if (delimiter_index == group.npos) {
      break;
    }
    group.remove_prefix(
        std::min(group.find(' ', delimiter_index) + 1, group.size()));
  }
  return {red, green, blue};
}

int main() {
  // Open the file
  std::ifstream inputFile("input.txt");

  if (!inputFile.is_open()) {
    std::cerr << "Error opening the file." << std::endl;
    return 1;
  }

  long int sum = 0;

  // Define a lambda function to process each line
  auto processLine = [&sum](std::string_view line) {
    int gameid = 0;
    line.remove_prefix(std::min(line.find(' ') + 1, line.size()));
    std::from_chars(line.begin(), line.begin() + line.find(':'), gameid);
    std::cout << " gameid " << gameid << " -- ";
    line.remove_prefix(std::min(line.find(' ') + 1, line.size()));

    std::cout << line << '\n';
    while (!line.empty()) {
      auto delimiter_index = line.find(";");
      auto group = line.substr(0, delimiter_index);
      std::cout << " PARSING GROUP:" << group << '\n';
      auto [red, green, blue] = parse_group_helper(group);
      std::cout << red << " " << green << " " << blue << " " << '\n';
      if (red > 12 || green > 13 || blue > 14) {
        std::cout << "COS TU SIE NIE ZGADZA - NIE DODAJEMY: " << gameid << "\n";
        std::cout << std::endl;
        return;
      }

      if (delimiter_index == line.npos) {
        break;
      }
      line.remove_prefix(
          std::min(line.find(' ', delimiter_index) + 1, line.size()));
    }
    std::cout << "WSZYSTKO OK DODAJEMY: " << gameid << "\n";
    sum += gameid;
    std::cout << std::endl;
  };

  // Read file line by line using ranges and apply the lambda function
  rng::for_each(rng::istream_view<detail::line>(inputFile), processLine);

  std::cout << "output: " << sum << std::endl;

  inputFile.close();

  return 0;
}
