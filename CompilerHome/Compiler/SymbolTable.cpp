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
	return Scope();
}

void SymbolTable::InsertSymbol(Symbol symbol) //Inserts the symbol at the correct scope of the symbol table
{
	Scope tempScope = GetScope(symbol.getScopeName());

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

Symbol SymbolTable::FindSymbol(std::string str, Scope scope)
{
	for (int i = 0; i < symTab.at(GetScope(scope.name).scopeLoc).symbols.size(); i++)
	{
		if (symTab.at(GetScope(scope.name).scopeLoc).symbols.at(i).getIdentifer() == str)
		{
			return symTab.at(GetScope(scope.name).scopeLoc).symbols.at(i);
		}
	}

	return Symbol();
}

Scope SymbolTable::FindScope(int scp)
{
	return symTab.at(scp);
}