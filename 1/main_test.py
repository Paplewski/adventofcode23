def parse_line(line):
    parsed_line = []
    print("LINE ", line.strip())
    index = 0
    while index < len(line):
        if line[index].isdigit():
            parsed_line.append((index, "digit", line[index]))  # (index, is digit)
            index += 1
        else:
            for digit, spelled_digit in enumerate(["zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"]):
                if line.startswith(spelled_digit, index):
                    parsed_line.append((index, "spelled", spelled_digit,str(digit)))  # (index, is spelled)
                    index += 1
                    break
            else:
                index += 1
    print("PARSED", parsed_line)
    return parsed_line
import sys

def process_file(file_path):
    sum = 0
    with open(file_path, 'r') as file:
        for line_number, line in enumerate(file, start=1):
            parsed = parse_line(line)
            parsed.sort(key=lambda x:x[0])
            print("sorted: ", parsed) # is it necessary?
            
            first_digit =   parsed[0][2] if parsed[0][1] == 'digit' else parsed[0][3]
            last_digit =   parsed[-1][2] if parsed[-1][1] == 'digit' else parsed[-1][3]
            # Find first and last digit
            # first_digit, last_digit = find_first_and_last_digit(parsed) || (None, None)
            
            # Display the results
            print(f"Line {line_number}:")
            print(f"Original line: {line.strip()}")
            print(f"Line after replacing spelled digits: {line.strip()}")
            print(f"First digit: {first_digit}")
            print(f"Last digit: {last_digit}")
            print("\n")
            inted = int(first_digit + last_digit)
            print(f"inted: ",inted)
            print(f'{inted} {line.strip()}', file=sys.stderr)
            sum = sum + inted
            print("\n")
    print("sum:", sum)

# Example usage: Replace 'your_file.txt' with the path to your actual file

# Example usage: Replace 'your_file.txt' with the path to your actual file
process_file('input.txt')