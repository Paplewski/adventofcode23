#include <algorithm>
#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>
namespace rng = std::ranges;

// Used when converting ascii digit to integer
constexpr int ZERO_ASCII_OFFSET = 48;

enum class character_type { none, digit, symbol };
std::ostream &operator<<(std::ostream &out, character_type c) {
  switch (c) {
  case character_type::none:
    out << "none";
    break;
  case character_type::digit:
    out << "digit";
    break;
  case character_type::symbol:
    out << "symbol";
    break;
  default:
    break;
  }
  return out;
}
struct Part {
  int line_no;
  std::vector<std::tuple<size_t, char>> digits_vec;
  bool is_valid;
};
int convert_part_to_int(Part &part);

std::ostream &operator<<(std::ostream &out, Part p) {
  out << std::format("Part(line:{},", p.line_no);
  for (const auto &tuple : p.digits_vec) {
    out << "(" << std::get<0>(tuple) << ", '" << std::get<1>(tuple) << "') ";
  }
  out << std::format(") is_valid:{})", p.is_valid);
  return out;
}
struct Symbol {
  int x, y;
  char c;
};
std::ostream &operator<<(std::ostream &out, Symbol c) {
  out << std::format("Symbol({},{},'{}')", c.x, c.y, c.c);
  return out;
}
struct Schematic {
  std::vector<Part> parts;
  std::vector<Symbol> symbols;
  long int accumulator = 0;
  void add_symbol(Symbol sym) {
    symbols.push_back(sym);
    auto should_apply_symbol = [&](Part &part) -> bool {
      bool check_adjacent_left =
          part.line_no == sym.x &&
          std::get<0>(part.digits_vec.back()) == sym.y - 1;
      auto check_adjacent_top = [&]() -> bool {
        if (part.line_no == sym.x - 1) {
          std::vector<int> indices;
          //   get indices
          rng::transform(part.digits_vec, std::back_inserter(indices),
                         [](auto dig) { return std::get<0>(dig); });
          if (rng::find(indices, sym.y - 1) != indices.end())
            return true;
          if (rng::find(indices, sym.y) != indices.end())
            return true;
          if (rng::find(indices, sym.y + 1) != indices.end())
            return true;
        }
        return false;
      };

      if (sym.x == 0) {
        return check_adjacent_left;
      } else {
        return check_adjacent_left || check_adjacent_top();
      }
      return false;
    };

    for (auto &part : this->parts | std::views::filter(should_apply_symbol)) {
      this->make_part_valid(part);
    }
  }
  void add_part(Part part) {
    std::cout << "ADDING PART\n" << part << "\n";

    int horizontal_min = std::get<0>(part.digits_vec[0]) - 1;
    int horizontal_max = std::get<0>(part.digits_vec.back()) + 1;
    // std::cout << "horizontal_min:" << horizontal_min << "\n";
    // std::cout << "horizontal_max:" << horizontal_max << "\n";

    auto is_symbol_adjacent = [&](Symbol &sym) {
      if ((sym.x == part.line_no || sym.x == (part.line_no - 1)) &&
          sym.y >= horizontal_min && sym.y <= horizontal_max) {
        return true;
      }
      return false;
    };
    if (auto result = rng::find_if(this->symbols, is_symbol_adjacent);
        result != this->symbols.end()) {
      std::cout << " SYM MATCH: " << *result << "\n";
      this->make_part_valid(part);
    }

    parts.push_back(part);
  }
  void make_part_valid(Part &part) {
    part.is_valid = true;
    std::cout << "MAKING PART VALID: " << part << "\n";

    this->accumulator += convert_part_to_int(part);

    // update accumulator
  }
};
std::ostream &operator<<(std::ostream &out, Schematic s) {
  for (const auto &sym : s.symbols) {
    out << sym << '\n';
  }
  for (const auto &p : s.parts) {
    out << p << '\n';
  }
  return out;
}

