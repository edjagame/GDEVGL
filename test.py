string = ""
with open("verticesNew.txt", "r") as file:
    index = 0
    for line in file:
        string += f"{line[:-1]} -> {str(index)}\n"
        index+=1

with open("verticesNew.txt", "w") as file:
    file.write(string)


# def read_numbers_from_file(filename):
#     numbers = []
#     with open(filename, 'r') as file:
#         for line in file:
#             line = line.strip()
#             if line and not line.startswith('/'):
#                 numbers.extend(map(int, line.rstrip(',').split(',')))
#     return numbers

# def get_unique_sorted_numbers(numbers):
#     unique_numbers = list(set(numbers))
#     unique_numbers.sort()
#     return unique_numbers

# def find_missing_integers(sorted_list):
#     if not sorted_list:
#         return []

#     min_val = sorted_list[0]
#     max_val = sorted_list[-1]
#     missing_integers = []

#     for num in range(min_val + 1, max_val):
#         if num not in sorted_list:
#             missing_integers.append(num)

#     return missing_integers

# def main():
#     filename = "indices.txt"
#     numbers = read_numbers_from_file(filename)
#     unique_sorted_numbers = get_unique_sorted_numbers(numbers)
#     print("Sorted list of unique numbers in the file:", unique_sorted_numbers)
#     missing_integers = find_missing_integers(unique_sorted_numbers)
#     print(missing_integers)
# if __name__ == "__main__":
#     main()

