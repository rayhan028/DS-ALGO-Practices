#include <cassert>
#include <cstdint>
#include <vector>

class MemorisedFactorial 
{
    std::vector<std::uint64_t> known_values = {1};
    
    void compute_up_to(std::uint64_t n) {
        std::size_t current_size = known_values.size();
        if (n >= current_size) {
            known_values.reserve(n + 1);
            for (std::uint64_t i = current_size; i <= n; ++i) {
                known_values.push_back(known_values.back() * i);
            }
        }
    }
    
public:
    std::uint64_t operator()(std::uint64_t n) 
{
        compute_up_to(n);
        return known_values[n];
    }
};

void test_MemorisedFactorial_in_order() 
{
    auto factorial = MemorisedFactorial();
    assert(factorial(0) == 1);
    assert(factorial(1) == 1);
    assert(factorial(5) == 120);
    assert(factorial(10) == 3628800);
}

void test_MemorisedFactorial_no_order() 
{
    auto factorial = MemorisedFactorial();
    assert(factorial(10) == 3628800);
}

int main() 
{
    test_MemorisedFactorial_in_order();
    test_MemorisedFactorial_no_order();
    return 0;
}
