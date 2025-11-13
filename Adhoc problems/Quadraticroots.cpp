#include <iostream>
#include <cmath>
#include <vector>

class Solution {
public:
    std::vector<int> quadraticRoots(int a, int b, int c) 
{
        std::vector<int> roots;
        // Calculate discriminant
        long long d = (long long)b * b - 4LL * a * c;
        
        if (d < 0) 
        {
            roots.push_back(-1);
        } 
        else 
        {
            double sqrtD = std::sqrt(d);
            int root1 = std::floor((-b + sqrtD) / (2.0 * a));
            int root2 = std::floor((-b - sqrtD) / (2.0 * a));
            
            // Push larger root first, then smaller
            roots.push_back(std::max(root1, root2));
            roots.push_back(std::min(root1, root2));  
        }
        
        return roots;
    }
};

int main() 
{
    int t;
    std::cin >> t;
    while (t--) {
        int a, b, c;
        std::cin >> a >> b >> c;
        Solution ob;
        std::vector<int> roots = ob.quadraticRoots(a, b, c);
        
        if (roots.size() == 1 && roots[0] == -1)
            std::cout << "Imaginary";
        else
            for (int root : roots)
                std::cout << root << " ";
        std::cout << std::endl;
    }
    return 0;
}
