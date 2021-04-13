#pragma once
#include <iostream>
#include "Token.h"
#include "Scanner.h"
#include "SymbolTable.h"
#include <fstream>
#include <string>

class Parser
{
	//SymbolTable SymTab;
	Scanner *scanner;
	Token token;

public:
	Parser();

	void Program();

	void ProgramHeader();

	void ProgramBody();

	void Declaration();

	void ProcedureDeclaration();

	void ProcedureHeader();

	void ProcedureBody();

	void ProcedureCall();

	void ParameterList();

	void Parameter();
	
	void VariableDeclaration();

	void Statement();

	void AssignmentStatement();

	void Destination();

	void IfStatement();

	void LoopStatement();

	void ReturnStatement();

	void Identifier();

	bool TypeMark(definition);

	void Bound();

	void Expression();

	void SubExpression();

	void ArithOp();

	void SubArithOp();

	void Relation();

	void SubRelation();

	void Term();

	void SubTerm();

	void Factor();

	void Name();

	void ArugmentList();

	void Number(); //returns an int or double

	void String(); //yet again, returns a String most likely and not a number

};