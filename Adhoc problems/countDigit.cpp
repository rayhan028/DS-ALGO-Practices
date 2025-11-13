#include <iostream>

int countDigit(long long n) 
{
    if (n == 0) return 1; 
    if (n < 0) n = -n; // negatives
    int count = 0;
    while (n > 0) 
    {
        n /= 10;
        count++;
    }
    return count;
}

int main() 
{
    long long n;
    std::cout << "Enter number to count digits: ";
    std::cin >> n;

    std::cout << "Number of digits: " << countDigit(n) << "\n";
    return 0;
}
