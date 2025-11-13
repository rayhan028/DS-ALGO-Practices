#include <iostream>
#include <vector>
#include <omp.h>
#include <algorithm>

int partition(std::vector<int> &arr, int low, int high) 
{
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) 
    {
        if (arr[j] < pivot) 
        {
            std::swap(arr[++i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(std::vector<int> &arr, int low, int high, int depth = 0) 
{
    if (low < high) 
    {
        int pi = partition(arr, low, high);

        if (high - low > 1000) { // cutoff for parallelism
            #pragma omp task shared(arr) if(depth < 4)
            quickSort(arr, low, pi - 1, depth + 1);

            #pragma omp task shared(arr) if(depth < 4)
            quickSort(arr, pi + 1, high, depth + 1);
        } 
        else 
        {
            quickSort(arr, low, pi - 1, depth + 1);
            quickSort(arr, pi + 1, high, depth + 1);
        }
    }
}