int convert_part_to_int(Part &part) {
  // convert part digit to a number
  std::vector<int> digits;
  //   get indices
  rng::transform(part.digits_vec, std::back_inserter(digits),
                 [](auto dig) { return std::get<1>(dig) - ZERO_ASCII_OFFSET; });

  int result = 0;

  for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
    result += *it * static_cast<int>(
                        std::pow(10, std::distance(digits.rbegin(), it)));
  }
  return result;
}

int main() {
  // Open the file
  std::ifstream inputFile("input.txt");
  if (!inputFile.is_open()) {
    std::cerr << "Error opening the file." << std::endl;
    return 1;
  }

  int sum = 0;
  Schematic schem{};
  // Define a lambda function to process each line
  auto processLine = [&](std::string_view line) {
    static int linecount = 0;
    std::cout << linecount << ":" << line << ":" << std::endl;
    character_type type;
    auto DigitOrSymbol = [](auto c) -> character_type {
      if ('.' == c)
        return character_type::none;
      if (std::isdigit(c)) {
        return character_type::digit;
      } else {
        return character_type::symbol;
      }
    };
    for (auto it = line.begin(); it != line.end(); it++) {
      character_type my_type = DigitOrSymbol(*it);
      if (my_type == character_type::none) {
        // std::cout << my_type << '\n';

      } else if (my_type == character_type::symbol) {
        Symbol s{linecount, static_cast<int>(std::distance(line.begin(), it)),
                 *it};
        std::cout << s << std::endl;
        schem.add_symbol(s);
      } else {
        std::cout << my_type << "\n";

        // Vector to store tuples (index, char)
        std::vector<std::tuple<size_t, char>> result;
        std::string hello = "hello";

        auto digits =
            rng::take_while_view(std::string_view(it, line.end()), ::isdigit);
        int index = 0;
        int number_base = static_cast<int>(std::distance(line.begin(), it));
        // Transform the string to vector of tuples
        rng::transform(digits, std::back_inserter(result), [&](char c) mutable {
          return std::make_tuple(number_base + index++, c);
        });

        schem.add_part(Part{linecount, result});

        std::cout << std::endl;
        std::advance(it, index - 1);
      }
    }

    std::cout << std::endl;
    linecount++;
  };

  // Read file line by line using ranges and apply the lambda function
  rng::for_each(rng::istream_view<std::string>(inputFile), processLine);

  // std::cout << "Schematics:\n" << schem << std::endl << std::endl;
  // std::cout << "Total sum:\n" << schem.accumulator << std::endl;

  // part 2 - getting gears
  schem.accumulator = 0;
  std::cout << "Potential gears:\n" << std::endl;

  auto is_gear = [](Symbol &sym) -> bool { return sym.c == '*'; };

  for (auto &sym : schem.symbols | std::views::filter(is_gear)) {
    int num_of_adjacent_parts = 0;
    int temp_sum = 1;
    std::cout << "Processing: " << sym << std::endl;

    auto process_adjacent_parts = [&](Part &part) {
      if (!(part.line_no == sym.x - 1 || part.line_no == sym.x ||
            part.line_no == sym.x + 1)) {
        return;
      }
      std::array<int, 3> horizontal_inices{sym.y - 1, sym.y, sym.y + 1};

      //   get indices
      std::vector<int> part_indices;
      rng::transform(part.digits_vec, std::back_inserter(part_indices),
                     [](auto dig) { return std::get<0>(dig); });

      // check if any horizontal idx is in part indices
      for (auto sym_idx : horizontal_inices) {
        if (rng::find(part_indices, sym_idx) != part_indices.end()) {
          // if found it is adjacent
          num_of_adjacent_parts++;
          int converted = convert_part_to_int(part);

          temp_sum *= converted;
          return;
        }
      }
    };
    rng::for_each(schem.parts, process_adjacent_parts);

    if (num_of_adjacent_parts == 2) {
      std::cout << "temp_sum " << temp_sum << std::endl << std::endl;
      schem.accumulator += temp_sum;
    }
  }

  std::cout << "Total sum PART2:\n" << schem.accumulator << std::endl;

  inputFile.close();

  return 0;
}