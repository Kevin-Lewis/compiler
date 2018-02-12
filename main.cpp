#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "lexer.hpp"

int main(){

	std::ifstream file("test.cpp");
	std::stringstream buffer;
	buffer << file.rdbuf();

	std::string str = buffer.str();

	lexer l = lexer(str);

	while(l.first != l.last){
		l.lex();
	}

	for(int i = 0; i < l.tokens.size(); i++){
		std::cout << l.tokens[i].toString();
		std::cout << std::endl;
	}

	std::cout << std::endl;


}