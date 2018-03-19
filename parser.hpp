#pragma once

#include "token.hpp"
#include "lexer.hpp"

#include <cassert>
#include <deque>

struct parser{

	//constructs a lexer object given a reference to a string
	parser(const std::deque<token> tokens) : tokens(tokens) {initialize_keytable();}
  
	//stream of tokens obtained from the lexer
	std::deque<token> tokens;

	//keyword table
	std::unordered_map<std::string, token_name> kw_table;

	//create keyword table
	void initialize_keytable();

	//type parsing
	void parse_type();
	void parse_postfix_type();
	void parse_reference_type();
	void parse_basic_type();
	void parse_type_list();

	//expression parsing
	void parse_expression();

	//returns the name of the current token
	token_name lookahead(){return tokens.front().name;}

	//tries to match a token to the lookahead token
	void match(token_name t);

	//consumes the current token
	void accept(){assert(!tokens.empty()); tokens.pop_front();}


};