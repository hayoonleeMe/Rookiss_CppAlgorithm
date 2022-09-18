#include "BinarySearchTree.h"
#include <iostream>
#include <Windows.h>
using namespace std;

enum class ConsoleColor
{
	BLACK = 0,
	RED = FOREGROUND_RED,
	GREEN = FOREGROUND_GREEN,
	BLUE = FOREGROUND_BLUE,
	YELLOW = RED | GREEN,
	WHITE = RED | GREEN | BLUE,
};

void SetCursorColor(ConsoleColor color)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	::SetConsoleTextAttribute(output, static_cast<SHORT>(color));
}

void SetCursorPosition(int x, int y)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	::SetConsoleCursorPosition(output, pos);
}

BinarySearchTree::BinarySearchTree()
{
	_nil = new Node(); // Black
	_root = _nil;
}

BinarySearchTree::~BinarySearchTree()
{
	delete _nil;
}

void BinarySearchTree::Print(Node* node, int x, int y)
{
	if (node == _nil)
		return;

	SetCursorPosition(x, y);

	if (node->color == Color::Black)
		SetCursorColor(ConsoleColor::BLUE);
	else
		SetCursorColor(ConsoleColor::RED);

	cout << node->key;
	Print(node->left, x - (5 / (y + 1)), y + 1);
	Print(node->right, x + (5 / (y + 1)), y + 1);

	SetCursorColor(ConsoleColor::WHITE);
}

Node* BinarySearchTree::Search(Node* node, int key)
{
	if (node == _nil || key == node->key)
		return node;

	if (key < node->key)
		return Search(node->left, key);
	else
		return Search(node->right, key);

}

Node* BinarySearchTree::Min(Node* node)
{
	while (node->left != _nil)
		node = node->left;

	return node;
}

Node* BinarySearchTree::Max(Node* node)
{
	while (node->right != _nil)
		node = node->right;

	return node;
}

// 현재 노드의 다음 값을 가지는 노드를 반환하는 메소드
Node* BinarySearchTree::Next(Node* node)
{
	// 노드의 오른쪽 노드가 있다면 오른쪽 노드에서 찾을 수 있는 최솟값이 현재 노드의 다음 값이 된다.
	if (node->right != _nil)
		return Min(node->right);

	// 노드의 오른쪽 노드가 없을 때
	// 현재 노드가 부모 노드의 왼쪽 노드라면 부모 노드가 현재 노드의 다음 값이 된다. (부모의 왼쪽노드는 항상 부모 노드보다 작으므로)
	// 현재 노드가 부모 노드의 오른쪽 노드라면 부모 노드가 현재 노드보다 큰 다음 값이 될것이다.
	// 만약 현재 노드보다 큰 다음 값이 없다면 계속 부모 노드로 거슬러 올라가다가 null을 만나 다음 값을 가진 노드를 찾지 못하고 nullptr를 반환할 것이다.
	Node* parent = node->parent;

	while (parent != _nil && node == parent->right)
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

	

	Node* node = _root;
	Node* parent = _nil;

	while (node != _nil)
	{
		parent = node;
		if (key < node->key)
			node = node->left;
		else
			node = node->right;
	}

	newNode->parent = parent;

	if (parent == _nil)
		_root = newNode;
	else if (key < parent->key)
		parent->left = newNode;
	else
		parent->right = newNode;

	// 검사
	newNode->left = _nil;
	newNode->right = _nil;
	newNode->color = Color::Red;

	InsertFixup(newNode);
}

void BinarySearchTree::InsertFixup(Node* node)
{
	// 1) p = red, uncle = red
	// -> p = black, uncle = black, pp = red로 바꿈
	// 2) p = red, uncle = black (triangle)
	// -> 회전을 통해 case 3으로 바꿈
	// 3) p = red, uncle = black (list)
	// -> 색상 변경 + 회전

	
	while (node->parent->color == Color::Red)
	{
		if (node->parent == node->parent->parent->left)
		{
			//      [pp(B)]
			//  [p(R)]    [u(R)]
			//      [n(R)]
			Node* uncle = node->parent->parent->right;
			if (uncle->color == Color::Red)
			{
				node->parent->color = Color::Black;	// p
				uncle->color = Color::Black; // u
				node->parent->parent->color = Color::Red; // pp
				node = node->parent->parent;
			}
			else
			{
				// Triangle
				//      [pp(B)]
				//  [p(R)]    [u(B)]
				//      [n(R)]	

				// List
				//         [pp(B)]
				//     [p(R)]    [u(B)]
				//  [n(R)]	

				// Triangle -> List
				if (node == node->parent->right) // Triangle 타입
				{
					node = node->parent;
					LeftRotate(node);
				}

				// List 타입
				//         [pp(R)]
				//     [p(B)]    [u(B)]
				//  [n(R)]	
				// 
				//         [p(B)]
				//     [n(R)]    [pp(R)]
				//					   [u(B)]	
				node->parent->color = Color::Black;
				node->parent->parent->color = Color::Red;
				RightRotate(node->parent->parent);
			}
		}
		else
		{
			//      [pp(R)]
			//  [u(B)]    [p(B)]
			//                [n(R)]
			Node* uncle = node->parent->parent->left;
			if (uncle->color == Color::Red)
			{
				node->parent->color = Color::Black;	// p
				uncle->color = Color::Black; // u
				node->parent->parent->color = Color::Red; // pp
				node = node->parent->parent;
			}
			else
			{
				// Triangle
				//      [pp(B)]
				//  [u(R)]    [p(R)]
				//          [n(R)]	

				//      [pp(B)]
				//  [u(R)]   [p(R)]
				//               [n(R)]	

				if (node == node->parent->left) // Triangle 타입
				{
					node = node->parent;
					RightRotate(node);
				}

				// List 타입
				//      [pp(B)]
				//  [u(R)]   [p(R)]
				//               [n(R)]	

				//            [p(B)]
				//      [pp(R)]    [n(R)]
				//  [u(B)]	
				node->parent->color = Color::Black;
				node->parent->parent->color = Color::Red;
				LeftRotate(node->parent->parent);
			}
		}
	}

	_root->color = Color::Black;
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

// (1)
//      [y]
//   [x]   [3]
// [1] [2]

// (2)
//      [x]
//   [1]   [y]
//		 [2]  [3]

// (2) -> (1)
void BinarySearchTree::LeftRotate(Node* x)
{
	Node* y = x->right;

	x->right = y->left; // [2]

	if (y->left != _nil)
		y->left->parent = x;

	y->parent = x->parent;

	if (x->parent == _nil)
		_root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->left = x;
	x->parent = y;
}

// (1) -> (2)
void BinarySearchTree::RightRotate(Node* y)
{
	Node* x = y->left;

	y->left = x->right; // [2];

	if (x->right != _nil)
		x->right->parent = y;

	x->parent = y->parent;

	if (y->parent == _nil)
		_root = x;
	else if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;

	x->right = y;
	y->parent = x;
}

