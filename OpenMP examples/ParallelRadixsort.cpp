#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>

void radixSort(std::vector<int> &arr) 
{
    int n = arr.size();
    if (n <= 1) return;

    int maxVal = *std::max_element(arr.begin(), arr.end());
    std::vector<int> output(n);

    // Process digits (base 10)
    for (int exp = 1; maxVal / exp > 0; exp *= 10) 
    {
        std::vector<int> count(10, 0);

        // Count occurrences of each digit
        #pragma omp parallel
        {
            std::vector<int> local_count(10, 0);
            #pragma omp for nowait
            for (int i = 0; i < n; i++) 
            {
                int digit = (arr[i] / exp) % 10;
                local_count[digit]++;
            }
            #pragma omp critical
            for (int d = 0; d < 10; d++) count[d] += local_count[d];
        }

        // Prefix sum (sequential, small array)
        for (int i = 1; i < 10; i++) count[i] += count[i - 1];

        // Build output array (stable, sequential to preserve order)
        for (int i = n - 1; i >= 0; i--) {
            int digit = (arr[i] / exp) % 10;
            output[--count[digit]] = arr[i];
        }

        // Copy back in parallel
        #pragma omp parallel for
        for (int i = 0; i < n; i++) arr[i] = output[i];
    }
}

int main() 
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> arr(n);
    for (int i = 0; i < n; i++) std::cin >> arr[i];

    radixSort(arr);

    for (int i = 0; i < n; i++) std::cout << arr[i] << " ";
    std::cout << "\n";
    return 0;
}
