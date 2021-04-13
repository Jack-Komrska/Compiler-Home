#pragma once
#include "Symbol.h"
#include "Scope.h"
#include <vector>
//#include "Parser.h"


class SymbolTable
{
private:
	std::vector<Scope> symTab;
	//std::vector<Scope>::iterator iterSym;
	int scopeLoc = 0;
public:
	SymbolTable();
	void AddScope(Scope scope); //add a scope to the symbol table
	Scope GetScope(std::string str); //gets the scope
	void InsertSymbol(Symbol symbol); //insert a symbol
	bool LookupSymbol(Symbol symbol); //check if a symbol exists
	Symbol FindSymbol(Symbol symbol); //return a symbol

};