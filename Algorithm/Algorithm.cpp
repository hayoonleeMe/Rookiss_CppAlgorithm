#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <thread>
#include <windows.h>
using namespace std;

// 오늘의 주제 : 동적 계획법 (DP)
// LIS (Longest Increasing Sequence)

// Seq : 1 9 2 5 7
// 부분 수열 : 일부(0개 이상) 숫자를 지우고 남은 수열
// ex) 1 2 5
// ex) 1 9 5 7
// 순 증가 부분 수열 : 숫자가 계속 증가하는 형태의 부분 수열
// ex) 1 2 5

// LIS : 제일 긴 [순 증가 부분 수열]의 길이
// 1 2 5 7 = 길이 4

int cache[100];
vector<int> seq;

int LIS(int pos)
{
	// 기저 사항

	// 캐시 확인
	int& ret = cache[pos];
	if (ret != -1)
		return ret;

	// Seq : 1 9 2 5 7
	// LIS(pos)가 중복되므로 캐싱이 유효함
	// 1 9 = 2		LIS(0) -> 1 + LIS(1)
	// 1 2 5 7 = 4	LIS(0) -> 1 + LIS(2) -> 1 + LIS(3) -> 1 + LIS(4)
	// 1 5 7 = 3	LIS(0) -> 1 + LIS(3) -> 1 + LIS(4)
	// 1 7 = 2		LIS(0) -> 1 + LIS(4)

	// LIS(0) = LIS(1) + 1
	// LIS(1) = 1

	// 최소 seq[pos]는 있으니 1부터 시작
	ret = 1;

	// 구하기
	for (int next = pos + 1; next < seq.size(); ++next)
		if (seq[pos] < seq[next])
			// ret는 레퍼런스 변수 이므로 seq[pos]에 업데이트도 함께 함
			ret = max(ret, 1 + LIS(next));

	return ret;
}

int main()
{
	::memset(cache, -1, sizeof(cache));

	seq = vector<int>{ 1,9,2,5,7 };

	int ret = 0;
	for (int pos = 0; pos < seq.size(); ++pos)
		ret = max(ret, LIS(pos));
}