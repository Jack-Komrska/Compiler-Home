#pragma once
#include <string>
#include "Token.h"


class Symbol
{
private:
	std::string identifier;
	std::string scopeName;
	int type;
	bool isProc;
	bool isGlob;
	bool isArr;

public:
	Symbol();

	std::string getIdentifer();
	std::string getScopeName();
	int getType();
	bool getProcedure();
	bool getGlobal();
	bool getArray();

	void setIdentifier(std::string str);
	void setScopeName(std::string str);
	void setType(int type);
	void setIsProcedure(bool isProc);
	void setIsGlobal(bool isGlob);
	void setIsArr(bool isArr);
	bool operator==(const Symbol& rhs)
	{
		if (*this == rhs)
		{
			return true;
		}
		return false;
	}
};