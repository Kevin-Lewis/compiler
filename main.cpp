#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "lexer.hpp"
#include "parser.hpp"

int main(){
	std::ifstream file("test2.mc");
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string str = buffer.str();

	lexer l = lexer(str);
	
	while(!l.end()){
		l.lex();
	}

	parser p = parser(l.tokens);
	p.parse_program();

	//for(int i = 0; i < l.tokens.size(); i++){
	//	std::cout << l.tokens[i].toString();
	//	if(i < l.tokens.size()-1){std::cout << std::endl;}
	//}

	std::cout << std::endl;


}