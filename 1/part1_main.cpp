#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>

// Used when converting ascii digit to integer
constexpr int ZERO_ASCII_OFFSET = 48;

int main() {
  namespace rng = std::ranges;
  // Open the file
  std::ifstream inputFile("input.txt");

  if (!inputFile.is_open()) {
    std::cerr << "Error opening the file." << std::endl;
    return 1;
  }

  int sum = 0;

  // Define a lambda function to process each line
  auto processLine = [&sum](const std::string &line) {
    auto firstDigit = rng::find_if(line, ::isdigit);
    auto lastDigit = rng::find_if(line | std::views::reverse, ::isdigit);

    if (firstDigit != line.end() && lastDigit != line.rend()) {
      std::cout << "Line: " << line << "\n";
      std::cout << "First Digit: " << *firstDigit << "\n";
      std::cout << "Last Digit: " << *lastDigit << "\n";
      int temporary_number = (*firstDigit - ZERO_ASCII_OFFSET) * 10 +
                             *lastDigit - ZERO_ASCII_OFFSET;
      std::cout << "temporary_number: " << temporary_number << "\n\n";
      sum += temporary_number;
    } else {
      std::cout << "No digits found in line: " << line << "\n\n";
    }
  };

  // Read file line by line using ranges and apply the lambda function
  rng::for_each(rng::istream_view<std::string>(inputFile), processLine);

  std::cout << "output: " << sum << std::endl;

  inputFile.close();

  return 0;
}