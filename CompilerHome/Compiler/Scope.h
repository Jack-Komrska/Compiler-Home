#pragma once
#include <string>
#include <vector>
#include "Symbol.h"

class Scope
{
private:
	
public:
	std::string name;
	std::vector<Symbol> symbols;
	int scopeLoc;
	bool operator==(const Scope &rhs)
	{
		if (*this == rhs)
		{
			return true;
		}
		return false;
	}
};

