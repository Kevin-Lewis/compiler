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
	void parse_primary_expression();
	void parse_postfix_expression();
	void parse_argument_list();
	void parse_argument();
	void parse_unary_expression();
	void parse_cast_expression();
	void parse_multiplicative_expression();
	void parse_additive_expression();
	void parse_shift_expression();
	void parse_relational_expression();
	void parse_equality_expression();
	void parse_bitwise_and_expression();
	void parse_bitwise_xor_expression();
	void parse_bitwise_or_expression();
	void parse_logical_and_expression();
	void parse_logical_or_expression();
	void parse_conditional_expression();
	void parse_assignment_expression();
	void parse_constant_expresssion();

	//statement parsing
	void parse_statement();
	void parse_block_statement();
	void parse_statement_seq();
	void parse_if_statement();
	void parse_while_statement();
	void parse_break_statement();
	void parse_continue_statement();
	void parse_return_statement();
	void parse_declaration_statement();
	void parse_expression_statement();

	//declaration parsing
	void parse_program();
	void parse_declaration_seq();
	void parse_declaration();
	void parse_local_declaration();
	void parse_object_definition();
	void parse_variable_definition();
	void parse_constant_definition();
	void parse_value_definition();
	void parse_function_definition();
	void parse_parameter_list();
	void parse_parameter();

	//returns the name of the current token
	token_name lookahead(){return tokens.front().name;}

	//tries to match a token to the lookahead token
	void match(token_name t);

	//consumes the current token
	void accept(){assert(!tokens.empty()); tokens.pop_front();}


};