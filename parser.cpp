#include "parser.hpp"
#include "scope.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

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

bool parser::compareType(type* t1, type* t2){
	if(t1->k != t2->k){throw std::runtime_error("incompatible types");}
	else{return true;}
	//TODO - Check pointer object types
}

void parser::match(token_name t){
	if(lookahead() == t){std::cout << tokens.front().toString(); accept();}
	else{throw std::runtime_error("Syntax error.");}
}

void parser::enter_scope(scope::kind k){

	if(k == scope::global){current_scope = new scope(k, nullptr);}
	else{current_scope = new scope(k,current_scope);}
}

void parser::leave_scope(){
		scope* temp = current_scope;
		current_scope = temp->parent;
		delete temp;
	}

void parser::declare_in_scope(declaration* d){
	if(current_scope->search(d->symbol)){
		std::stringstream ss;
		ss << "redeclaration of " << d->symbol;
		throw std::runtime_error(ss.str());
	}
	current_scope -> declare(d->symbol, d);
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
	std::vector<expression*> args;
	switch(lookahead()){
		case tok_lparen:
			args = parse_argument_list();
			match(tok_rparen);
			return new postfix_expression(e->t,e,args);
		case tok_lbracket:
			//TODO - Maybe add index support
			args = parse_argument_list();
			match(tok_rbracket);
			return new postfix_expression(e->t,e,args);
			break;
		default: 
			break;
	}
	return e;
}
std::vector<expression*> parser::parse_argument_list(){
	std::vector<expression*> args;
	expression* e = parse_argument();
	args.push_back(e);
	while(lookahead() == tok_comma){
		args.push_back(parse_argument());
	}
	return args;
}
expression* parser::parse_argument(){return parse_expression();}
expression* parser::parse_unary_expression(){
	expression* e;
	switch(lookahead()){
		case tok_op_plus:
			match(tok_op_plus);
			e = parse_postfix_expression();
			if(!e -> is_arithmetic()){throw std::runtime_error("expected expression of type arithmetic");}
			return new u_add_expression(e);

		case tok_op_minus:
			match(tok_op_minus);
			e = parse_postfix_expression();
			if(!e -> is_arithmetic()){throw std::runtime_error("expected expression of type arithmetic");}
			return new u_sub_expression(e);

		case tok_op_not_bw:
			match(tok_op_not_bw);
			e = parse_postfix_expression();
			return new u_not_expression(e);
		
		case tok_op_and_bw: 
			match(tok_op_and_bw);
			e = parse_postfix_expression();
			if(!e -> is_ref()){throw std::runtime_error("expected expression of type reference");}
			return new u_ref_expression(e);
		
		case tok_op_mul:
			match(tok_op_mul);
			e = parse_postfix_expression();
			if(!e -> is_ref()){throw std::runtime_error("expected expression of type reference");}
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
		e = e->convertExpression(e,t);
		return new cast_expression(e,t);
	}
	return e;
}
expression* parser::parse_multiplicative_expression(){
	expression* e1 = parse_cast_expression();
	if(!e1 -> is_arithmetic()){throw std::runtime_error("expected expression of type arithmetic");}
	while(lookahead()==tok_op_mul || lookahead()==tok_op_div || lookahead()==tok_op_mod){
		accept();
		expression* e2 = parse_cast_expression();
		if(!e2 -> is_arithmetic()){throw std::runtime_error("expected expression of type arithmetic");}
		e1 = new b_mul_expression(e1->t,e1,e2);
		compareType(e1->t,e2->t);
	}
	return e1;
}
expression* parser::parse_additive_expression(){
	expression* e1 = parse_multiplicative_expression();
	if(!e1 -> is_arithmetic()){throw std::runtime_error("expected expression of type arithmetic");}
	while(lookahead()==tok_op_plus || lookahead()==tok_op_minus){
		accept();
		expression* e2 = parse_multiplicative_expression();
		if(!e2 -> is_arithmetic()){throw std::runtime_error("expected expression of type arithmetic");}
		e1 = new b_add_expression(e1->t,e1,e2);
		compareType(e1->t,e2->t);
	}
	return e1;
}
expression* parser::parse_shift_expression(){
	expression* e1 = parse_additive_expression();
	if(!e1 -> is_int()){throw std::runtime_error("expected expression of type integer");}
	//TODO - add shift operators
	while(lookahead()==tok_op_plus || lookahead()==tok_op_minus){
		accept();
		expression* e2 = parse_additive_expression();
		if(!e2 -> is_int()){throw std::runtime_error("expected expression of type integer");}
		e1 = new b_shift_expression(e1->t,e1,e2);
	}
	return e1;
}
expression* parser::parse_relational_expression(){
	expression* e1 = parse_shift_expression();
	if(!e1 -> is_int()){throw std::runtime_error("expected expression of type numeric");}
	e1 -> op = lookahead();
	while(lookahead()==tok_op_gt || lookahead()==tok_op_lt || lookahead()==tok_op_gte || lookahead() == tok_op_lte){
		accept();
		expression* e2 = parse_shift_expression();
		e2->op = lookahead();
		if(!e2 -> is_int()){throw std::runtime_error("expected expression of type numeric");}
		e1 = new b_rel_expression(e1->t,e1,e2);
	}
	return e1;
}
expression* parser::parse_equality_expression(){
	expression* e1 = parse_relational_expression();
	while(lookahead()==tok_op_eq || lookahead()==tok_op_neq){
		accept();
		expression* e2 = parse_relational_expression();
		e1 = new b_eq_expression(e1->t,e1,e2);
	}
	return e1;
}
expression* parser::parse_bitwise_and_expression(){
	expression* e1 = parse_relational_expression();
	if(!e1 -> is_int()){throw std::runtime_error("expected expression of type integer");}
	while(lookahead()==tok_op_and_bw){
		accept();
		expression* e2 = parse_relational_expression();
		if(!e2 -> is_int()){throw std::runtime_error("expected expression of type integer");}
		e1 = new bw_and_expression(e1->t,e1,e2);
	}
	return e1;
}
expression* parser::parse_bitwise_xor_expression(){
	expression* e1 = parse_bitwise_and_expression();
	if(!e1 -> is_int()){throw std::runtime_error("expected expression of type integer");}
	while(lookahead()==tok_op_xor_bw){
		accept();
		expression* e2 = parse_bitwise_and_expression();
		if(!e2 -> is_int()){throw std::runtime_error("expected expression of type integer");}
		e1 = new bw_xor_expression(e1->t,e1,e2);
	}
	return e1;
}
expression* parser::parse_bitwise_or_expression(){
	expression* e1 = parse_bitwise_xor_expression();
	if(!e1 -> is_int()){throw std::runtime_error("expected expression of type integer");}
	while(lookahead()==tok_op_or_bw){
		accept();
		expression* e2 = parse_bitwise_xor_expression();
		if(!e2 -> is_int()){throw std::runtime_error("expected expression of type integer");}
		e1 = new bw_or_expression(e1->t,e1,e2);
	}
	return e1;
}
expression* parser::parse_logical_and_expression(){
	expression* e1 = parse_bitwise_or_expression();
	if(!e1 -> is_bool()){throw std::runtime_error("expected expression of type bool");}
	while(lookahead()==tok_kw_and){
		accept();
		expression* e2 = parse_bitwise_or_expression();
		if(!e2 -> is_bool()){throw std::runtime_error("expected expression of type bool");}
		e1 = new and_expression(e1->t,e1,e2);
	}
	return e1;
}
expression* parser::parse_logical_or_expression(){
	expression* e1 = parse_logical_and_expression();
	if(!e1 -> is_bool()){throw std::runtime_error("expected expression of type bool");}
	while(lookahead()==tok_kw_or){
		accept();
		expression* e2 = parse_logical_and_expression();
		if(!e2 -> is_bool()){throw std::runtime_error("expected expression of type bool");}
		e1 = new bw_or_expression(e1->t,e1,e2);
	}
	return e1;
}
expression* parser::parse_conditional_expression(){
	expression* e1 = parse_logical_or_expression();
	if(!e1 -> is_bool()){throw std::runtime_error("expected expression of type bool");}
	if(lookahead() == tok_op_question){
		match(tok_op_question);
		expression* e2 = parse_expression();
		match(tok_colon);
		expression* e3 = parse_conditional_expression();
		return new conditional_expression(e1->t,e1,e2,e3);
	}
	return e1;
}
expression* parser::parse_assignment_expression(){
	expression* e1 = parse_conditional_expression();
	if(!e1 -> is_ref()){throw std::runtime_error("expected expression of type reference");}
	if(lookahead() == tok_op_assignment){
		match(tok_op_assignment);
		type* t = e1->t;
		expression* e2 = parse_assignment_expression();
		if(e2->t->k == type::ref_kind || e2->t->k == type::ptr_kind){e2 = e2->convertExpression(e2,e2->t->otype);}
		compareType(e1->t, e2->t);
		return new assignment_expression(e1->t,e1,e2);
	}
	return e1;
}
expression* parser::parse_constant_expresssion(){return parse_conditional_expression();}

