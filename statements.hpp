#pragma once

#include "expressions.hpp"
#include <vector>

struct statement{

	enum kind{
		block_kind,
		if_kind,
		while_kind,
		return_kind,
		expression_kind
	};

	statement(kind k) : k(k){}

	kind k;
};

struct block_statement : statement{block_statement(std::vector<statement*> statements) 
	: statement(block_kind), statements(statements){}
	
	std::vector<statement*> statements;
};

struct if_statement : statement{if_statement(expression* e, statement* s1, statement* s2) 
	: statement(if_kind), cond(e), t(s1), f(s2){}
	
	expression* cond;
	statement* t;
	statement* f;
};

struct while_statement : statement{while_statement(expression* e, statement* body) 
	: statement(while_kind), cond(e), body(body){}
	
	expression* cond;
	statement* body;
};

struct return_statement : statement{return_statement(expression* e) 
	: statement(return_kind), rvalue(e) {}
	
	expression* rvalue;
};

struct expression_statement : statement{expression_statement(expression* e) 
	: statement(expression_kind), e(e) {}
	
	expression* e;
};