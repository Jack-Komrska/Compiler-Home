#pragma once
#include "Symbol.h"
#include "Scope.h"
#include <vector>

class SymbolTable
{
private:
	std::vector<Scope> symTab;
	//std::vector<Scope>::iterator iterSym;
	int scopeLoc = 0;
public:
	SymbolTable();
	void AddScope(Scope scope); //add a scope to the symbol table
	Scope GetScope(std::string str); //gets the scope of a symbol
	void InsertSymbol(Symbol symbol); //insert a symbol
	bool LookupSymbol(std::string str); //check if a symbol exists
	Symbol FindSymbol(std::string str); //return a symbol
	int getScopeLoc();
};