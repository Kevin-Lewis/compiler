#pragma once

#include "parser.hpp"

#include <iostream>
#include <string>

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

type* parser::parse_type(){return parse_postfix_type();}

type* parser::parse_postfix_type(){
	type* t = parse_basic_type();
	switch(lookahead()){
		case tok_op_mul:
			match(tok_op_mul);
			return new ptr_type(t);
		case tok_lbracket:
			match(tok_lbracket);
			if(lookahead() == tok_rbracket){match(tok_rbracket);}
			else{parse_expression(); match(tok_rbracket);}
			break;
		default: break;
	}
	return t;
}

type* parser::parse_reference_type(){
	type* t = parse_postfix_type();
	switch(lookahead()){
		case tok_op_and_bw: match(tok_op_and_bw); return new ref_type(t);
		default: break;
	}
	return t;
}
type* parser::parse_basic_type(){
	type* t;
	switch(lookahead()){
		case tok_kw_bool: match(tok_kw_bool); return new bool_type();
		case tok_kw_int: match(tok_kw_int); return new int_type();
		case tok_kw_float: match(tok_kw_float); return new float_type();
		case tok_kw_char: match(tok_kw_char); return new char_type();
		case tok_lparen: 
			match(tok_lparen);
			t = parse_type_list();
			match(tok_rparen);
			break;
		default: break;
	}
	return t;
}
type* parser::parse_type_list(){
	return parse_type();
}


//Expression Parsing
expression* parser::parse_expression(){return parse_assignment_expression();}

expression* parser::parse_primary_expression(){
	token t = tokens.front();
	bool b = false;
	expression* e;
	switch(lookahead()){
		case tok_lparen: match(tok_lparen); e = parse_expression(); match(tok_rparen); break;

		case tok_decimal_integer_literal:
			match(tok_decimal_integer_literal);
			return new int_expression(new int_type(), t.attr.value);
		
		case tok_hexadecimal_integer_literal:
			match(tok_hexadecimal_integer_literal);
			return new int_expression(new int_type(), t.attr.value);
		
		case tok_binary_integer_literal:
			match(tok_binary_integer_literal);
			return new int_expression(new int_type(), t.attr.value);
		
		case tok_floating_point_literal:
			match(tok_floating_point_literal);
			return new float_expression(new float_type(), t.attr.floating);
		
		case tok_boolean_literal:
			match(tok_boolean_literal);
			if(t.attr.name == "true"){b = true;}
			return new bool_expression(new bool_type(), b);
		
		case tok_character_literal:
			match(tok_character_literal);
			return new char_expression(new char_type(), t.attr.name.c_str()[0]);
		
		case tok_string_literal:
			match(tok_string_literal); break; 
			//TODO - Add string support
		
		case tok_identifier:
			match(tok_identifier); break;
			//TODO - Handle Identifiers

		default: break;
	}
	return e;
}
expression* parser::parse_postfix_expression(){
	expression* e = parse_primary_expression();
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
	return e;
}
expression* parser::parse_argument_list(){
	//TODO - Handle argument lists
	expression* e = parse_argument();
	while(lookahead() == tok_comma){
		parse_argument();
	}
	return e;
}
expression* parser::parse_argument(){return parse_expression();}
expression* parser::parse_unary_expression(){
	expression* e;
	switch(lookahead()){
		case tok_op_plus:
			match(tok_op_plus);
			e = parse_postfix_expression();
			return new u_add_expression(e);

		case tok_op_minus:
			match(tok_op_minus);
			e = parse_postfix_expression();
			return new u_sub_expression(e);

		case tok_op_not_bw:
			match(tok_op_not_bw);
			e = parse_postfix_expression();
			return new u_not_expression(e);
		
		case tok_op_and_bw: 
			match(tok_op_and_bw);
			e = parse_postfix_expression();
			return new u_ref_expression(e);
		
		case tok_op_mul:
			match(tok_op_mul);
			e = parse_postfix_expression();
			return new u_mul_expression(e);
		
		default: break;
	}
	return parse_postfix_expression();
}
expression* parser::parse_cast_expression(){
	expression* e = parse_unary_expression();
	while(lookahead() == tok_kw_as){
		match(tok_kw_as); 
		type* t = parse_type();
		return new cast_expression(e,t);
	}
	return e;
}
expression* parser::parse_multiplicative_expression(){
	parse_cast_expression();
	while(lookahead()==tok_op_mul || lookahead()==tok_op_div || lookahead()==tok_op_mod){
		accept();
		parse_cast_expression();
	}
}
expression* parser::parse_additive_expression(){
	parse_multiplicative_expression();
	while(lookahead()==tok_op_plus || lookahead()==tok_op_minus){
		accept();
		parse_multiplicative_expression();
	}
}
expression* parser::parse_shift_expression(){
	parse_additive_expression();
	//TODO - add shift operators
	while(lookahead()==tok_op_plus || lookahead()==tok_op_minus){
		accept();
		parse_additive_expression();
	}
}
expression* parser::parse_relational_expression(){
	parse_shift_expression();
	while(lookahead()==tok_op_gt || lookahead()==tok_op_lt || lookahead()==tok_op_gte || lookahead() == tok_op_lte){
		accept();
		parse_shift_expression();
	}
}
expression* parser::parse_equality_expression(){
	parse_relational_expression();
	while(lookahead()==tok_op_eq || lookahead()==tok_op_neq){
		accept();
		parse_relational_expression();
	}
}
expression* parser::parse_bitwise_and_expression(){
	parse_relational_expression();
	while(lookahead()==tok_op_and_bw){
		accept();
		parse_relational_expression();
	}
}
expression* parser::parse_bitwise_xor_expression(){
	parse_bitwise_and_expression();
	while(lookahead()==tok_op_xor_bw){
		accept();
		parse_bitwise_and_expression();
	}
}
expression* parser::parse_bitwise_or_expression(){
	parse_bitwise_xor_expression();
	while(lookahead()==tok_op_or_bw){
		accept();
		parse_bitwise_xor_expression();
	}
}
expression* parser::parse_logical_and_expression(){
	parse_bitwise_or_expression();
	while(lookahead()==tok_kw_and){
		accept();
		parse_bitwise_or_expression();
	}
}
expression* parser::parse_logical_or_expression(){
	parse_logical_and_expression();
	while(lookahead()==tok_kw_or){
		accept();
		parse_logical_and_expression();
	}
}
expression* parser::parse_conditional_expression(){
	parse_logical_or_expression();
	if(lookahead() == tok_op_question){
		match(tok_op_question);
		parse_expression();
		match(tok_colon);
		parse_conditional_expression();

	}
}
expression* parser::parse_assignment_expression(){
	parse_conditional_expression();
	if(lookahead() == tok_op_assignment){
		match(tok_op_assignment);
		parse_assignment_expression();
	}
}
expression* parser::parse_constant_expresssion(){return parse_conditional_expression();}

