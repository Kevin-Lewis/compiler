#pragma once

#include "types.hpp"
#include "expressions.hpp"
#include "statements.hpp"
#include <string>
#include <vector>

struct declaration{

	enum kind{
		var_kind,
		const_kind,
		val_kind,
		func_kind,
		param_kind
	};

	declaration(kind k, std::string symbol) : k(k), symbol(symbol){}
	declaration(kind k, std::string symbol, type* t) : k(k), symbol(symbol), t(t){}
	declaration(kind k, std::string symbol, type* t, std::vector<declaration*> d_list) : k(k), symbol(symbol), t(t), d_list(d_list){}
	declaration(kind k, std::string symbol, type* t, expression* e) : k(k), symbol(symbol), t(t), e(e){}

	kind k;
	std::string symbol;
	type* t;
	expression* e;

	std::vector<declaration*> d_list;

};

struct var_declaration : declaration{var_declaration(std::string s, type* t, expression* e = nullptr) 
	: declaration(var_kind, s, t, e){}
};

struct const_declaration : declaration{const_declaration(std::string s, type* t, expression* e = nullptr) 
	: declaration(const_kind, s, t, e){}
};

struct value_declaration : declaration{value_declaration(std::string s, type* t, expression* e = nullptr) 
	: declaration(val_kind, s, t, e){}
};

struct param_declaration : declaration{param_declaration(std::string s, type* t) 
	: declaration(param_kind, s, t){}
};

struct func_declaration : declaration{func_declaration(std::string s, type* t, std::vector<declaration*> d_list, statement* body = nullptr) 
	: declaration(func_kind, s, t, d_list), body(body) {}
	
	statement* body;
};