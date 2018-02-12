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
					if(peek() == '='){tokens.push_back(token(tok_op_eq)); step();}
					else{tokens.push_back(token(tok_op_assignment));} 	break;
				case '!':	
					if(peek() == '='){tokens.push_back(token(tok_op_neq)); step();}
																		break;
				case '<':
					if(peek() == '='){tokens.push_back(token(tok_op_lte)); step();}
					else{tokens.push_back(token(tok_op_lt));} 			break;
				case '>':
					if(peek() == '='){tokens.push_back(token(tok_op_gte)); step();}
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
		step();
	}
}

char lexer::peek(){return *(first+1);}

void lexer::step(){if(first != last){first++;}}

void lexer::step_back(){first--;}

void lexer::word(){
	std::string str;
	while(std::isalpha(*first) && *first != *last){
		str += *first;
		step();
	}

	std::unordered_map<std::string, token_name>::iterator it = kw_table.find(str);
	if(it != kw_table.end()){
		tokens.push_back(token(it->second, it->first));
	}
	else{
		tokens.push_back(token(tok_identifier, str));
	}
	step_back();
}

void lexer::number(){
	std::string str;
	while(std::isdigit(*first) && *first != *last){
		str += *first;
		step();
	}
	tokens.push_back(token(tok_decimal_integer_literal, std::stoi(str)));
	step_back();
}

void lexer::initialize_keytable(){
	kw_table.insert({"and",tok_kw_and});
	kw_table.insert({"or",tok_kw_or});
	kw_table.insert({"not",tok_kw_not});
	kw_table.insert({"let",tok_kw_let});
	kw_table.insert({"bool",tok_kw_bool});
	kw_table.insert({"char",tok_kw_char});
	kw_table.insert({"if",tok_kw_if});
	kw_table.insert({"else",tok_kw_else});
	kw_table.insert({"true",tok_kw_true});
	kw_table.insert({"false",tok_kw_false});
	kw_table.insert({"var",tok_kw_var});
	kw_table.insert({"def",tok_kw_def});
	kw_table.insert({"int",tok_kw_int});
	kw_table.insert({"float",tok_kw_float});
}