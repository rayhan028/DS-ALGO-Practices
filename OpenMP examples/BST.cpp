#include <iostream>
#include <vector>
#include <omp.h>

struct Node {
    int key;
    Node *left, *right;
    Node(int val) : key(val), left(nullptr), right(nullptr) {}
};

// Sequential insert
Node* insert(Node* root, int key) 
{
    if (!root) return new Node(key);
    if (key < root->key) root->left = insert(root->left, key);
    else root->right = insert(root->right, key);
    return root;
}

// Parallel bulk insertion
Node* buildBST(const std::vector<int> &arr) 
{
    Node* root = nullptr;
    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int val : arr) {
                #pragma omp task shared(root)
                {
                    #pragma omp critical
                    root = insert(root, val);
                }
            }
        }
    }
    return root;
}

// Parallel in-order traversal
void inorder(Node* root) 
{
    if (!root) return;
    #pragma omp task shared(root)
    inorder(root->left);

    #pragma omp task shared(root)
    inorder(root->right);

    #pragma omp taskwait
    std::cout << root->key << " ";
}

// Parallel search (multiple queries)
bool search(Node* root, int key) 
{
    if (!root) return false;
    if (root->key == key) return true;
    if (key < root->key) return search(root->left, key);
    return search(root->right, key);
}

int main() 
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> arr(n);
    for (int i = 0; i < n; i++) std::cin >> arr[i];

    Node* root = buildBST(arr);

    std::cout << "Inorder traversal (parallel): ";
    #pragma omp parallel
    {
        #pragma omp single
        inorder(root);
    }
    std::cout << "\n";

    int q;
    std::cin >> q;
    std::cout << "Search " << q << ": " << (search(root, q) ? "Found" : "Not Found") << "\n";

    return 0;
}
