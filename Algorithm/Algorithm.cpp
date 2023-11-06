#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <thread>
#include <windows.h>
using namespace std;

// 오늘의 주제 : 동적 계획법 (DP)
// TIC-TAE-TOE
// [.][.][.]
// [.][o][x]
// [.][.][o]

// [.][.][.][.][o][012][012][012][012] => 0 ~ 3^9=19683 (정수로 치환가능)
//				   27	 9	  3   0    (3진법 변환 처럼)
int HashKey(const vector<vector<char>>& board)
{
	int ret = 0;

	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			ret *= 3;
			if (board[y][x] == 'o')
				ret += 1;
			else if (board[y][x] == 'x')
				ret += 2;
		}
	}
	return ret;
}


bool IsFinished(const vector<vector<char>>& board, char turn)
{
	// 좌우
	for (int i = 0; i < 3; ++i)
		if (board[i][0] == turn && board[i][1] == turn && board[i][2] == turn)
			return true;

	// 상하
	for (int i = 0; i < 3; ++i)
		if (board[0][i] == turn && board[1][i] == turn && board[2][i] == turn)
			return true;

	// 대각선
	if (board[0][0] == turn && board[1][1] == turn && board[2][2] == turn)
		return true;

	if (board[0][2] == turn && board[1][1] == turn && board[2][0] == turn)
		return true;

	return false;
}

vector<vector<char>> board;
// key: 보드 상태에 따른 HashKey, 즉 0~19682까지의 key는 가능한 모든 보드 상태
// cache[key] key 보드 상태에 따른 경기 결과 캐싱
int cache[19683];

enum
{
	DEFAULT = 2,
	WIN = 1,
	DRAW = 0,
	LOSE = -1
};

int CanWin(vector<vector<char>>& board, char turn)
{
	// 기저 사례
	if (IsFinished(board, 'o' + 'x' - turn))
		return LOSE;

	// 캐시 확인
	int key = HashKey(board);
	int& ret = cache[key];
	// key 보드 상태에 따른 경기 결과가 이미 저장됐다면 반환
	if (ret != DEFAULT)
		return ret;

	// 풀기
	int minValue = DEFAULT;
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			if (board[y][x] != '.')
				continue;

			// 착수
			board[y][x] = turn;

			// 확인
			// 상대방이 패배하는게 제일 좋은 케이스
			// 상대방 턴일 때의 CanWin과 비교해 제일 작은 값이므로 LOSE인 -1이 나와야 제일 좋은 케이스
			minValue = min(minValue, CanWin(board, 'o' + 'x' - turn)); 

			// 취소
			// board 원본을 바꾸므로 다시 초기값 '.'로 초기화
			// board를 복사해서 넘기면 너무 복사가 많이 일어나므로 원본을 사용
			board[y][x] = '.';
		}
	}

	if (minValue == DRAW || minValue == DEFAULT)
		return ret = DRAW;

	// 상대 턴 값과 비교해 저장했으므로 반대값 리턴
	// 이를 위해 WIN = 1 LOSE = -1로 반대 부호로 설정
	return ret = -minValue;
}

int main()
{
	board = vector<vector<char>>
	{
		{'o','.','.'},
		{'.','o','x'},
		{'.','.','.'},
	}; 

	for (int i = 0; i < 19683; ++i)
		cache[i] = DEFAULT;

	int win = CanWin(board, 'o');
	switch (win)
	{
	case WIN:
		cout << "Win" << endl;
		break;
	case DRAW:
		cout << "Draw" << endl;
		break;
	case LOSE:
		cout << "Lose" << endl;
		break;
	}
}
