
#include <bits/stdc++.h>
using std::cout;
using std::endl;
struct Node {
    int mData;
    Node* mLeft;
    Node* mRight;

    Node(int data)
    : mData(data)
	, mLeft(nullptr)
	, mRight(nullptr) {
        cout << __func__ << endl;
    }
	
	virtual ~Node(){
		std::cout << __func__ << std::endl;
	}
	/**
	* Pre ==> root left right
	* In  ==> left root right
	* Pos ==> left right root
	*/
    // Preorder that takes a pointer
    void printPreorder(Node* node) {
        if (node == nullptr) return;

        // first print data of node
        cout << node->mData << " ";

        // then recur on left subtree
        printPreorder(node->mLeft);

        // now recur on right subtree
        printPreorder(node->mRight);
    }
	
	// Inorder that takes a pointer
    void printInorder(Node* node) {
        if (node == nullptr) return;

        // then recur on left subtree
        printInorder(node->mLeft);
		
        // first print data of node
        cout << node->mData << " ";

        // now recur on right subtree
        printInorder(node->mRight);
    }
	
	// Postorder that takes a pointer
    void printPostorder(Node* node) {
        if (node == nullptr) return;

        // then recur on left subtree
        printPostorder(node->mLeft);

        // now recur on right subtree
        printPostorder(node->mRight);
		
		// first print data of node
        cout << node->mData << " ";
    }
	
	void invertTree(Node* node) {
		std::cout << __func__ << std::endl;
		if (node == nullptr) return;
		
		invertTree(node->mLeft);
		invertTree(node->mRight);
		std::swap(node->mLeft, node->mRight);
	}
	
	uint vertexCount(Node* node) {
		std::cout << __func__ << std::endl;
		if (node == nullptr) return 0;
		
		uint leftSubCount = vertexCount(node->mLeft);
		uint rightSubCount = vertexCount(node->mRight);
		
		return (leftSubCount+rightSubCount+1);
	}
};

int main() {
    Node root(22);
    root.mLeft  = new Node(11);
    root.mRight = new Node(33);

    root.printPreorder(&root);   // pass a pointer
    std::cout << "\n";
	
	root.printInorder(&root);
	std::cout << "\n";
	
	root.printPostorder(&root);
	std::cout << "\n";
	
	root.invertTree(&root);
	
	root.printInorder(&root);
	std::cout << "\n";
	
	std::cout << root.vertexCount(&root) << std::endl;
    delete root.mLeft;
    delete root.mRight;
    return 0;
}