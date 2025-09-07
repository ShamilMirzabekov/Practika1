#include <iostream>
#include<fstream>
#include <vector>
using namespace std;

//Класс узла
class Node {
public:
    double key;
	double priority;
	int size;
	Node* left;
	Node* right;
	Node* parent;
	//Конструктор
	Node(double k, double p) : key(k), priority(p), size(1), left(nullptr), right(nullptr), parent(nullptr) {};

	friend ostream& operator<<(ostream& stream, Node& N)
	{
		stream << N.key << "; " << N.priority << "\n";
		if (N.left != NULL)
			stream << *N.left;
		if (N.right != NULL)
			stream << *N.right;
		return stream;
	}
};
//Класс курева 4.1
class Treap {
protected:
	Node* root;
	//Получить размер
	int getSize(Node* node) {
		if (node == nullptr) {
			return 0;
		}
		return node->size;
	}
	//Обновить поле размера
	void updateSize(Node* someNode) {
		if (someNode != nullptr)
			someNode->size = getSize(someNode->left) + getSize(someNode->right) + 1;
	}
	//Объединение деревьев
	Node* merge(Node* L, Node* R) {
		if (!L || !R)
			return L ? L : R;

		if (L->priority > R->priority) {
			L->right = merge(L->right, R);
			if (L->right != nullptr) 
				L->right->parent = L;
			updateSize(L);
            return L;
		}
		else {
			R->left = merge(L, R->left);
			if (R->left!= nullptr)
				R->left->parent = R;
			updateSize(R);
			return R;
		}
	}
	//Разрезание деревьев по ключу
	void split(Node* root, double key, Node*& L, Node*& R) {
		if (root == nullptr)
		{
			L = nullptr;
			R = nullptr;
			return;
		}
		else if (root->key <= key) {
			split(root->right, key, root->right, R);
			L = root;
			if (L->right)
				root->right->parent = root;
		}
		else {
			split(root->left, key, L, root->left);
			R = root;
			if (root->left)
				root->left->parent = root;
		}
		updateSize(root);
	}

public:
	//Геттер
	Node* GetNRoot() { return root; }
	//Конструкторы
	//Treap() { root = NULL; }
	Treap() : root(nullptr) {};
	//Деструктор
	~Treap() {}

	void insert(double key, double priority) {
		Node* someNode = new Node(key, priority);
		Node* left;
		Node* right;
		split(root, key, left, right);
		root = merge(merge(left, someNode), right);
		if (root)
			root->parent = nullptr;
	}

	Node* find(double key) {
		Node* current = root;
		while (current) {
			if (current->key == key)
				return current;
			else if (current->key < key)
				current = current->right;
			else
				current = current->left;
		}
		return current;
	}

	void remove(double key) {
		Node* someNode = find(key);
		if (!someNode)
			return;
		if (someNode->left)
			someNode->left->parent = nullptr;
		if (someNode->right)
			someNode->right->parent = nullptr;

		root = merge(someNode->left, someNode->right);
		delete someNode;
	}

	Node* successor(Node* someNode) {
		if (someNode->right) {
			Node* current = someNode->right;
			while (current->left)
				current = current->left;
			return current;
		}
		else {
			Node* current = someNode->parent;
			while (current && someNode == current->right) {
				someNode = current;
				current = current->parent;
			}
			return current;
		}
	}

	Node* min() {
		Node* current = root;
		while (current && current->left)
			current = current->left;
		return current;
	}

	Node* max() {
		Node* current = root;
		while (current && current->right)
			current = current->right;
		return current;
	}

	void inorderPrnt(Node* someNode) {
		if (someNode != nullptr) {
			inorderPrnt(someNode->left);
			cout << someNode->key << " ";
			inorderPrnt(someNode->right);
		}
	}

	void printInOrder() {
		inorderPrnt(root);
	}

	friend ostream& operator<<(ostream& stream, Treap& T)
	{
		if (T.root != NULL)
			stream << *T.root;
		return stream;
	}

};
//Класс итератора
class Iterator:public Treap {
private:
	Node* current;
	Treap* tree;
public:
	Iterator(Node* someNode, Treap* t) :current(someNode), tree(t) {}

	double operator*() {
		return current->key;
	}
	Iterator operator++(int) {
		Iterator temp = *this;
		++(*this);
		return temp;
	}
	Iterator& operator++() {
		current = tree->successor(current);
		return *this;
	}

	bool operator!=(const Iterator& other) {
		return current != other.current;
	}

};
//Класс курева с неявным ключом
class KeyTreap: public Treap {
private:
	int size(Node* root) {
		return Treap::getSize(root);
	}

	void updateSize(Node* root) {
		Treap::updateSize(root);
	}

	void splitic(Node* root, int index, Node*& L, Node*& R) {
		if (root == nullptr) {
			L = nullptr;
			R = nullptr;
		}
		else {
			int rootIndex = getSize(root->left) + 1;
			if (rootIndex <= index) {
				splitic(root->right, index - rootIndex, root->right, R);
				L = root;
			}
			else {
				splitic(root->left, index, L, root->left);
				R = root;
			}
			updateSize(root);
		}
	}

public:
	KeyTreap() :Treap() {}

	void split(int index, KeyTreap& L, KeyTreap& R) {
		Node* LNode = nullptr;
		Node* RNode = nullptr;
		splitic(root, index, LNode, RNode);
		L.root = LNode;
		R.root = RNode;
	}

};

int main() {
	//Задание 4.1
	cout << "Treap:"<<"\n";
	Treap tree;
	tree.insert(5, rand());
	tree.insert(6, rand());
	tree.insert(8, rand());
	tree.insert(1, rand());
	tree.insert(2, rand());

	Node* mini = tree.min();
	Node* maxi = tree.max();

	cout << mini->key<<"\n";
	cout << maxi->key << "\n";

	Iterator it(mini, &tree);
	//Вывод по возрастанию
	while (it != Iterator(nullptr, &tree))
	{
		cout << *it << " ";
		++it;
	}
	//Задание 4.2
	cout <<"\n"<< "KeyTreap:";
	//Добавление отдельно чисел
	KeyTreap keytree1;

	keytree1.insert(2.1, 5);
	keytree1.insert(3.7, 2);
	keytree1.insert(5.4, 1);
	keytree1.insert(8.2, 7);
	keytree1.insert(6.9, 6);

	cout <<"\n"<< "Inorder Traversal 1: ";
	keytree1.printInOrder();

	KeyTreap left, right;
	keytree1.split(3, left, right);

	cout<<"\n"<<"Left subtree 1: ";
	left.printInOrder();

	cout << "Right subtree 1: ";
	right.printInOrder();

    KeyTreap keytree2;
	//Долбавление массива(Вектора)
	vector<double> num = { 1.4, 2.8, 3.2, 4.9, 5.6 };
	
	for (double i = 0; i < num.size(); i++) {
		keytree2.insert(num[i], i + 1);
	}
	
	cout << "\n" << "Inorder Traversal 2: ";
	keytree2.printInOrder();

	KeyTreap left1, right1;
	keytree2.split(2, left1, right1);

	cout << "\n" << "Left subtree 2: ";
	left1.printInOrder();

	cout << "Right subtree 2: ";
	right1.printInOrder();

	return 0;
}
