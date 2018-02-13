#include <string>
#include <vector>
#include <unordered_map>
#include "token.hpp"

struct lexer{

	const char* first;
	const char* last;

	//vector of tokens
	std::vector<token> tokens;

	//keyword table
	std::unordered_map<std::string, token_name> kw_table;

	lexer(const std::string& str) : first(str.data()), last(first + str.size()){initialize_keytable();}

	//create keyword table
	void initialize_keytable();

	//lex a file
	void lex();

	//checks the next character in the string
	char peek();

	//looks one character behind the current pointer
	char peek_back();

	//increments the first pointer by 1
	void step();

	//decrements the first pointer by 1
	void step_back();

	//appends an word token
	void word();

	//appends a number token
	void number();

	//checks for end of file
	bool end(){return *first == *last;}
};