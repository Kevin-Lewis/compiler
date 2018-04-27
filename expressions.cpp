#include "expressions.hpp"


expression* expression::convertExpression(expression* original, type* newType){
	expression* e = original;
	type* currentType = original->t;
	switch(currentType->k){

		case type::ref_kind: //convert to value
			return new converted_expression(original, newType);

		case type::int_kind: //convert to bool or float
			if(newType->k == type::bool_kind){
				return new converted_expression(original, newType);
			}
			else if(newType->k == type::float_kind){
				return new converted_expression(original, newType);
			}								

		case type::float_kind: //convert to bool or int
			if(newType->k == type::bool_kind){
				return new converted_expression(original, newType);
			}
			else if(newType->k == type::float_kind){
				return new converted_expression(original, newType);
			}		

		case type::bool_kind: //convert to int
			if(newType->k == type::int_kind){
				return new converted_expression(original, newType);
			}

		case type::ptr_kind: //convert to value
			return new converted_expression(original, newType);
		
		case type::char_kind: //convert to bool or int
			if(newType->k == type::bool_kind){
				return new converted_expression(original, newType);
			}
			else if(newType->k == type::int_kind){
				return new converted_expression(original, newType);
			}	

		default:
			return e;
	}
}