#pragma once

#include "token.hpp"
#include "lexer.hpp"
#include "types.hpp"
#include "expressions.hpp"
#include "statements.hpp"
#include "declarations.hpp"
#include "scope.hpp"

#include <cassert>
#include <deque>
#include <vector>

struct parser{

	//constructs a lexer object given a reference to a string
	parser(const std::deque<token> tokens) : tokens(tokens) {initialize_keytable();}
  
	//stream of tokens obtained from the lexer
	std::deque<token> tokens;

	//keyword table
	std::unordered_map<std::string, token_name> kw_table;

	//current scope
	scope* current_scope;

	//create keyword table
	void initialize_keytable();

	//type parsing
	type* parse_type();
	type* parse_postfix_type();
	type* parse_reference_type();
	type* parse_basic_type();
	type* parse_type_list();

	//expression parsing
	expression* parse_expression();
	expression* parse_primary_expression();
	expression* parse_postfix_expression();
	expression* parse_argument_list();
	expression* parse_argument();
	expression* parse_unary_expression();
	expression* parse_cast_expression();
	expression* parse_multiplicative_expression();
	expression* parse_additive_expression();
	expression* parse_shift_expression();
	expression* parse_relational_expression();
	expression* parse_equality_expression();
	expression* parse_bitwise_and_expression();
	expression* parse_bitwise_xor_expression();
	expression* parse_bitwise_or_expression();
	expression* parse_logical_and_expression();
	expression* parse_logical_or_expression();
	expression* parse_conditional_expression();
	expression* parse_assignment_expression();
	expression* parse_constant_expresssion();

	//statement parsing
	statement* parse_statement();
	statement* parse_block_statement();
	std::vector<statement*> parse_statement_seq();
	statement* parse_if_statement();
	statement* parse_while_statement();
	statement* parse_break_statement();
	statement* parse_continue_statement();
	statement* parse_return_statement();
	statement* parse_declaration_statement();
	statement* parse_expression_statement();

	//declaration parsing
	declaration* parse_program();
	declaration* parse_declaration_seq();
	declaration* parse_declaration();
	declaration* parse_local_declaration();
	declaration* parse_object_definition();
	declaration* parse_variable_definition();
	declaration* parse_constant_definition();
	declaration* parse_value_definition();
	declaration* parse_function_definition();
	std::vector<declaration*> parse_parameter_list();
	declaration* parse_parameter();

	//returns the name of the current token
	token_name lookahead(){return tokens.front().name;}
	token_name lookahead(int x){return tokens[x].name;}

	//tries to match a token to the lookahead token
	void match(token_name t);

	//consumes the current token
	void accept(){assert(!tokens.empty()); tokens.pop_front();}

	//scope stuff
	void enter_scope(scope::kind k);
	void leave_scope();
	void declare_in_scope(declaration* d);

};