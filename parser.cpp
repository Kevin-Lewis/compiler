#pragma once

#include "parser.hpp"

#include <iostream>

void parser::initialize_keytable(){
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

void parser::match(token_name t){
	if(lookahead() == t){std::cout << tokens.front().toString(); accept();}
	else{throw std::runtime_error("Syntax error.");}
}

type* parser::parse_type(){std::cout << "TYPE-"; parse_postfix_type();}

type* parser::parse_postfix_type(){
	std::cout << "PFIXTYPE-";
	parse_basic_type();
	switch(lookahead()){
		case tok_op_mul:
			match(tok_op_mul);
			break;
		case tok_lbracket:
			match(tok_lbracket);
			if(lookahead() == tok_rbracket){match(tok_rbracket);}
			else{parse_expression(); match(tok_rbracket);}
			break;
		default: break;
	}
}

type* parser::parse_reference_type(){
	std::cout << "REFTYPE-";
	parse_postfix_type();
	switch(lookahead()){
		case tok_op_and_bw: match(tok_op_and_bw); break;
		default: break;
	}
}
type* parser::parse_basic_type(){
	std::cout << "BASICTYPE-";
	switch(lookahead()){
		case tok_kw_bool: match(tok_kw_bool); break;
		case tok_kw_int: match(tok_kw_int); break;
		case tok_kw_float: match(tok_kw_float); break;
		case tok_kw_char: match(tok_kw_char); break;
		case tok_lparen: 
			match(tok_lparen);
			parse_type_list();
			match(tok_rparen);
			break;
		default: break;

	}
}
type* parser::parse_type_list(){
	std::cout << "TYPELIST-";
	parse_type();
}


//Expression Parsing
expression* parser::parse_expression(){std::cout << "EXPRESSION-"; parse_assignment_expression();}

expression* parser::parse_primary_expression(){
	std::cout << "PRIMARYEXPR-";
	switch(lookahead()){
		case tok_lparen: match(tok_lparen); parse_expression(); match(tok_rparen); break;
		case tok_decimal_integer_literal: match(tok_decimal_integer_literal); break;
		case tok_hexadecimal_integer_literal: match(tok_hexadecimal_integer_literal); break;
		case tok_binary_integer_literal: match(tok_binary_integer_literal); break;
		case tok_floating_point_literal: match(tok_floating_point_literal); break;
		case tok_boolean_literal: match(tok_boolean_literal); break;
		case tok_character_literal: match(tok_character_literal); break;
		case tok_string_literal: match(tok_string_literal); break;
		case tok_identifier: match(tok_identifier); break;
		default: break;
	}
}
expression* parser::parse_postfix_expression(){
	std::cout << "PFIXEXPR-";
	parse_primary_expression();
	switch(lookahead()){
		case tok_lparen:
			parse_argument_list();
			match(tok_rparen);
			break;
		case tok_lbracket:
			parse_argument_list();
			match(tok_rbracket);
			break;
		default: 
			break;
	}
}
expression* parser::parse_argument_list(){
	std::cout << "ARGLIST-";
	parse_argument();
	while(lookahead() == tok_comma){
		parse_argument();
	}
}
expression* parser::parse_argument(){std::cout << "ARGUMENT-"; parse_expression();}
expression* parser::parse_unary_expression(){
	std::cout << "UNARYEXPR-";
	switch(lookahead()){
		case tok_op_plus: match(tok_op_plus); break;
		case tok_op_minus: match(tok_op_minus); break;
		case tok_op_not_bw: match(tok_op_not_bw); break;
		case tok_op_and_bw: match(tok_op_and_bw); break;
		case tok_op_mul: match(tok_op_mul); break;
		default: break;
	}
	parse_postfix_expression();
}
expression* parser::parse_cast_expression(){
	std::cout << "CASTEXPR-";
	parse_unary_expression();
	while(lookahead() == tok_kw_as){
		match(tok_kw_as); 
		parse_unary_expression();
	}
}
expression* parser::parse_multiplicative_expression(){
	std::cout << "MULTEXPR-";
	parse_cast_expression();
	while(lookahead()==tok_op_mul || lookahead()==tok_op_div || lookahead()==tok_op_mod){
		accept();
		parse_cast_expression();
	}
}
expression* parser::parse_additive_expression(){
	std::cout << "ADDEXPR-";
	parse_multiplicative_expression();
	while(lookahead()==tok_op_plus || lookahead()==tok_op_minus){
		accept();
		parse_multiplicative_expression();
	}
}
expression* parser::parse_shift_expression(){
	std::cout << "SHIFTEXPR-";
	parse_additive_expression();
	//TODO - add shift operators
	while(lookahead()==tok_op_plus || lookahead()==tok_op_minus){
		accept();
		parse_additive_expression();
	}
}
expression* parser::parse_relational_expression(){
	std::cout << "RELATEXPR-";
	parse_shift_expression();
	while(lookahead()==tok_op_gt || lookahead()==tok_op_lt || lookahead()==tok_op_gte || lookahead() == tok_op_lte){
		accept();
		parse_shift_expression();
	}
}
expression* parser::parse_equality_expression(){
	std::cout << "EQUALITYEXPR-";
	parse_relational_expression();
	while(lookahead()==tok_op_eq || lookahead()==tok_op_neq){
		accept();
		parse_relational_expression();
	}
}
expression* parser::parse_bitwise_and_expression(){
	std::cout << "BITWISEEXPR-";
	parse_relational_expression();
	while(lookahead()==tok_op_and_bw){
		accept();
		parse_relational_expression();
	}
}
expression* parser::parse_bitwise_xor_expression(){
	std::cout << "BXOREXPR-";
	parse_bitwise_and_expression();
	while(lookahead()==tok_op_xor_bw){
		accept();
		parse_bitwise_and_expression();
	}
}
expression* parser::parse_bitwise_or_expression(){
	std::cout << "BOREXPR-";
	parse_bitwise_xor_expression();
	while(lookahead()==tok_op_or_bw){
		accept();
		parse_bitwise_xor_expression();
	}
}
expression* parser::parse_logical_and_expression(){
	std::cout << "LANDEXPR-";
	parse_bitwise_or_expression();
	while(lookahead()==tok_kw_and){
		accept();
		parse_bitwise_or_expression();
	}
}
expression* parser::parse_logical_or_expression(){
	std::cout << "LOREXPR-";
	parse_logical_and_expression();
	while(lookahead()==tok_kw_or){
		accept();
		parse_logical_and_expression();
	}
}
expression* parser::parse_conditional_expression(){
	std::cout << "CONDITIONALEXPR-";
	parse_logical_or_expression();
	if(lookahead() == tok_op_question){
		match(tok_op_question);
		parse_expression();
		match(tok_colon);
		parse_conditional_expression();

	}
}
expression* parser::parse_assignment_expression(){
	std::cout << "ASSIGNMENTEXPR-";
	parse_conditional_expression();
	if(lookahead() == tok_op_assignment){
		match(tok_op_assignment);
		parse_assignment_expression();
	}
}
expression* parser::parse_constant_expresssion(){std::cout << "CONSTEXPR-"; parse_conditional_expression();}

//statement parsing
statement* parser::parse_statement(){
	std::cout << "PARSESTMT-";
	switch(lookahead()){
		case tok_lbrace:
			parse_block_statement();
			break;
		case tok_kw_if:
			parse_if_statement();
			break;
		case tok_kw_while:
			parse_while_statement();
			break;
		case tok_kw_return:
			parse_return_statement();
			break;
		default:
			break;
		//TODO: Add break and continue tokens
	}
}
statement* parser::parse_block_statement(){
	std::cout << "BLOCKSTMT-";
	if(lookahead() == tok_lbrace){
		match(tok_lbrace);
		parse_statement_seq();
		match(tok_rbrace);
	}
}
statement* parser::parse_statement_seq(){
	std::cout << "STMTSEQ-";
	parse_statement();
	while(lookahead() != tok_rbrace){
		parse_statement();
	}
}
statement* parser::parse_if_statement(){
	std::cout << "IFSTMT-";
	match(tok_kw_if);
	match(tok_lparen);
	parse_expression();
	match(tok_rparen);
	parse_statement();
	if(lookahead() == tok_kw_else){
		match(tok_kw_else);
		parse_statement();
	}
}
statement* parser::parse_while_statement(){
	std::cout << "WHILESTMT-";
	match(tok_kw_while);
	match(tok_lparen);
	parse_expression();
	match(tok_rparen);
	parse_statement();
}
//statement* parser::parse_break_statement(){break;}
//statement* parser::parse_continue_statement(){continue;}
statement* parser::parse_return_statement(){
	std::cout << "RETURNSTMT-";
	match(tok_kw_return);
	if(lookahead() != tok_semicolon){parse_expression();}
	match(tok_semicolon);
}
//statement* parser::parse_declaration_statement(){parse_local_declaration();}
statement* parser::parse_expression_statement(){
	std::cout << "EXPRSTMT-";
	parse_expression();
	match(tok_semicolon);
}

//declaration parsing
declaration* parser::parse_program(){
	std::cout << "PROGRAM-";
	while(!tokens.empty()){
		parse_declaration_seq();
	}
}
declaration* parser::parse_declaration_seq(){
	std::cout << "DECLARATIONSEQ-";
	parse_declaration();
}
declaration* parser::parse_declaration(){
	std::cout << "DECLARATION-";
	switch(lookahead(2)){
		case tok_colon:
			parse_object_definition();
			break;
		case tok_lparen:
			parse_function_definition();
			break;
		default:
			break;
	}
}
declaration* parser::parse_local_declaration(){std::cout << "LOCALDEC-"; parse_object_definition();}
declaration* parser::parse_object_definition(){
	std::cout << "OBJECTDEF-";
	switch(lookahead()){
		case tok_kw_var:
			parse_variable_definition();
			break;
		case tok_kw_let:
			parse_constant_definition();
			break;
		case tok_kw_def:
			parse_value_definition();
			break;
		default:
			break;
	}
}
declaration* parser::parse_variable_definition(){
	std::cout << "VARIABLEDEF-";
	match(tok_kw_var);
	match(tok_identifier);
	match(tok_colon);
	parse_type();
	if(lookahead() != tok_semicolon){
		match(tok_op_assignment);
		parse_expression();
	}
	match(tok_semicolon);
}
declaration* parser::parse_constant_definition(){
	std::cout << "CONSTANTDEF-";
	match(tok_kw_let);
	match(tok_identifier);
	match(tok_colon);
	parse_type();
	match(tok_op_assignment);
	parse_expression();
	match(tok_semicolon);

}
declaration* parser::parse_value_definition(){
	std::cout << "VALUEDEF-";
	match(tok_kw_def);
	match(tok_identifier);
	match(tok_colon);
	parse_type();
	match(tok_op_assignment);
	parse_expression();
	match(tok_semicolon);
}
declaration* parser::parse_function_definition(){
	std::cout << "FUNCTIONDEF-";
	match(tok_kw_def);
	match(tok_identifier);
	match(tok_lparen);
	parse_parameter_list();
	match(tok_rparen);
	parse_block_statement();
	//TODO: Add arrow token
}
declaration* parser::parse_parameter_list(){
	std::cout << "PARAMLIST-";
	parse_parameter();
	while(lookahead() == tok_comma){
		match(tok_comma);
		parse_parameter();
	}
}
declaration* parser::parse_parameter(){
	std::cout << "PARSEPARAM-";
	match(tok_identifier);
	match(tok_colon);
	parse_type();
}