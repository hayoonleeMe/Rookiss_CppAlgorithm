#include <iostream>
#include <vector>
#include <list>
#include <stack>
using namespace std;

// Stack (LIFO Last-In-First-Out 후입선출)
// EX) 되돌리기 Ctrl+Z

template<typename T, typename Container = vector<T>>
class Stack
{
public:
	void push(const T& value)
	{
		_container.push_back(value);
	}

	void pop()
	{
		_container.pop_back();
	}

	T& top()
	{
		// back() : vector의 마지막 원소 레퍼런스를 반환함.
		return _container.back();
	}

	bool empty() { return _container.empty(); }
	int size() { return _container.size(); }

private:
	//vector<T> _container;
	//list<T> _container;
	Container _container;
};

int main()
{
	Stack<int, list<int>> s;

	// 삽입
	s.push(1);
	s.push(2);
	s.push(3);

	// 비었는지 체크
	while (s.empty() == false)
	{
		// 최상위 원소
		int data = s.top();
		// 최상위 원소 삭제(반환X)
		s.pop();

		cout << data << endl;
	}
	
	// 스택의 크기
	int size = s.size();
}