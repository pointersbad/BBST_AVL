#include <iostream>
#include <stdio.h>
#pragma warning(disable:4996)
using namespace std;
class Node {
public:
	Node() {}
	Node(const int& x) {
		data = x;
	}
	Node* left = NULL;
	Node* right = NULL;
	int data = 0;
	int height = 1;
	int value = 1;
	int descendants = 0;
};
class BBST {
public:
	int getHeight(const Node* node);
	void insert(Node*& node, const int& x);
	void erase(Node*& node, const int& x);
	void find(Node*& node, const int& x, int& count);
	void findxsmallest(Node*& node, const int& x, int& count);
	void precursor(Node*& node, const int& x, Node*& value);
	void successor(Node*& node, const int& x, Node*& value);
	void rotateL(Node*& node);
	void rotateR(Node*& node);
	void doubleRotateL(Node*& node);
	void doubleRotateR(Node*& node);
	int countRank(Node*& node);
	Node* root = NULL;
};

int BBST::getHeight(const Node* node) {
	return node == NULL ? 0 : node->height;
}

void BBST::rotateL(Node*& node) {
	Node* lson = node->left;
	node->descendants = countRank(node->right) + countRank(lson->right);
	lson->descendants = countRank(node) + countRank(lson->left);
	node->left = lson->right;
	lson->right = node;
	node->height = ((getHeight(node->left) >= getHeight(node->right)) ?
		getHeight(node->left) : getHeight(node->right)) + 1;
	lson->height = ((getHeight(lson->left) >= getHeight(lson->right)) ?
		getHeight(lson->left) : getHeight(lson->right)) + 1;
	node = lson;
}

void BBST::rotateR(Node*& node) {
	Node* rson = node->right;
	node->descendants = countRank(node->left) + countRank(rson->left);
	rson->descendants = countRank(node) + countRank(rson->right);
	node->right = rson->left;
	rson->left = node;
	node->height = ((getHeight(node->left) >= getHeight(node->right)) ?
		getHeight(node->left) : getHeight(node->right)) + 1;
	rson->height = ((getHeight(rson->left) >= getHeight(rson->right)) ?
		getHeight(rson->left) : getHeight(rson->right)) + 1;
	node = rson;
}

void BBST::doubleRotateL(Node*& node) {
	rotateR(node->left);
	rotateL(node);
}

void BBST::doubleRotateR(Node*& node) {
	rotateL(node->right);
	rotateR(node);
}

void BBST::insert(Node*& node, const int& x) {
	if (node == NULL) node = new Node(x);
	else if (x < node->data) {
		node->descendants++;
		insert(node->left, x);
		if (getHeight(node->left) == getHeight(node->right) + 2) {
			if (x < node->left->data) rotateL(node);
			else doubleRotateL(node);
		}
	}
	else if (x > node->data) {
		node->descendants++;
		insert(node->right, x);
		if (getHeight(node->right) == getHeight(node->left) + 2) {
			if (x > node->right->data) rotateR(node);
			else doubleRotateR(node);
		}
	}
	else node->value++;
	node->height = ((getHeight(node->left) >= getHeight(node->right)) ?
		getHeight(node->left) : getHeight(node->right)) + 1;
}

void BBST::erase(Node*& node, const int& x) {
	if (node != NULL) {
		if (x < node->data) erase(node->left, x);
		else if (x > node->data) erase(node->right, x);
		else {
			if (node->value == 1) {
				if (node->left == NULL && node->right == NULL)
					node = NULL;
				else if (node->left == NULL) {
					Node* temp = node->right;
					node->data = temp->data;
					node->value = temp->value;
					node->right = NULL;
					delete temp;
				}
				else if (node->right == NULL) {
					Node* temp = node->left;
					node->data = temp->data;
					node->value = temp->value;
					node->left = NULL;
					delete temp;
				}
				else {
					Node* temp = new Node();
					successor(node, node->data, temp);
					node->data = temp->data;
					node->value = temp->value;
					temp->value = 1;
					erase(node->right, temp->data);
				}
			}
			else node->value--;
		}
		if (node != NULL) {
			node->descendants = countRank(node->left) + countRank(node->right);
			node->height = ((getHeight(node->left) >= getHeight(node->right)) ?
				getHeight(node->left) : getHeight(node->right)) + 1;
			if (getHeight(node->left) == getHeight(node->right) + 2) {
				if (getHeight(node->left->left) >= getHeight(node->left->right)) rotateL(node);
				else doubleRotateL(node);
			}
			else if (getHeight(node->right) == getHeight(node->left) + 2) {
				if (getHeight(node->right->right) >= getHeight(node->right->left)) rotateR(node);
				else doubleRotateR(node);
			}
		}
	}
}

int BBST::countRank(Node*& node) {
	if (node != NULL) {
		return node->descendants + node->value;
	}
	return 0;
}

void BBST::find(Node*& node, const int& x, int& count) {
	if (node != NULL) {
		if (count > 0) {
			if (x > node->data) {
				count += (countRank(node->left) + node->value);
				find(node->right, x, count);
			}
			else if (x == node->data) {
				count += countRank(node->left);
				printf("%d\n", count);
				count = -1;
			}
			else {
				find(node->left, x, count);
			}
		}
	}
}

void BBST::findxsmallest(Node*& node, const int& x, int& count) {
	if (count >= 0) {
		if (node != NULL) {
			if (count + countRank(node->left) + 1 <= x &&
				count + countRank(node->left) + node->value >= x) {
				printf("%d\n", node->data);
				count = -1;
			}
			else if (count + countRank(node->left) + 1 < x) {
				count += (countRank(node->left) + node->value);
				findxsmallest(node->right, x, count);
			}
			else findxsmallest(node->left, x, count);
		}
	}
}

void BBST::precursor(Node*& node, const int& x, Node*& value) {
	if (node != NULL) {
		if (x > node->data) {
			value = node;
			precursor(node->right, x, value);
		}
		else precursor(node->left, x, value);
	}
}

void BBST::successor(Node*& node, const int& x, Node*& value) {
	if (node != NULL) {
		if (x < node->data) {
			value = node;
			successor(node->left, x, value);
		}
		else successor(node->right, x, value);
	}
}

int main() {
	BBST b;
	int n; scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		int opt, x; 
		scanf("%d%d", &opt, &x);
		if (opt == 1) {
			b.insert(b.root, x);
		}
		else if (opt == 2) {
			bool found = false;
			Node* deleteAt = new Node();
			int num = 1;
			b.erase(b.root, x);
		}
		else if (opt == 3) {
			int count = 1;
			b.find(b.root, x, count);
		}
		else if (opt == 4) {
			int count = 0;
			b.findxsmallest(b.root, x, count);
		}
		else if (opt == 5) {
			Node* value = NULL;
			b.precursor(b.root, x, value);
			if (value != NULL) {
				printf("%d\n", value->data);
			}
		}
		else if (opt == 6) {
			Node* value = NULL;
			b.successor(b.root, x, value);
			if (value != NULL) {
				printf("%d\n", value->data);
			}
		}
	}
	return 0;
}