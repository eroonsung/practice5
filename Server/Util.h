#ifndef  GOMOKU_UTIL_H
#define GOMOKU_UTIL_H
using namespace std;
#include <vector>
#include <sstream>
class Util {
public:
	// 문자열 : Hello World Eroon
	// 구분자 : ' '
	// Hello, World, Eroon
	vector<string> getTokens(string input, char delimiter);
	//어떠한 입력값이 들어왔을 때 그 입력값을 delimiter(구분자)를 기준으로 해서 토큰으로 나누겠다
};
#endif // ! GOMOKU_UTIL_H
