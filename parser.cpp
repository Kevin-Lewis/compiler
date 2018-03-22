#include "parser.hpp"

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
	parse_bitwise_xor_expression()
	while(lookahead()==tok_op_or_bw){
		accept();
		parse_bitwise_xor_expression()
	}
}
void parser::parse_logical_and_expression(){
	parse_bitwise_or_expression()
	while(lookahead()==tok_kw_and){
		accept();
		parse_bitwise_or_expression()
	}
}
void parser::parse_logical_or_expression(){
	parse_logical_and_expression()
	while(lookahead()==tok_kw_or){
		accept();
		parse_logical_and_expression()
	}
}
void parser::parse_conditional_expression(){
	parse_logical_or_expression();
	if(lookahead() == tok_op_question){
		match(tok_op_question);
		parse_expression();
		match(tok_op_colon);
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