//statement parsing
statement* parser::parse_statement(){
	switch(lookahead()){
		case tok_lbrace:
			return parse_block_statement();
			break;
		case tok_kw_if:
			return parse_if_statement();
			break;
		case tok_kw_while:
			return parse_while_statement();
			break;
		case tok_kw_return:
			return parse_return_statement();
			break;
		default:
			break;
		//TODO: Add break and continue tokens
	}
}
statement* parser::parse_block_statement(){
	if(lookahead() == tok_lbrace){
		match(tok_lbrace);
		parse_statement_seq();
		match(tok_rbrace);
	}
}
statement* parser::parse_statement_seq(){
	parse_statement();
	while(lookahead() != tok_rbrace){
		parse_statement();
	}
}
statement* parser::parse_if_statement(){
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
	match(tok_kw_while);
	match(tok_lparen);
	parse_expression();
	match(tok_rparen);
	parse_statement();
}
//statement* parser::parse_break_statement(){break;}
//statement* parser::parse_continue_statement(){continue;}
statement* parser::parse_return_statement(){
	match(tok_kw_return);
	if(lookahead() != tok_semicolon){parse_expression();}
	match(tok_semicolon);
}
//statement* parser::parse_declaration_statement(){parse_local_declaration();}
statement* parser::parse_expression_statement(){
	parse_expression();
	match(tok_semicolon);
}

//declaration parsing
declaration* parser::parse_program(){
	while(!tokens.empty()){
		parse_declaration_seq();
	}
}
declaration* parser::parse_declaration_seq(){
	parse_declaration();
}
declaration* parser::parse_declaration(){
	switch(lookahead(2)){
		case tok_colon:
			return parse_object_definition();
			break;
		case tok_lparen:
			return parse_function_definition();
			break;
		default:
			break;
	}
}
declaration* parser::parse_local_declaration(){return parse_object_definition();}
declaration* parser::parse_object_definition(){
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
	match(tok_kw_let);
	match(tok_identifier);
	match(tok_colon);
	parse_type();
	match(tok_op_assignment);
	parse_expression();
	match(tok_semicolon);

}
declaration* parser::parse_value_definition(){
	match(tok_kw_def);
	match(tok_identifier);
	match(tok_colon);
	parse_type();
	match(tok_op_assignment);
	parse_expression();
	match(tok_semicolon);
}
declaration* parser::parse_function_definition(){
	match(tok_kw_def);
	match(tok_identifier);
	match(tok_lparen);
	parse_parameter_list();
	match(tok_rparen);
	parse_block_statement();
	//TODO: Add arrow token
}
declaration* parser::parse_parameter_list(){
	parse_parameter();
	while(lookahead() == tok_comma){
		match(tok_comma);
		parse_parameter();
	}
}
declaration* parser::parse_parameter(){
	match(tok_identifier);
	match(tok_colon);
	parse_type();
}