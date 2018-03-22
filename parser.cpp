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
	if(lookahead() == t){accept();}
	else{throw std::runtime_error("Syntax error.");}
}

void parser::parse_type(){parse_postfix_type();}

void parser::parse_postfix_type(){
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

void parser::parse_reference_type(){
	parse_postfix_type();
	switch(lookahead()){
		case tok_op_and_bw: match(tok_op_and_bw); break;
		default: break;
	}
}
void parser::parse_basic_type(){
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
void parser::parse_type_list(){
	parse_type();
}


//Expression Parsing
void parser::parse_expression(){parse_assignment_expression();}

void parser::parse_primary_expression(){
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
void parser::parse_postfix_expression(){
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
void parser::parse_argument_list(){
	parse_argument();
	while(lookahead() == tok_comma){
		parse_argument();
	}
}
void parser::parse_argument(){parse_expression();}
void parser::parse_unary_expression(){
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
void parser::parse_cast_expression(){
	parse_unary_expression();
	while(lookahead() == tok_kw_as){
		match(tok_kw_as); 
		parse_unary_expression();
	}
}
void parser::parse_multiplicative_expression(){
	parse_cast_expression();
	while(lookahead()==tok_op_mul || lookahead()==tok_op_div || lookahead()==tok_op_mod){
		accept();
		parse_cast_expression();
	}
}
void parser::parse_additive_expression(){
	parse_multiplicative_expression();
	while(lookahead()==tok_op_plus || lookahead()==tok_op_minus){
		accept();
		parse_multiplicative_expression();
	}
}
void parser::parse_shift_expression(){
	parse_additive_expression();
	//TODO - add shift operators
	while(lookahead()==tok_op_plus || lookahead()==tok_op_minus){
		accept();
		parse_additive_expression();
	}
}
void parser::parse_relational_expression(){
	parse_shift_expression();
	while(lookahead()==tok_op_gt || lookahead()==tok_op_lt || lookahead()==tok_op_gte || lookahead() == tok_op_lte){
		accept();
		parse_shift_expression();
	}
}
void parser::parse_equality_expression(){
	parse_relational_expression();
	while(lookahead()==tok_op_eq || lookahead()==tok_op_neq){
		accept();
		parse_relational_expression();
	}
}
void parser::parse_bitwise_and_expression(){
	parse_relational_expression();
	while(lookahead()==tok_op_and_bw){
		accept();
		parse_relational_expression();
	}
}
void parser::parse_bitwise_xor_expression(){
	parse_bitwise_and_expression();
	while(lookahead()==tok_op_xor_bw){
		accept();
		parse_bitwise_and_expression();
	}
}
void parser::parse_bitwise_or_expression(){
	parse_bitwise_xor_expression();
	while(lookahead()==tok_op_or_bw){
		accept();
		parse_bitwise_xor_expression();
	}
}
void parser::parse_logical_and_expression(){
	parse_bitwise_or_expression();
	while(lookahead()==tok_kw_and){
		accept();
		parse_bitwise_or_expression();
	}
}
void parser::parse_logical_or_expression(){
	parse_logical_and_expression();
	while(lookahead()==tok_kw_or){
		accept();
		parse_logical_and_expression();
	}
}
void parser::parse_conditional_expression(){
	parse_logical_or_expression();
	if(lookahead() == tok_op_question){
		match(tok_op_question);
		parse_expression();
		match(tok_colon);
		parse_conditional_expression();

	}
}
void parser::parse_assignment_expression(){
	parse_conditional_expression();
	if(lookahead() == tok_op_assignment){
		match(tok_op_assignment);
		parse_assignment_expression();
	}
}
void parser::parse_constant_expresssion(){parse_conditional_expression();}

//statement parsing
void parser::parse_statement(){
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
void parser::parse_block_statement(){
	if(lookahead() == tok_lbrace){
		match(tok_lbrace);
		parse_statement_seq();
		match(tok_rbrace);
	}
}
void parser::parse_statement_seq(){
	parse_statement();
	while(lookahead() != tok_rbrace){
		parse_statement();
	}
}
void parser::parse_if_statement(){
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
void parser::parse_while_statement(){
	match(tok_kw_while);
	match(tok_lparen);
	parse_expression();
	match(tok_rparen);
	parse_statement();
}
//void parser::parse_break_statement(){break;}
//void parser::parse_continue_statement(){continue;}
void parser::parse_return_statement(){
	match(tok_kw_return);
	if(lookahead() != tok_semicolon){parse_expression();}
	match(tok_semicolon);
}
//void parser::parse_declaration_statement(){parse_local_declaration();}
void parser::parse_expression_statement(){
	parse_expression();
	match(tok_semicolon);
}

//declaration parsing
void parser::parse_program(){
	std::cout << tokens.size();
	while(!tokens.empty()){
		//std::cout << "TEST";
		parse_declaration_seq();
	}
}
void parser::parse_declaration_seq(){
	parse_declaration();
}
void parser::parse_declaration(){
	parse_object_definition();
}
void parser::parse_local_declaration(){parse_object_definition();}
void parser::parse_object_definition(){
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
void parser::parse_variable_definition(){
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
void parser::parse_constant_definition(){
	match(tok_kw_let);
	match(tok_identifier);
	match(tok_colon);
	parse_type();
	match(tok_op_assignment);
	parse_expression();
	match(tok_semicolon);

}
void parser::parse_value_definition(){
	match(tok_kw_def);
	match(tok_identifier);
	match(tok_colon);
	parse_type();
	match(tok_op_assignment);
	parse_expression();
	match(tok_semicolon);
}
void parser::parse_function_definition(){
	match(tok_kw_def);
	match(tok_identifier);
	match(tok_lparen);
	parse_parameter_list();
	match(tok_rparen);
	//TODO: Add arrow token
}
void parser::parse_parameter_list(){
	parse_parameter();
	while(lookahead() == tok_comma){
		match(tok_comma);
		parse_parameter();
	}
}
void parser::parse_parameter(){
	match(tok_identifier);
	match(tok_colon);
	parse_type();
}