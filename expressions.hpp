#pragma once
#include "types.hpp"

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
		mul_kind,
		add_kind,
		shift_kind,
		rel_kind,
		eq_kind,
		bw_and_kind,
		bw_xor_kind,
		bw_or_kind,
		and_kind,
		or_kind,
		bool_kind,
		int_kind,
		float_kind,
		id_kind,
		char_kind

	};

	expression(kind k) : k(k), t(){}
	expression(kind k, type* t) : k(k), t(t){}

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

struct cast_expression : expression{cast_expression(expression* e, type* t) : expression(cast_kind, t), e(e), t(t){} expression* e; type* t;};