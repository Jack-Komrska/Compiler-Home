#pragma once
#include <iostream>
#include "Token.h"
#include "Scanner.h"
#include <fstream>
#include <string>

class Parser
{
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

	void Expression();

	void ArithOp();

	void Relation();//may return the actual relation and not be void

	void Term();

	void Factor();

	void Name();

	void ArugmentList();

	void Number(); //returns an int or double

	void String(); //yet again, returns a String most likely and not a number

};