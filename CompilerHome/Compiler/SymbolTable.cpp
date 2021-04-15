#pragma once
#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
	//iterSym = symTab.begin();
}

int SymbolTable::getScopeLoc() 
{
	return scopeLoc;
}

void SymbolTable::AddScope(Scope scope)
{
	symTab.push_back(scope);
	scopeLoc++;
}

Scope SymbolTable::GetScope(std::string str)
{
	for (int i = 0; i < symTab.size(); i++)
	{
		if (symTab.at(i).name == str)
		{
			return symTab.at(i);
		}
	}
}

void SymbolTable::InsertSymbol(Symbol symbol) //Inserts the symbol at the correct scope of the symbol table
{
	Scope tempScope = GetScope(symbol.getScopeName());

	/*int loc;
	for (int i = 0; i < symTab.size(); i++)
	{
		if (tempScope == symTab.at(i))
		{
			loc = i;
		}
	}*/

	symTab[tempScope.scopeLoc].symbols.push_back(symbol);
}

bool SymbolTable::LookupSymbol(std::string str)
{
	for (int i = 0; i < symTab.size(); i++)
	{
		for (int j = 0; j < symTab.at(i).symbols.size(); j++)
		{
			if (symTab.at(i).symbols.at(j).getIdentifer() == str)
			{
				return true;
			}
		}

	}

	return false;
}

Symbol SymbolTable::FindSymbol(std::string str)
{
	/*Scope tempScope = GetScope(str);

	for(int i = 0; i < symTab.at(tempScope.scopeLoc).symbols.size(); i++)
	{
		if (str == symTab.at(tempScope.scopeLoc).symbols.at(i).getIdentifer())
		{
			return symTab.at(tempScope.scopeLoc).symbols.at(i);
		}
	}
	*/
	for (int i = 0; i < symTab.size(); i++)
	{
		for (int j = 0; j < symTab.at(i).symbols.size(); j++)
		{
			if (symTab.at(i).symbols.at(j).getIdentifer() == str)
			{
				return symTab.at(i).symbols.at(j);
			}
		}

	}

	//return empty symbol if we cannot find it
	return Symbol();
}