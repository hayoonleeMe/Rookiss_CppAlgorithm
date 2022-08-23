#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

using NodeRef = shared_ptr<struct Node>;



template<typename T>
class PriorityQueue
{
public:
	PriorityQueue()
	{
		_pq.reserve(100);
	}

	void push(const T& data)
	{
		_pq.push_back(data);

		if (_pq.size() == 1)
			return;

		int idx = _pq.size() - 1;
		int parent = -1;

		while (idx != 0)
		{
			int changeIdx = -1;

			// 왼쪽
			if (idx % 2 == 1)
			{
				parent = idx / 2;

				if (_pq[idx] > _pq[parent])
					changeIdx = idx;
			}
			// 오른쪽
			else
			{
				parent = idx / 2 - 1;

				int large = idx;
				if (_pq[idx] < _pq[idx - 1])
					large = idx - 1;

				if (_pq[large] > _pq[parent])
					changeIdx = large;
			}

			if (changeIdx == -1)
				break;

			// 부모와 변경
			int temp = _pq[parent];
			_pq[parent] = _pq[changeIdx];
			_pq[changeIdx] = temp;

			idx = parent;
		}
	}

	void pop()
	{
		if (_pq.size() == 1)
		{
			_pq.pop_back();
			return;
		}

		_pq[0] = _pq[_pq.size() - 1];
		_pq.resize(_pq.size() - 1);

		if (_pq.size() == 1)
			return;

		int idx = 0;

		while (idx != _pq.size() - 1)
		{
			int ChangeIndex = -1;
			int size = _pq.size();
			int left = idx * 2 + 1;
			int right = idx * 2 + 2;

			// 왼, 오 존재
			if (right < size)
			{
				int large = right;
				if (_pq[right] < _pq[left])
					large = left;

				if (_pq[idx] < _pq[large])
					ChangeIndex = large;
			}
			// 왼쪽만 존재
			else if (left < size)
			{
				if (_pq[idx] < _pq[left])
					ChangeIndex = left;
			}

			if (ChangeIndex == -1)
				break;;

			int temp = _pq[ChangeIndex];
			_pq[ChangeIndex] = _pq[idx];
			_pq[idx] = temp;

			idx = ChangeIndex;
		}
	}

	T& top()
	{
		return _pq[0];
	}

	int size() { return _pq.size(); }

	bool empty()
	{
		return _pq.empty();
	}

private:
	vector<T> _pq;
};

int main()
{
	PriorityQueue<int> pq;
	pq.push(100);
	pq.push(300);
	pq.push(200);
	pq.push(500);
	pq.push(400);

	while (pq.empty() == false)
	{
		int value = pq.top();
		pq.pop();

		cout << value << endl;	
	}
}
