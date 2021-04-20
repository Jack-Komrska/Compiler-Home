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
	Scope tempScope; //the current scope of the parsing
	//bool isChild = false;
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

	Symbol Expression(definition &expressionType, Symbol arith = Symbol());

	Symbol ArithOp(definition &arithType, Symbol relation = Symbol());

	Symbol Relation(definition &relationType, Symbol term = Symbol());

	bool isRelation(definition relation);

	Symbol Term(definition &termType, Symbol factor = Symbol());

	Symbol Factor(definition &termType);

	Symbol Name();

	void ArgumentList();

	Symbol Number(); //returns an int or double

	Symbol String(); //yet again, returns a String most likely and not a number

	bool ValidTypesExpression(definition& returnDef, int lhs, int rhs);

	bool ValidTypesAssignment(definition& returnDef, int lhs, int rhs);

	definition MapVariableToLiteral(int def);

	void BuiltInFunctionDeclarations();
};