#include <cstring>
#include <ctime>
#include <iostream>
#include "./large_number.h"

bool test1() {
    unsigned int number = 10;
    large_number result;
    for (unsigned int i = 2; i <= number; i++)
        result *= i;

    const char* known_reslt = "3628800";
    if (strlen(known_reslt) != result.num_digits())
        return false;

    for (size_t i = 0; i < result.num_digits(); i++)
        if (known_reslt[i] != result.digit_char(i))
            return false;

    return true;
}

bool test2() {
    unsigned int number = 100;
    large_number result;
    for (unsigned int i = 2; i <= number; i++)
        result *= i;

    const char* known_reslt =
        "9332621544394415268169923885626670049071596826438162146859296389521759"
        "9993229915608941463976156518286253697920827223758251185210916864000000"
        "000000000000000000";

    if (strlen(known_reslt) != result.num_digits())
        return false;

    for (size_t i = 0; i < result.num_digits(); i++)
        if (known_reslt[i] != result.digit_char(i))
            return false;

    return true;
}

int main(int argc, char* argv[]) {
    int number;
    if (argc == 2)
        number = atoi(argv[1]);
    else
        std::cin >> number;

    large_number result;
    std::clock_t start_time = std::clock();
    for (int i = 2; i <= number; i++)
        result *= i;
    double time_taken = static_cast<double>(std::clock() - start_time) / CLOCKS_PER_SEC;

    std::cout << number << "! = " << result << "\n"
              << "Number of digits: " << result.num_digits() << "\n"
              << "Time taken: " << std::scientific << time_taken << " s\n";

    std::cout << (test1() ? "Test1 Passed\n" : "Test1 Failed\n");
    std::cout << (test2() ? "Test2 Passed\n" : "Test2 Failed\n");
    result.test();

    return 0;
}
