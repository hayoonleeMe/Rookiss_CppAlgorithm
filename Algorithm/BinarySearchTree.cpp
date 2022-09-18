#include "BinarySearchTree.h"
#include <iostream>
#include <Windows.h>
using namespace std;

void SetCursorPosition(int x, int y)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	::SetConsoleCursorPosition(output, pos);
}

void BinarySearchTree::Print(Node* node, int x, int y)
{
	if (node == nullptr)
		return;

	SetCursorPosition(x, y);
	cout << node->key;
	Print(node->left, x - (5 / (y + 1)), y + 1);
	Print(node->right, x + (5 / (y + 1)), y + 1);
}

void BinarySearchTree::Print_Inorder(Node* node)
{
	// 전위 순회 (preorder traverse)
	// 중위 순회 (inorder)
	// 후위 순회 (postorder)

	//   [중]
	// [좌][우]

	// 전위 : [중]이 앞에 온다.		[중][좌][우]
	// 중위 : [중]이 중간에 온다.		[좌][중][우]
	// 후위 : [중이] 마지막에 온다.	[좌][우][중]

	if (node == nullptr)
		return;

	cout << node->key << endl;	// [중]
	Print_Inorder(node->left);	// [좌]
	Print_Inorder(node->right);	// [우]
}

Node* BinarySearchTree::Search(Node* node, int key)
{
	if (node == nullptr || key == node->key)
		return node;

	if (key < node->key)
		return Search(node->left, key);
	else
		return Search(node->right, key);

}

Node* BinarySearchTree::Search2(Node* node, int key)
{
	while (node && key != node->key)
	{
		if (key < node->key)
			node = node->left;
		else
			node = node->right;
	}

	return node;
}

Node* BinarySearchTree::Min(Node* node)
{
	while (node->left)
		node = node->left;

	return node;
}

Node* BinarySearchTree::Max(Node* node)
{
	while (node->right)
		node = node->right;

	return node;
}

// 현재 노드의 다음 값을 가지는 노드를 반환하는 메소드
Node* BinarySearchTree::Next(Node* node)
{
	// 노드의 오른쪽 노드가 있다면 오른쪽 노드에서 찾을 수 있는 최솟값이 현재 노드의 다음 값이 된다.
	if (node->right)
		return Min(node->right);

	// 노드의 오른쪽 노드가 없을 때
	// 현재 노드가 부모 노드의 왼쪽 노드라면 부모 노드가 현재 노드의 다음 값이 된다. (부모의 왼쪽노드는 항상 부모 노드보다 작으므로)
	// 현재 노드가 부모 노드의 오른쪽 노드라면 부모 노드가 현재 노드보다 큰 다음 값이 될것이다.
	// 만약 현재 노드보다 큰 다음 값이 없다면 계속 부모 노드로 거슬러 올라가다가 null을 만나 다음 값을 가진 노드를 찾지 못하고 nullptr를 반환할 것이다.
	Node* parent = node->parent;

	while (parent && node == parent->right)
	{
		node = parent;
		parent = parent->parent;
	}

	return parent;
}

void BinarySearchTree::Insert(int key)
{
	Node* newNode = new Node();
	newNode->key = key;

	if (_root == nullptr)
	{
		_root = newNode;
		return;
	}

	Node* node = _root;
	Node* parent = nullptr;

	while (node)
	{
		parent = node;
		if (key < node->key)
			node = node->left;
		else
			node = node->right;
	}

	newNode->parent = parent;

	if (key < parent->key)
		parent->left = newNode;
	else
		parent->right = newNode;
}

void BinarySearchTree::Delete(int key)
{
	Node* deleteNode = Search(_root, key);
	Delete(deleteNode);
}

void BinarySearchTree::Delete(Node* node)
{
	if (node == nullptr)
		return;

	if (node->left == nullptr)
		Replace(node, node->right);
	else if (node->right == nullptr)
		Replace(node, node->left);
	else
	{
		// 다음 데이터 찾기
		Node* next = Next(node);
		node->key = next->key;
		Delete(next);
	}
}

// u 서브트리를 v 서브트리로 교체 
// 그리고 delete u
void BinarySearchTree::Replace(Node* u, Node* v)
{
	if (u->parent == nullptr)
		_root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;

	if (v)
		v->parent = u->parent;

	delete u;
}

