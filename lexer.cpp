#include "lexer.hpp"
#include <cctype>
#include <vector>
#include <string>
#include <iostream>

static bool isHex(char c){
	return std::isdigit(c) || c=='A' || c=='B' || c=='C' || c=='D' || c=='E' || c=='F';
}

void lexer::lex(){
	if(first != last){
		if(!std::isspace(*first)){
			switch(*first){
				case '#':	skip_comment(); 							break;

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

char lexer::peek_back(){return *(first-1);}

void lexer::step(){if(first != last){first++;}}

void lexer::step_back(){first--;}

void lexer::skip_comment(){while(*first != '\n'){step();}}


void lexer::word(){
	std::string str;
	char prev = peek_back();
	while(std::isalpha(*first) && !end()){
		str += *first;
		step();
	}

	std::unordered_map<std::string, token_name>::iterator it = kw_table.find(str);
	if(it != kw_table.end()){ //a keyword token is found
		tokens.push_back(token(it->second, it->first));
	}
	else if(prev == '\"'){ //a string token is found
		if(*first != '\"'){throw std::runtime_error("unterminated string literal");}
		tokens.push_back(token(tok_string_literal, str));
	}
	else if(prev == '\''){ //a character token is found
		if(*first != '\''){throw std::runtime_error("unterminated string literal");}
		if(str.size() > 1){throw std::runtime_error("invalid character literal");}
		tokens.push_back(token(tok_character_literal, str));
	}
	else{ //an identifier token is found
		if(!std::isdigit(*first)){tokens.push_back(token(tok_identifier, str));}
	}
	step_back();
}

void lexer::number(){
	std::string str;
	char type = *(first - 1); //checks for binary or hex integer

	while((std::isdigit(*first) || *first == '.' || isHex(*first)) && !end()){
		str += *first;
		step();
	}

	if(str.find('.') != std::string::npos){ //floating point number
		tokens.push_back(token(tok_floating_point_literal, std::stod(str)));
	}
	else{ //integer
		if(type == 'b' || str[0] == 'B'){ //binary number
			for(int i=0; i<str.size(); i++){if(str[i] != '0' && str[i] != '1'){throw std::runtime_error("binary literal contains invalid values");}}
			tokens.push_back(token(tok_binary_integer_literal, std::stoi(str, 0, 2)));

		}
		else if(type == 'x' || str[0] == 'X'){ //hexadecimal number
			for(int i=0; i<str.size(); i++){if(!(isHex(str[i]))){throw std::runtime_error("hexadecimal literal contains invalid values");}}
			tokens.push_back(token(tok_hexadecimal_integer_literal, std::stoi(str, 0, 16)));

		}
		else{ //decimal number
			for(int i=0; i<str.size(); i++){if(!(std::isdigit(str[i]))){throw std::runtime_error("decimal literal contains invalid values");}}
			tokens.push_back(token(tok_decimal_integer_literal, std::stoi(str)));
		}	
	}
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