#include <string>
#include <vector>
#include "token.hpp"

struct lexer{

	const char* first;
	const char* last;

	//vector of tokens
	std::vector<token> tokens;

	lexer(const std::string& str) : first(str.data()), last(first + str.size()){}

	//lex a file
	void lex();

	//Checks the next character in the string
	char peek();

	//Increments the first pointer by 1
	void accept();

	//appends an word token
	void word();

	//appends a number token
	void number();

};