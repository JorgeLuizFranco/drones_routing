import random

def generate_test_case(n, m, k):
    test_case = f"{n} {m} {k}\n"

    for _ in range(k):
        i_begin = random.randint(0, n - 1)
        j_begin = random.randint(0, m - 1)
        i_end = random.randint(0, n - 1)
        j_end = random.randint(0, m - 1)

        test_case += f"{i_begin} {j_begin} {i_end} {j_end}\n"

    return test_case

def save_test_case(file_path, test_case):
    with open(file_path, 'w') as file:
        file.write(test_case)

def main():
    for test_number in range(4, 11):
        n = random.randint(5, 15)  # adjust the grid dimensions range
        m = random.randint(5, 15)
        k = random.randint(1, 5)    # adjust the number of drones range

        test_case = generate_test_case(n, m, k)
        file_path = f"{test_number}.in"
        save_test_case(file_path, test_case)


main()
