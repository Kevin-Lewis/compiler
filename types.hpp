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

	type(kind k, type* otype = nullptr) : k(k), otype(otype){}

	bool is_int(){return k == int_kind;}
	bool is_float(){return k == float_kind;}
	bool is_char(){return k == char_kind;}
	bool is_bool(){return k == bool_kind;}
	bool is_ptr(){return k == ptr_kind;}
	bool is_ref(){return k == ref_kind;}
	bool is_arithmetic(){return k == int_kind || k == float_kind;}
	bool is_numeric(){return k == bool_kind || k == int_kind || k == float_kind || k == char_kind;}

	kind k;

	type* otype;

};

struct bool_type : type{bool_type() : type(bool_kind){}};
struct int_type : type{int_type() : type(int_kind){}};
struct float_type : type{float_type() : type(float_kind){}};
struct char_type : type{char_type() : type(char_kind){}};
struct ptr_type : type{ptr_type(type* t) : type(ptr_kind, t){}};
struct ref_type : type{ref_type(type* t) : type(ref_kind, t){}};