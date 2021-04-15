#pragma once
#include <iostream>
#include "Token.h"
#include "Scanner.h"
#include "SymbolTable.h"
#include <fstream>
#include <string>

class Parser
{
	SymbolTable SymTab;
	Scanner *scanner;
	Token token;
	Scope tempScope;

public:
	Parser();

	void Program();

	void ProgramHeader();

	void ProgramBody();

	void Declaration();

	void ProcedureDeclaration();

	void ProcedureHeader();

	void ProcedureBody();

	Symbol ProcedureCall();

	void ParameterList();

	void Parameter();
	
	void VariableDeclaration();

	void Statement();

	void AssignmentStatement();

	Symbol Destination();

	void IfStatement();

	void LoopStatement();

	void ReturnStatement();

	void Identifier();

	bool TypeMark(definition);

	void Bound();

	Symbol Expression();

	Symbol SubExpression();

	Symbol ArithOp();

	Symbol SubArithOp();

	Symbol Relation();

	Symbol SubRelation();

	Symbol Term();

	Symbol SubTerm();

	Symbol Factor();

	Symbol Name();

	void ArgumentList();

	Symbol Number(); //returns an int or double

	Symbol String(); //yet again, returns a String most likely and not a number

};