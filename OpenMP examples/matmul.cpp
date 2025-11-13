#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <immintrin.h> // AVX intrinsics

constexpr int BLOCK_SIZE = 64;

// Access helper for 1D array representing 2D matrix
inline double &at(std::vector<double> &M, int n, int i, int j) {
    return M[i * n + j];
}
inline const double &at(const std::vector<double> &M, int n, int i, int j) {
    return M[i * n + j];
}

void matrixMultiply(const std::vector<double> &A, const std::vector<double> &B, std::vector<double> &C, int n) 
{
    std::fill(C.begin(), C.end(), 0.0);

    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (int ii = 0; ii < n; ii += BLOCK_SIZE) 
    {
        for (int jj = 0; jj < n; jj += BLOCK_SIZE) 
        {
            for (int kk = 0; kk < n; kk += BLOCK_SIZE) 
            {
                for (int i = ii; i < std::min(ii + BLOCK_SIZE, n); i++) 
                {
                    for (int k = kk; k < std::min(kk + BLOCK_SIZE, n); k++) 
                    {
                        __m256d r = _mm256_set1_pd(at(A, n, i, k)); // broadcast A[i][k]
                        int j = jj;
                        for (; j + 4 <= std::min(jj + BLOCK_SIZE, n); j += 4) 
                        {
                            __m256d b = _mm256_loadu_pd(&B[k * n + j]);   // load 4 doubles from B[k][j..j+3]
                            __m256d c = _mm256_loadu_pd(&C[i * n + j]);   // load 4 doubles from C[i][j..j+3]
                            c = _mm256_fmadd_pd(r, b, c);                 // c += r * b
                            _mm256_storeu_pd(&C[i * n + j], c);           // store back
                        }
                        // Handle remaining elements (if n not divisible by 4)
                        for (; j < std::min(jj + BLOCK_SIZE, n); j++) 
                        {
                            at(C, n, i, j) += at(A, n, i, k) * at(B, n, k, j);
                        }
                    }
                }
            }
        }
    }
}

int main() 
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n; // Matrix size (n x n)

    std::vector<double> A(n * n), B(n * n), C(n * n);

    // Input matrices
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            std::cin >> at(A, n, i, j);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            std::cin >> at(B, n, i, j);

    // Perform multiplication
    matrixMultiply(A, B, C, n);

    // result
    for (int i = 0; i < n; i++) 
    {
        for (int j = 0; j < n; j++) 
        {
            std::cout << at(C, n, i, j) << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
