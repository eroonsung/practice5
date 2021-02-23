#include "Util.h"

// => ��ū�� �и��ϵ��� ���ִ� ��ǥ���� Util library
// input ���� ������ �� 
vector<string> Util::getTokens(string input, char delimiter) {
	vector<string> tokens;
	// istringstrem���� ��üȭ 
	istringstream f(input);
	string s;
	// �����ڸ� �������� �ؼ� �Է����� ���� ���ڿ��� �Է°����� ó���ؼ� ��� ���� ��ü�� �ϳ��� ��Ƽ� ��ȯ
	while (getline(f, s, delimiter)) {
		tokens.push_back(s);
	}
	return tokens;
}