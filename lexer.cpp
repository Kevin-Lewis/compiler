#include "lexer.hpp"
#include <cctype>
#include <vector>
#include <string>
#include <iostream>

void lexer::lex(){
	if(first != last){
		if(!std::isspace(*first)){
			switch(*first){
				case '{':	tokens.push_back(token(tok_lbrace)); 		break;
				case '}':	tokens.push_back(token(tok_rbrace)); 		break;
				case '(':	tokens.push_back(token(tok_lparen)); 		break;
				case ')':	tokens.push_back(token(tok_rparen)); 		break;
				case '[':	tokens.push_back(token(tok_lbracket)); 		break;
				case ']':	tokens.push_back(token(tok_rbracket)); 		break;
				case ',':	tokens.push_back(token(tok_comma)); 		break;
				case ';':	tokens.push_back(token(tok_semicolon)); 	break;
				case ':':	tokens.push_back(token(tok_colon)); 		break;
				case '=':
					if(peek() == '='){tokens.push_back(token(tok_op_eq)); accept();}
					else{tokens.push_back(token(tok_op_assignment));} 	break;
				case '!':	
					if(peek() == '='){tokens.push_back(token(tok_op_neq)); accept();}
																		break;
				case '<':
					if(peek() == '='){tokens.push_back(token(tok_op_lte)); accept();}
					else{tokens.push_back(token(tok_op_lt));} 			break;
				case '>':
					if(peek() == '='){tokens.push_back(token(tok_op_gte)); accept();}
					else{tokens.push_back(token(tok_op_gt));} 			break;

				case '+':	tokens.push_back(token(tok_op_plus));		break;
				case '-':	tokens.push_back(token(tok_op_minus)); 		break;
				case '*':	tokens.push_back(token(tok_op_mul)); 		break;
				case '/':	tokens.push_back(token(tok_op_div)); 		break;
				case '%':	tokens.push_back(token(tok_op_mod)); 		break;
				case '?':	tokens.push_back(token(tok_op_question)); 	break;
				case '&':	tokens.push_back(token(tok_op_and_bw)); 	break;
				case '|':	tokens.push_back(token(tok_op_or_bw));		break;
				case '^':	tokens.push_back(token(tok_op_xor_bw)); 	break;
				case '~':	tokens.push_back(token(tok_op_not_bw));		break;
				default:
					if(std::isalpha(*first)){word();}
					else if(std::isdigit(*first)){number();}
					break;
			}
		}
		accept();
	}
}

char lexer::peek(){return *(first+1);}

void lexer::accept(){if(first != last){first++;}}

void lexer::word(){
	std::string str;
	while(std::isalpha(*first) && *first != *last){
		str += *first;
		accept();
	}
	tokens.push_back(token(tok_identifier, str));
	*first--;
}

void lexer::number(){
	std::string str;
	while(std::isdigit(*first) && *first != *last){
		str += *first;
		accept();
	}
	tokens.push_back(token(tok_decimal_integer_literal, std::stoi(str)));
	*first--;
}