//statement parsing
statement* parser::parse_statement(){
	statement* s;
	switch(lookahead()){
		case tok_lbrace:
			s = parse_block_statement();
			break;
		case tok_kw_if:
			s = parse_if_statement();
			break;
		case tok_kw_while:
			s = parse_while_statement();
			break;
		case tok_kw_return:
			s = parse_return_statement();
			break;
		default:
			break;
		//TODO: Add break and continue tokens
	}
	return s;
}
statement* parser::parse_block_statement(){
	std::vector<statement*> ss;
	if(lookahead() == tok_lbrace){
		match(tok_lbrace);
		enter_scope(scope::block);
		ss = parse_statement_seq();
		leave_scope();
		match(tok_rbrace);
	}
	return new block_statement(ss);
}
std::vector<statement*> parser::parse_statement_seq(){
	std::vector<statement*> ss;
	while(lookahead() != tok_rbrace){
		statement* s = parse_statement();
		ss.push_back(s);
	}
	return ss;
}
statement* parser::parse_if_statement(){
	match(tok_kw_if);
	match(tok_lparen);
	expression* e = parse_expression();
	match(tok_rparen);
	statement* s1 = parse_statement();
	match(tok_kw_else);
	statement* s2 = parse_statement();
	return new if_statement(e,s1,s2);
}
statement* parser::parse_while_statement(){
	match(tok_kw_while);
	match(tok_lparen);
	expression* e = parse_expression();
	match(tok_rparen);
	statement* s = parse_statement();
	return new while_statement(e,s);
}
//statement* parser::parse_break_statement(){break;}
//statement* parser::parse_continue_statement(){continue;}
statement* parser::parse_return_statement(){
	expression* e;
	match(tok_kw_return);
	if(lookahead() != tok_semicolon){e = parse_expression();}
	match(tok_semicolon);
	return new return_statement(e);
}
//statement* parser::parse_declaration_statement(){parse_local_declaration();}
statement* parser::parse_expression_statement(){
	expression* e = parse_expression();
	match(tok_semicolon);
	return new expression_statement(e);
}

