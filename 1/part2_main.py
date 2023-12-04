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

def process_file(file_path):
    sum = 0
    with open(file_path, 'r') as file:
        for line_number, line in enumerate(file, start=1):
            print(f"Line {line_number}:")
            parsed = parse_line(line)
            first_digit =   parsed[0][2] if parsed[0][1] == 'digit' else parsed[0][3]
            last_digit =   parsed[-1][2] if parsed[-1][1] == 'digit' else parsed[-1][3]

            # Display the results
            print(f"First digit: {first_digit}")
            print(f"Last digit: {last_digit}")
            print(f"inted: ",int(first_digit + last_digit))
            sum = sum + int(first_digit + last_digit)
            print("\n")
    print("sum:", sum)

process_file('input.txt')