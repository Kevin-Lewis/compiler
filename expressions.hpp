#pragma once
#include "types.hpp"
#include <vector>

struct expression{

	enum kind{
		cast_kind,
		assign_kind,
		cond_kind,
		u_add_kind,
		u_sub_kind,
		u_not_kind,
		u_ref_kind,
		u_mul_kind,
		bw_and_kind,
		bw_xor_kind,
		bw_or_kind,
		and_kind,
		or_kind,
		bool_kind,
		int_kind,
		float_kind,
		id_kind,
		char_kind,
		postfix_kind,
		b_add_kind,
		b_sub_kind,
		b_mul_kind,
		b_shift_kind,
		b_rel_kind,
		b_eq_kind
	};

	expression(kind k) : k(k), t(){}
	expression(kind k, type* t) : k(k), t(t){}

	bool is_int(){return t->is_int();}
	bool is_float(){return t->is_float();}
	bool is_bool(){return t->is_bool();}
	bool is_ref(){return t-> is_ref();}
	bool is_arithmetic(){return t->is_arithmetic();}
	bool is_numeric(){return t->is_arithmetic();}

	kind k;
	type* t;

};

struct int_expression : expression{int_expression(type* t, int val) : expression(int_kind, t), val(val){} int val;};
struct float_expression : expression{float_expression(type* t, double val) : expression(float_kind, t), val(val){} double val;};
struct bool_expression : expression{bool_expression(type* t, bool val) : expression(bool_kind, t), val(val){} bool val;};
struct char_expression : expression{char_expression(type* t, char val) : expression(char_kind, t), val(val){} char val;};

struct u_add_expression : expression{u_add_expression(expression* e) : expression(u_add_kind), e(e){} expression* e;};
struct u_sub_expression : expression{u_sub_expression(expression* e) : expression(u_sub_kind), e(e){} expression* e;};
struct u_not_expression : expression{u_not_expression(expression* e) : expression(u_not_kind), e(e){} expression* e;};
struct u_ref_expression : expression{u_ref_expression(expression* e) : expression(u_ref_kind), e(e){} expression* e;};
struct u_mul_expression : expression{u_mul_expression(expression* e) : expression(u_mul_kind), e(e){} expression* e;};

struct cast_expression : expression{
	cast_expression(expression* e, type* t) : expression(cast_kind, t), e(e), t(t){} 
	expression* e; 
	type* t;
};

struct b_mul_expression : expression{
	b_mul_expression(type*t, expression* e1, expression* e2) : expression(b_mul_kind, t), left(e1), right(e2){}
	expression* left;
	expression* right;
};

struct b_add_expression : expression{
	b_add_expression(type*t, expression* e1, expression* e2) : expression(b_add_kind, t), left(e1), right(e2){}
	expression* left;
	expression* right;
};

struct b_shift_expression : expression{
	b_shift_expression(type*t, expression* e1, expression* e2) : expression(b_shift_kind, t), left(e1), right(e2){}
	expression* left;
	expression* right;
};

struct b_rel_expression : expression{
	b_rel_expression(type*t, expression* e1, expression* e2) : expression(b_shift_kind, t), left(e1), right(e2){}
	expression* left;
	expression* right;
};

struct b_eq_expression : expression{
	b_eq_expression(type*t, expression* e1, expression* e2) : expression(b_eq_kind, t), left(e1), right(e2){}
	expression* left;
	expression* right;
};

struct bw_and_expression : expression{
	bw_and_expression(type*t, expression* e1, expression* e2) : expression(bw_and_kind, t), left(e1), right(e2){}
	expression* left;
	expression* right;
};

struct bw_or_expression : expression{
	bw_or_expression(type*t, expression* e1, expression* e2) : expression(bw_or_kind, t), left(e1), right(e2){}
	expression* left;
	expression* right;
};

struct bw_xor_expression : expression{
	bw_xor_expression(type*t, expression* e1, expression* e2) : expression(bw_xor_kind, t), left(e1), right(e2){}
	expression* left;
	expression* right;
};

struct and_expression : expression{
	and_expression(type*t, expression* e1, expression* e2) : expression(and_kind, t), left(e1), right(e2){}
	expression* left;
	expression* right;
};

struct or_expression : expression{
	or_expression(type*t, expression* e1, expression* e2) : expression(or_kind, t), left(e1), right(e2){}
	expression* left;
	expression* right;
};

struct conditional_expression : expression{
	conditional_expression(type*t, expression* e1, expression* e2, expression* e3) : expression(cond_kind, t), cond(e1), t(e2), f(e3){}
	expression* cond;
	expression* t;
	expression* f;
};

struct assignment_expression : expression{
assignment_expression(type*t, expression* e1, expression* e2) : expression(assign_kind, t), left(e1), right(e2){}
	expression* left;
	expression* right;
};

struct postfix_expression : expression{
postfix_expression(type*t, expression* e, std::vector<expression*> args) : expression(postfix_kind, t), base(e), args(args){}
	expression* base;
	std::vector<expression*> args;
};
