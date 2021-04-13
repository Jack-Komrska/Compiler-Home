#pragma once
#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
	//iterSym = symTab.begin();
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

bool SymbolTable::LookupSymbol(Symbol symbol)
{
	Scope tempScope = GetScope(symbol.getScopeName());

	if (std::find(symTab.at(tempScope.scopeLoc).symbols.begin(), symTab.at(tempScope.scopeLoc).symbols.end(), symbol) != symTab.at(tempScope.scopeLoc).symbols.end())
	{
		return true;
	}
}

Symbol SymbolTable::FindSymbol(Symbol symbol)
{
	Scope tempScope = GetScope(symbol.getScopeName());

	for (int i = 0; i < symTab.at(tempScope.scopeLoc).symbols.size(); i++)
	{
		if (symbol == symTab.at(tempScope.scopeLoc).symbols.at(i))
		{
			return symTab.at(tempScope.scopeLoc).symbols.at(i);
		}
	}
	//return empty symbol if we cannot find it
	Symbol badSymbol;
	return badSymbol;
}