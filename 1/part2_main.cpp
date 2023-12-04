#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>

// Used when converting ascii digit to integer
constexpr int ZERO_ASCII_OFFSET = 48;

static auto custom_find_digit_helper(std::string_view line) -> int {
  for (auto it = line.cbegin(); it != line.cend(); it++) {
    if (std::isdigit(*it)) {
      std::cout << "is digit: " << *it << '\n';
      return *it - ZERO_ASCII_OFFSET;
    } else {
      static const std::vector<std::string> spelledOutDigits = {
          "zero", "one", "two",   "three", "four",
          "five", "six", "seven", "eight", "nine"};
      std::string_view from_current_to_end(it, line.cend());
      for (auto index = 0; index < spelledOutDigits.size(); ++index) {
        if (from_current_to_end.starts_with(spelledOutDigits[index])) {
          // std::cout << "string view: " << from_current_to_end << '\n';
          std::cout << "converted: " << index << '\n';
          return index;
        }
      }
    }
  }
  return 0;
}

static auto custom_find_digit_helper_rev(std::string_view line) -> int {
  for (auto it = line.crbegin(); it != line.crend(); it++) {
    if (std::isdigit(*it)) {
      std::cout << "is digit: " << *it << '\n';
      return *it - ZERO_ASCII_OFFSET;
    } else {
      static const std::vector<std::string> spelledOutDigits = {
          "orez", "eno", "owt",   "eerht", "ruof",
          "evif", "xis", "neves", "thgie", "enin"};
      std::string from_current_to_begin(it, line.crend());
      for (auto index = 0; index < spelledOutDigits.size(); ++index) {
        if (from_current_to_begin.starts_with(spelledOutDigits[index])) {
          // std::cout << "string view: " << from_current_to_begin << '\n';
          std::cout << "converted: " << index << '\n';
          return index;
        }
      }
    }
  }
  return 0;
}

int main() {
  namespace rng = std::ranges;
  // Open the file
  std::ifstream inputFile("input.txt");

  if (!inputFile.is_open()) {
    std::cerr << "Error opening the file." << std::endl;
    return 1;
  }

  long int sum = 0;

  // Define a lambda function to process each line
  auto processLine = [&sum](const std::string &line) {
    static int count = 0;
    std::cout << "line: " << line << " coutn: " << ++count << '\n';
    int firstDigit = custom_find_digit_helper(line);
    int lastDigit = custom_find_digit_helper_rev(line);
    // line | std::views::reverse |
    // std::ranges::to<std::vector>()); // use ranges::to<stringview>()  from
    // std23
    sum += 10 * firstDigit + lastDigit;
    std::cout << "sum: " << sum << '\n' << '\n';
  };

  // Read file line by line using ranges and apply the lambda function
  rng::for_each(rng::istream_view<std::string>(inputFile), processLine);

  std::cout << "output: " << sum << std::endl;

  inputFile.close();

  return 0;
}
