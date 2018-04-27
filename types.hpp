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

	bool is_int(){return k == int_kind;}
	bool is_float(){return k == float_kind;}
	bool is_char(){return k == char_kind;}
	bool is_bool(){return k == bool_kind;}
	bool is_ptr(){return k == ptr_kind;}
	bool is_ref(){return k == ref_kind;}

	kind k;

};

struct bool_type : type{bool_type() : type(bool_kind){}};
struct int_type : type{int_type() : type(int_kind){}};
struct float_type : type{float_type() : type(float_kind){}};
struct char_type : type{char_type() : type(char_kind){}};
struct ptr_type : type{ptr_type(type* t) : type(ptr_kind), ptype(t){} type* ptype;};
struct ref_type : type{ref_type(type* t) : type(ref_kind), rtype(t){} type* rtype;};