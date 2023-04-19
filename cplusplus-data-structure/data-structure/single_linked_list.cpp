//============================================================================
// Name        : single_linked_list.cpp
// Author      : Jotirling Swami
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

class Node {
public:
	int value;
	Node *next;
	Node(int Val) {
		value = Val;
		next = NULL;
	}
};

class LinkedList {
private:
	Node *head;
public:
	LinkedList() {
		head = NULL;
	}

	void addFront(int val) {
		Node *newNode = new Node(val);

		if (head == NULL) {
			head = newNode;
		} else {
			newNode->next = head;
			head = newNode;
		}
	}

	void display() {
		Node *temp = head;
		if (temp == NULL) {
			cout << "Linked List Empty" << endl;
		} else {

			cout << "HEAD-->";
			while (temp != NULL) {
				cout << temp->value << "-->";
				temp = temp->next;
			}
			cout << "NULL" << endl;
		}
	}

	void addBack(int val) {
		Node *newNode = new Node(val);
		if (head == NULL) {
			head = newNode;
	
		} else {
			Node *temp = head;
			while (temp->next != NULL) {
				temp = temp->next;
			}
			temp->next = newNode;

		}
	}

	int posCheck() {
		int posCheck;
		Node *temp = head;
		while (temp != NULL) {
			++posCheck;
			temp = temp->next;
		}
		return posCheck;
	}

	void addAtPos(int val, int pos) {
		if (pos > posCheck()) {
			cout << "Invalid Pos" << endl;
		} else if (pos == 0) {
			Node *newNode = new Node(val);
			head = newNode;
		} else {
			Node *temp = head;
			Node *newNode = new Node(val);
			while (pos > 1) {
				temp = temp->next;
				--pos;
			}
			newNode->next = temp->next;
			temp->next = newNode;
		}
	}

	void reverseList() {
		Node *current = head;
		Node *Prev = NULL, *Next = NULL;

		while (current != NULL) {
			// Store next
			Next = current->next;

			// Reverse current node's pointer
			current->next = Prev;

			// Move pointers one position ahead.
			Prev = current;
			current = Next;
		}
		head = Prev;
	}

	void addMid(int val) {
		int count = 0;
		Node *newNode = new Node(val);
		if (head == NULL) {
			head = newNode;
		} else {
			Node *middle = head;
			Node *temp = head;
			while (temp != NULL) {
				if (count & 1)
					middle = middle->next;
				++count;
				temp = temp->next;
			}
			cout << "mid: " << middle->value << endl;
		}

	}
	void makeLoop(int pos) {
		Node *temp = head;
		Node *tempEnd = head;
		while (tempEnd->next != NULL) {
			tempEnd = tempEnd->next;
		}
		while (pos > 1) {
			temp = temp->next;
			--pos;
		}
		cout << tempEnd->value << " is connected to " << temp->value << endl;
		tempEnd->next = temp;

	}

	void detectLoop() {
		Node *fast = head;
		Node *slow = head;
		while (slow != NULL && fast != NULL && fast->next != NULL) {
			slow = slow->next;
			fast = fast->next->next;

			if (slow == fast) {
				cout << "Found Loop" << endl;
				return;
			}
		}
	}

	void deleteFront() {
		if (head == NULL) {
			cout << "Linked List Empty" << endl;
		} else if (head->next == NULL) {
			cout << "Delete: " << head->value << endl;
			delete (head);
			head = NULL;
		} else {
			Node *temp = head;
			cout << "Delete: " << temp->value << endl;
			head = head->next;
			delete (temp);
			temp = NULL;
		}
	}

	void deleteEnd() {

		if (head == NULL) {
			cout << "Linked List Empty" << endl;
		} else if (head->next == NULL) {
			cout << "Delete: " << head->value << endl;
			delete (head);
			head = NULL;
		} else {
			Node *temp = head;
			while (temp->next->next != NULL) {
				temp = temp->next;
			}
			cout << "Delete: " << temp->next->value << endl;
			delete (temp->next);
			temp->next = NULL;
		}
	}
};

int main() {
	LinkedList l;
	l.addFront(10);
	l.addFront(20);
	l.addFront(30);
	l.addFront(40);
	l.addFront(50);
	l.addBack(15);
	l.addBack(20);
	l.addBack(30);
	l.addBack(40);
	l.addBack(50);
	l.addBack(60);
	l.addBack(70);
	l.addAtPos(500, 3);
	l.addMid(300);
	l.display();
//    l.makeLoop(3);
	l.detectLoop();
	l.deleteFront();
	l.display();
	l.deleteEnd();
	l.display();
	l.reverseList();
	l.display();

	return 0;
}

