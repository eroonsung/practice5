#include "Util.h"

// => 토큰을 분리하도록 해주는 대표적인 Util library
// input 값이 들어왔을 때 
vector<string> Util::getTokens(string input, char delimiter) {
	vector<string> tokens;
	// istringstrem으로 객체화 
	istringstream f(input);
	string s;
	// 구분자를 기준으로 해서 입력으로 들어온 문자열을 입력값으로 처리해서 결과 벡터 객체에 하나씩 담아서 반환
	while (getline(f, s, delimiter)) {
		tokens.push_back(s);
	}
	return tokens;
}