//declaration parsing
declaration* parser::parse_program(){
	enter_scope(scope::global);
	declaration* d;
	while(!tokens.empty()){
		d = parse_declaration_seq();
	}
	leave_scope();
	return d;
}
declaration* parser::parse_declaration_seq(){
	//TODO - Acually parse declaration sequence
	declaration* d = parse_declaration();
	return d;
}
declaration* parser::parse_declaration(){
	declaration* d;
	switch(lookahead(2)){
		case tok_colon:
			d = parse_object_definition();
			break;
		case tok_lparen:
			d = parse_function_definition();
			break;
		default:
			break;
	}
	return d;
}
declaration* parser::parse_local_declaration(){return parse_object_definition();}
declaration* parser::parse_object_definition(){
	declaration* d = nullptr;
	switch(lookahead()){
		case tok_kw_var:
			d = parse_variable_definition();
			break;
		case tok_kw_let:
			d = parse_constant_definition();
			break;
		case tok_kw_def:
			d = parse_value_definition();
			break;
		default:
			break;
	}
	return d;
}
declaration* parser::parse_variable_definition(){
	expression* e = nullptr;
	match(tok_kw_var);
	token tok = tokens.front();
	match(tok_identifier);
	match(tok_colon);
	type* t = parse_type();
	if(lookahead() != tok_semicolon){
		match(tok_op_assignment);
		e = parse_expression();
	}
	match(tok_semicolon);
	declaration* d = new var_declaration(tok.attr.name,t,e);
	declare_in_scope(d);
	return d;
}
declaration* parser::parse_constant_definition(){
	match(tok_kw_let);
	token tok = tokens.front();
	match(tok_identifier);
	match(tok_colon);
	type* t = parse_type();
	match(tok_op_assignment);
	expression* e = parse_expression();
	match(tok_semicolon);
	declaration* d = new const_declaration(tok.attr.name,t,e);
	declare_in_scope(d);
	return d;
}
declaration* parser::parse_value_definition(){
	match(tok_kw_def);
	token tok = tokens.front();
	match(tok_identifier);
	match(tok_colon);
	type* t = parse_type();
	match(tok_op_assignment);
	expression* e = parse_expression();
	match(tok_semicolon);
	declaration* d = new value_declaration(tok.attr.name,t,e);
	declare_in_scope(d);
	return d;
}
declaration* parser::parse_function_definition(){
	match(tok_kw_def);
	token tok = tokens.front();
	match(tok_identifier);
	match(tok_lparen);
	enter_scope(scope::parameter);
	std::vector<declaration*> decs = parse_parameter_list();
	leave_scope();
	match(tok_rparen);
	statement* s = parse_block_statement();
	//TODO: Add arrow token
	type* t = parse_type();
	declaration* d = new func_declaration(tok.attr.name,t,decs,s);
	declare_in_scope(d);
	return d;
}
std::vector<declaration*> parser::parse_parameter_list(){
	declaration* d;
	d -> d_list.push_back(parse_parameter());
	while(lookahead() == tok_comma){
		match(tok_comma);
		d -> d_list.push_back(parse_parameter());
	}
	return d -> d_list;

}
declaration* parser::parse_parameter(){
	token tok = tokens.front();
	match(tok_identifier);
	match(tok_colon);
	type* t = parse_type();
	declaration* d = new param_declaration(tok.attr.name,t);
	declare_in_scope(d);
	return d;
}
