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

void parser::parse_type(){

}