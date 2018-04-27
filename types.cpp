#include "types.hpp"

bool type::compareType(type* t1, type* t2){
	if(t1->k != t2->k){return false;}
	else{return true;}
	//TODO - Check pointer object types
}