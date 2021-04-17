#pragma once
#include <string>
#include "Token.h"
#include <vector>

class Symbol
{
private:
	std::string identifier;
	std::string scopeName; 
	int type;
	bool isProc;
	bool isGlob;
	bool isArr;
	std::vector<Symbol> children;

public:
	Symbol();

	void printTree();

	std::string getIdentifer();
	std::string getScopeName();
	int getType();
	bool getProcedure();
	bool getGlobal();
	bool getArray();
	std::vector<Symbol> getChildren();

	void setIdentifier(std::string str);
	void setScopeName(std::string str);
	void setType(int type);
	void setIsProcedure(bool isProc);
	void setIsGlobal(bool isGlob);
	void setIsArr(bool isArr);
	void addChild(Symbol child);
	bool operator==(const Symbol& rhs)
	{
		if (*this == rhs)
		{
			return true;
		}
		return false;
	}
};