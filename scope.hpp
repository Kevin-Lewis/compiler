#pragma once

#include <unordered_map>
#include <string>
#include <cassert>
#include "declarations.hpp"

struct scope : std::unordered_map<std::string, declaration*>{

	enum kind{
		global,
		block,
		parameter
	};

	scope(kind k, scope* parent = nullptr) : k(k), parent(parent){}

	scope* parent;
	kind k;

	declaration* search(std::string symbol){
		auto iter = find(symbol);
		return iter == end() ? nullptr : iter -> second;
	}

	void declare(std::string symbol, declaration* d){
		assert(count(symbol) == 0);
		emplace(symbol,d);
	}
};