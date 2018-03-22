#include "token.hpp"
#include <string>
#include <cctype>

std::string token::toString(){
	switch (name)
	{
		case tok_lbrace:	return "<left-brace>";
		case tok_rbrace:	return "<right-brace>";
		case tok_lparen:	return "<left-parenthesis>";
		case tok_rparen:	return "<right-parenthesis>";
		case tok_lbracket:	return "<left-bracket>";
		case tok_rbracket:	return "<right-bracket>";
		case tok_comma:	return "<comma>";
		case tok_semicolon:	return "<semicolon>";
		case tok_colon:	return "<colon>";

		case tok_op_eq:	return "<relational-operator:equals>";
		case tok_op_neq:	return "<relational-operator:notequals>";
		case tok_op_lt:	return "<relational-operator:lt";
		case tok_op_gt:	return "<relational-operator:gt>";
		case tok_op_lte:	return "<relational-operator:lt_equals>";
		case tok_op_gte:	return "<relational-operator:gt_equals>";

		case tok_op_plus:	return "<arithmetic-operator:add>";
		case tok_op_minus:	return "<arithmetic-operator:sub>";
		case tok_op_mul:	return "<arithmetic-operator:multiply>";
		case tok_op_div:	return "<arithmetic-operator:divide>";
		case tok_op_mod:	return "<arithmetic-operator:mod>";

		case tok_op_and_bw:	return "<bitwise-operator:add>";
		case tok_op_or_bw:	return "<bitwise-operator:or>";
		case tok_op_xor_bw:	return "<bitwise-operator:xor>";
		case tok_op_not_bw:	return "<bitwise-operator:not>";

		case tok_op_and:	return "<logical-operator:add>";
		case tok_op_or:		return "<logical-operator:or>";
		case tok_op_not:	return "<logical-operator:not>";
		case tok_op_tilde:	return "<logical-operator:tilde>";

		case tok_op_question:	return "<conditional-operator:question>";
		case tok_op_assignment:	return "<conditional-operator:assignment>";

		case tok_identifier:	return "<identifier:" + attr.name + ">";

		case tok_decimal_integer_literal: 	return "<decimal_integer_literal:" + std::to_string(attr.value) + ">";
		case tok_hexadecimal_integer_literal: return "<hexadecimal_integer_literal:" + std::to_string(attr.value) + ">";
		case tok_binary_integer_literal: 	return "<binary_integer_literal:" + std::to_string(attr.value) + ">";

		case tok_floating_point_literal: 	return "<floating_point_literal:" + std::to_string(attr.floating) + ">";

		case tok_string_literal:	return "<string_literal:" + attr.name + ">";

		case tok_character_literal:	return "<character_literal:" + attr.name + ">";

		case tok_kw_and: return "<keyword:" + attr.name + ">";
		case tok_kw_or: return "<keyword:" + attr.name + ">";
		case tok_kw_not: return "<keyword:" + attr.name + ">";
		case tok_kw_let: return "<keyword:" + attr.name + ">";
		case tok_kw_bool: return "<keyword:" + attr.name + ">";
		case tok_kw_char: return "<keyword:" + attr.name + ">";
		case tok_kw_if: return "<keyword:" + attr.name + ">";
		case tok_kw_else: return "<keyword:" + attr.name + ">";
		case tok_kw_true: return "<keyword:" + attr.name + ">";
		case tok_kw_false: return "<keyword:" + attr.name + ">";
		case tok_kw_var: return "<keyword:" + attr.name + ">";
		case tok_kw_def: return "<keyword:" + attr.name + ">";
		case tok_kw_int: return "<keyword:" + attr.name + ">";
		case tok_kw_float: return "<keyword:" + attr.name + ">";
		case tok_kw_void: return "<keyword:" + attr.name + ">";
		case tok_kw_const: return "<keyword:" + attr.name + ">";
		case tok_kw_as: return "<keyword:" + attr.name + ">";
		case tok_kw_while: return "<keyword:" + attr.name + ">";
		case tok_kw_return: return "<keyword:" + attr.name + ">";

		default:	return "";
	}
}

token::token(const token_name n, std::string str){
	name = n;
	attr = token_attribute(str);
}

token::token(const token_name n, int value){
	name = n;
	attr = token_attribute(value);
}

token::token(const token_name n, double floating){
	name = n;
	attr = token_attribute(floating);
}