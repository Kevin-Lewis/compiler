#pragma once

struct type{

	enum kind{
		bool_kind,
		int_kind,
		float_kind,
		char_kind,
		ptr_kind,
		ref_kind
	};

	type(kind k) : k(k){}

	kind k;

};

struct bool_type : type{bool_type() : type(bool_kind){}};
struct int_type : type{int_type() : type(int_kind){}};
struct float_type : type{float_type() : type(float_kind){}};
struct char_type : type{char_type() : type(char_kind){}};
struct ptr_type : type{ptr_type(type* t) : type(ptr_kind), ptype(t){} type* ptype;};
struct ref_type : type{ref_type(type* t) : type(ref_kind), rtype(t){} type* rtype;};