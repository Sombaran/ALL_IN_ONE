

#include <bits/stdc++.h>

template<typename T>

struct TreeNode {
    T mVal;
    TreeNode* mLeft;
    TreeNode* mRight;
    TreeNode(T x) 
    : mVal(x)
    , mLeft(nullptr)
    , mRight(nullptr) { }

    TreeNode* insertIntoBST(TreeNode* root, T& val) {
        if (root == nullptr) return new TreeNode(val);
        if (val < root->mVal) {
            root->mLeft = insertIntoBST(root->mLeft, val);
        } else {
            root->mRight = insertIntoBST(root->mRight, val);
        }
        return root;
    }

     // Preorder that takes a pointer
    void printPreorder(TreeNode* node) {
        if (node == nullptr) return;

        // first print data of node
        std::cout << node->mVal << " ";

        // then recur on left subtree
        printPreorder(node->mLeft);

        // now recur on right subtree
        printPreorder(node->mRight);
    }
};






int main(int argc, char** argv)
{
    TreeNode<int>* root = nullptr;
    size_t n{};
    std::cout << "Enter number of nodes: ";
    std::cin >> n;

    for (size_t i = 0; i < n; ++i) {
        int val{};
        std::cout << "Enter value of node " << i + 1 << ": ";
        std::cin >> val;
        root = root -> insertIntoBST(root, val);
    }

    root -> printPreorder(root);
    std::cout << "\n";
    return (0);
}