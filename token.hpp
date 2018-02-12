#include <string>

enum token_name{

	//punctuators
	tok_lbrace,
	tok_rbrace,
	tok_lparen,
	tok_rparen,
	tok_lbracket,
	tok_rbracket,
	tok_comma,
	tok_semicolon,
	tok_colon,

	//relational operators
	tok_op_eq,
	tok_op_neq,
	tok_op_lt,
	tok_op_gt,
	tok_op_lte,
	tok_op_gte,

	//arithmetic operators
	tok_op_plus,
	tok_op_minus,
	tok_op_mul,
	tok_op_div,
	tok_op_mod,

	//bitwise operators
	tok_op_and_bw,
	tok_op_or_bw,
	tok_op_xor_bw,
	tok_op_not_bw,

	//logical operators
	tok_op_and,
	tok_op_or,
	tok_op_not,
	tok_op_tilde,

	//conditional operator
	tok_op_question,
	tok_op_assignment,

	//keywords
	tok_kw_and,
	tok_kw_or,
	tok_kw_not,
	tok_kw_let,
	tok_kw_bool,
	tok_kw_char,
	tok_kw_if,
	tok_kw_else,
	tok_kw_true,
	tok_kw_false,
	tok_kw_var,
	tok_kw_def,
	tok_kw_int,
	tok_kw_float,

	//identifiers
	tok_identifier,

	//integer literals
	tok_decimal_integer_literal,
	tok_hexadecimal_digit,
	tok_hexadecimal_integer_literal,
	tok_binary_digit,
	tok_binary_integer_literal,

	//floating point literals
	tok_floating_point_literal,
	tok_fraction,
	tok_exponent,

	//boolean literals
	boolean_literal,

	//character literals
	tok_character_character,
	tok_escape_sequence,
	tok_character_literal,

	//string literals
	tok_string_character,
	tok_string_literal

};

struct token_attribute{
	int value;
	std::string name;

	token_attribute(){}
	token_attribute(int value) : value(value){}
	token_attribute(std::string str) : name(str){}
};

struct token{
	token_name name;

	token_attribute attr;

	token(const token_name n) : name(n) {}

	token(const token_name n, std::string str);

	token(const token_name n, int value);

	//returns token information as string
	std::string toString();

};