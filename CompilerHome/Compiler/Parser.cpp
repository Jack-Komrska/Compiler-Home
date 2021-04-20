#pragma once
#include "Parser.h"
#include "Scanner.h"
#include <iostream>

Parser::Parser(std::string fName)
{
	scanner = new Scanner(fName); //pass the file name to the scanner

	Program();

	scanner->closeFile();
}

void Parser::Program()
{
	Scope global; //the default local scope
	global.name = "global";
	global.scopeLoc = SymTab.getScopeLoc();
	SymTab.AddScope(global); //adding the global scope at the very beginning
	tempScope.name = "global";

	BuiltInFunctionDeclarations(); //adds the bulit in functions to the global scope

	ProgramHeader(); //program builds out from the project language document

	ProgramBody();

	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == key_period)
	{
		token = scanner->CallScanner(true);

		std::cout << "End of program.\n";
	}
	else
	{
		std::cout << "ERROR: Missing perdiod '.', and the end of program on line: " << scanner->getLineNum() << std::endl;
	}
}

void Parser::ProgramHeader()
{
	Token tempToken = scanner->CallScanner(false);
	// for almost every single check, we check if the peeked token is what we need, and then we scan it in
	if (tempToken.type == key_program)
	{
		token = scanner->CallScanner(true);

		tempToken = scanner->CallScanner(false);

		if (tempToken.type == key_progName)
		{
			token = scanner->CallScanner(true);

			tempToken = scanner->CallScanner(false);

			if (tempToken.type == key_is)
			{
				token = scanner->CallScanner(true);

				return;
			}
			else
			{
				std::cout << "ERROR, finding the key 'is', in program.\n";
				//return; //need to not run the body
			}
		}
		else
		{
			std::cout << "ERROR, finding the program name, in program on line: " << scanner->getLineNum() << std::endl;
		}
	}
	else
	{
		std::cout << "ERROR, finding the key 'program', in program on line: " << scanner->getLineNum() << std::endl;
	}
}

void Parser::ProgramBody() //contains declarations and statements
{
	Declaration();
	
	token = scanner->CallScanner(true); //scan in begin

	Token tempToken = scanner->CallScanner(false);
	tempScope.name = "global";
	while (tempToken.type != key_end) 
	{
		Statement(); //calls statemnet repeatedly until end is scanned
		
		tempToken = scanner->CallScanner(false);
	}

	if (tempToken.type == key_end)
	{
		//std::cout << "This should be end\t"; //uncomment the beginning of the line to check if the end lines up
		token = scanner->CallScanner(true);

		tempToken = scanner->CallScanner(false);
		
		if (tempToken.type == key_program)
		{
			//std::cout << "This should be program\t"; 
			token = scanner->CallScanner(true);
			return;
		}
		else
		{
			std::cout << "ERROR, finding the key 'program', in program on line: " << scanner->getLineNum() << std::endl;
		}
	}
	else
	{
		std::cout << "ERROR, finding the key 'end', in program on line: " << scanner->getLineNum() << std::endl;
	}
}

void Parser::Declaration() //calls either procedure/variable declaration
{
	while (token.type != key_begin) 
	{
		Token tempToken = scanner->CallScanner(false); //scans in the scope of variable/procedure

		if (tempToken.type == key_begin)
		{
			//token = scanner->CallScanner(true);
			break;
		}
		else if (tempToken.type == key_global)
		{ //making sure the variable we are inserting into the symbol table is of the global scope
			token = scanner->CallScanner(true);
			tempToken = scanner->CallScanner(false);
			tempScope.name = "global";

			if (tempToken.type == key_variable) //matching it to either a procedure or a variable and calling the appropriate function
			{
				token = scanner->CallScanner(true);
				
				VariableDeclaration();
			}
			else if (tempToken.type == key_procedure)
			{
				token = scanner->CallScanner(true);
				ProcedureDeclaration();
			}
		}
		else if (tempToken.type == key_procedure)
		{
			token = scanner->CallScanner(true);
			ProcedureDeclaration();
		}
		else if (tempToken.type == key_variable)
		{			
			token = scanner->CallScanner(true);
			VariableDeclaration();
		}
		//tempToken = scanner->CallScanner(false); //determines if it is either a procedure/variable
		else
		{
			std::cout << "ERROR, your program does not follow the guidelines for a declaration. Line: " << scanner->getLineNum() << std::endl;
		}
	}
}

void Parser::ProcedureDeclaration() //the procedure tree
{
	ProcedureHeader();

	ProcedureBody();

	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == sym_sc)
	{
		token = scanner->CallScanner(true);
	}
	else
	{
		std::cout << "ERROR, a semi colon was expected to be found at the end of your procedure declaration. Line: " << scanner->getLineNum() << std::endl;
	}
}

void Parser::ProcedureHeader()
{
	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == id)
	{
		token = scanner->CallScanner(true);
		Scope scope;
		scope.name = token.val.stringVal;
		scope.scopeLoc = SymTab.getScopeLoc();
		if (tempScope.name != "global")
		{ //if the procedure has a subprocedure we ensure it knows which scope is its parent
			scope.parent = scope.scopeLoc - 1;
		}
		tempScope.name = token.val.stringVal;
		
		SymTab.AddScope(scope);

		Symbol procedure; //for adding a new procedure under a new scope
		procedure.setIdentifier(token.val.stringVal);
		procedure.setIsProcedure(true);
		
		if (tempScope.name != "global") //a check to see if the parent is global or another procedure
		{
			procedure.setScopeName(tempScope.name);
		}
		else
		{
			Scope parent = SymTab.GetScope(tempScope.name);
			tempScope.name = token.val.stringVal;
		}

		tempToken = scanner->CallScanner(false);

		if (tempToken.type == sym_colon)
		{
			token = scanner->CallScanner(true);

			tempToken = scanner->CallScanner(false);

			if (TypeMark(tempToken.type))
			{
				token = scanner->CallScanner(true);
				procedure.setType(token.type); //adding the type to the function call
				SymTab.InsertSymbol(procedure);
				tempToken = scanner->CallScanner(false);

				if (tempToken.type == sym_lparen)
				{
					token = scanner->CallScanner(true);

					ParameterList(); //calling parameter list

					tempToken = scanner->CallScanner(false);

					if (tempToken.type == sym_rparen)
					{
						token = scanner->CallScanner(true);
						
						return;
					}
					else
					{
						std::cout << "ERROR, a right parentheses was expected to be found in your procedure header. Line: " << scanner->getLineNum() << std::endl;
					}
				}
				else
				{
					std::cout << "ERROR, a left parentheses was expected to be found in your procedure header. Line: " << scanner->getLineNum() << std::endl;
				}
			}
			else
			{
				std::cout << "ERROR, a Type Mark was expected to be found in your procedure header. Line: " << scanner->getLineNum() << std::endl;
			}
		}
		else
		{
			std::cout << "ERROR, a colon was expected to be found in your procedure header. Line: " << scanner->getLineNum() << std::endl;
		}
	}
	else
	{
		std::cout << "ERROR, an identifier was expected to be found in your procedure header. Line: " << scanner->getLineNum() << std::endl;
	}
}

void Parser::ParameterList()
{
	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type != key_variable)
	{
		return; // no params
	}
	
	token = scanner->CallScanner(true);

	Parameter();

	tempToken = scanner->CallScanner(false);

	if (tempToken.type == sym_comma)
	{
		token = scanner->CallScanner(true);

		ParameterList(); //recursivley calls itself until it is ready to move on
	}
	else if (tempToken.type == sym_rparen)
	{
		return;
	}
}

void Parser::Parameter()
{
	VariableDeclaration(); //just calls variable declaration
}

void Parser::ProcedureBody()
{
	Declaration();

	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == key_begin) //second check
	{
		token = scanner->CallScanner(true);

		Token tempToken = scanner->CallScanner(false);
		while (tempToken.type != key_end)
		{
			Statement(); //recursive call

			tempToken = scanner->CallScanner(false);
		}
	}

	tempToken = scanner->CallScanner(false);

	if (tempToken.type == key_end)
	{
		token = scanner->CallScanner(true);

		tempToken = scanner->CallScanner(false);

		if (tempToken.type == key_procedure)
		{
			token = scanner->CallScanner(true);
			if (SymTab.GetScope(tempScope.name).parent != 0)
			{
				tempScope.name = SymTab.FindScope(SymTab.GetScope(tempScope.name).scopeLoc-1).name; //if the scope has a parent, we make sure it goes to that scope and not global
			}
			return;
		}
		else
		{
			std::cout << "ERROR, a procedure was expected to be found in your procedure body. Line: " << scanner->getLineNum() << std::endl;
		}
	}
	else
	{
		std::cout << "ERROR, an end was expected to be found in your procedure body. Line: " << scanner->getLineNum() << std::endl;
	}
}

void Parser::VariableDeclaration() //declaring the variable
{
	Token tempToken = scanner->CallScanner(false); //scanning for the identifier

	if (tempToken.type == id)
	{
		token = scanner->CallScanner(true);
		
		Symbol id;
		id.setIdentifier(token.val.stringVal);
		if (tempScope.name == "global")
		{
			id.setIsGlobal(true);
		}
		
		id.setScopeName(tempScope.name); //adding the proper scope to it

		tempToken = scanner->CallScanner(false);

		if (tempToken.type == sym_colon)
		{
			token = scanner->CallScanner(true); //scanning for the type mark
			//definition type = token.type; //not sure if i need this for something but I am going to leave it here

			tempToken = scanner->CallScanner(false);

			if (TypeMark(tempToken.type))
			{
				token = scanner->CallScanner(true);
				id.setType(token.type);
				
				tempToken = scanner->CallScanner(false);

				if (tempToken.type == sym_lbrack)
				{
					token = scanner->CallScanner(true);

					id.setIsArr(true);

					tempToken = scanner->CallScanner(false);

					if (tempToken.type == literal_int) //bound check
					{
						token = scanner->CallScanner(true);

						Number();

						tempToken = scanner->CallScanner(false);

						if (tempToken.type == sym_rbrack)
						{
							token = scanner->CallScanner(true);

							tempToken = scanner->CallScanner(false);
							if (tempToken.type == sym_sc)
							{
								token = scanner->CallScanner(true);
								SymTab.InsertSymbol(id);
								return;
							}
							else
							{
								std::cout << "ERROR, finding a semi colon, in program on line: " << scanner->getLineNum() << std::endl;
							}
						}
						else
						{
							std::cout << "ERROR, we were expecting a right bracket on line: " << scanner->getLineNum() << std::endl;
						}
					}
					else
					{
						std::cout << "ERROR, we were expecting an integer on line: " << scanner->getLineNum() << std::endl;
					}
				}
				else if (tempToken.type == sym_sc || tempToken.type == sym_rparen || tempToken.type == sym_comma)
				{
					id.setIsArr(false);
					SymTab.InsertSymbol(id);
					if (tempToken.type == sym_sc)
					{
						token = scanner->CallScanner(true);
					}
					
					return;
				}
				else
				{
					std::cout << "ERROR, a semi colon was expected to be found in your variable declaration. Line: " << scanner->getLineNum() << std::endl;
				}
			}
			else
			{
				std::cout << "ERROR, a Type Mark was expected to be found in your variable declaration. Line: " << scanner->getLineNum() << std::endl;
			}
		}
		else
		{
			std::cout << "ERROR, a colon was expected to be found in your variable declaration. Line: " << scanner->getLineNum() << std::endl;
		}
	}
	else
	{
		std::cout << "ERROR, an identifier was expected to be found in your variable declaration. Line: " << scanner->getLineNum() << std::endl;
	}
}

bool Parser::TypeMark(definition type) //makes sure the token scanned in is of the correct type mark and returns a bool
{
	if (type == num_integer || type == num_float || type == str || type == boolean)
	{
		return true;
	}
	return false;
}

void Parser::Statement() //assignment, if, loop, and return
{
	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == id)
	{
		token = scanner->CallScanner(true);

		AssignmentStatement();
	}
	else if (tempToken.type == key_if)
	{
		token = scanner->CallScanner(true);

		IfStatement();
	}
	else if (tempToken.type == key_for)
	{
		token = scanner->CallScanner(true);

		LoopStatement();
	}
	else if (tempToken.type == key_return)
	{
		token = scanner->CallScanner(true);

		ReturnStatement();
	}
	else
	{
		std::cout << "ERROR, a statement declaration was expected on line: " << scanner->getLineNum() << std::endl;
	}

	tempToken = scanner->CallScanner(false);

	if (tempToken.type == sym_sc)
	{
		token = scanner->CallScanner(true);
	}
	else
	{
		std::cout << "ERROR, a semi colon was expected to be found at the end of your procedure declaration. Line: " << scanner->getLineNum() << std::endl;
	}
}

void Parser::AssignmentStatement()
{
	//check if the types match

	Symbol dest = Destination();
	std::cout << "Symbol identifier: " + dest.getIdentifer() << std::endl;
	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == sym_colEqual)
	{
		token = scanner->CallScanner(true);
	}
	else
	{
		std::cout << "ERROR, a assignment expression or := was expected to be found in your Assignment Statement. Line: " << scanner->getLineNum() << std::endl;
	}

	definition expressionType;
	Symbol exp = Expression(expressionType); //calls the expression tree
	
	//type check here
	definition typeReturn;
	bool assignCheck = ValidTypesAssignment(typeReturn, dest.getType(), expressionType);
	
	if (!assignCheck)
	{
		std::cout << dest.getType() << " and " << expressionType << ", are invalid types.\n";
	}
	else
	{
		std::cout << dest.getType() << " and " << expressionType << ", are valid types.\n";
	}
}

bool Parser::ValidTypesAssignment(definition &returnDef, int lhs, int rhs) //checks if the types match upon assignment
{
	if (lhs == num_integer && (rhs == literal_int || rhs == bool_true || rhs == bool_false))
	{
		returnDef = num_integer;
		return true;
	}
	else if (lhs == str && rhs == literal_string)
	{
		returnDef = str;
		return true;
	}
	else if (lhs == boolean && (rhs == bool_true || rhs == bool_false || rhs == literal_int))
	{
		returnDef = boolean;
		return true;
	}
	else if (lhs == num_float && (rhs == literal_int || rhs == literal_float))
	{
		returnDef = num_float;
		return true;
	}
	return false;
}

Symbol Parser::Destination()
{
	if (token.type == id) //already scanned
	{
		Symbol destination;
		//std::cout << token.val.stringVal << std::endl; //checks
		//std::cout << tempScope.name << std::endl;
		destination = SymTab.FindSymbol(token.val.stringVal, SymTab.GetScope(tempScope.name)); //finds the symbol in the symbol table using the tempScope
		if (destination.getArray() == true)
		{
			Token tempToken = scanner->CallScanner(false);

			if (tempToken.type == sym_lbrack)
			{
				token = scanner->CallScanner(true);

				tempToken = scanner->CallScanner(false);

				if (tempToken.type == literal_int)
				{
					token = scanner->CallScanner(true);

					tempToken = scanner->CallScanner(false);

					if (tempToken.type == sym_rbrack)
					{
						token = scanner->CallScanner(true);
						
						return destination;
					}
					else
					{
						std::cout << "ERROR, a right bracket was expected to be found apart of your array. Line: " << scanner->getLineNum() << std::endl;
					}
				}
				else
				{
					std::cout << "ERROR, integers are the only type compatible with arrays. Line: " << scanner->getLineNum() << std::endl;
				}
			}
		}

		return destination;
	}
	else
	{
		std::cout << "ERROR, an identifier was expected in your Destination. Line: " << scanner->getLineNum() << std::endl;
		return Symbol();
	}
}
/*
Everything in the expression tree uses the same logic as Expression()
if the symbol being passed in (empty by default) is empty, we go down the tree til we return the correct token
Does a type check comparisons back up the tree
*/
Symbol Parser::Expression(definition& expressionType, Symbol arith)
{
	definition lhsType; //if the optional symbol (declared in the header) is blank, we go down the tree, else assign the left side type
	if (arith.getIdentifer() == "")
	{
		arith = ArithOp(lhsType);
	}
	else
	{
		lhsType = definition(arith.getType());
	}

	Symbol expr = Symbol();

	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == key_and || tempToken.type == key_or || tempToken.type == key_not) //if the next token is an expression
	{
		expr.setIdentifier(tempToken.val.stringVal); //assign it to expr
		expr.setType(tempToken.type);

		expr.addChild(arith); //adding child to the symbol that has gone down the tree
		token = scanner->CallScanner(true);
		definition rhsType;

		arith.addChild(ArithOp(rhsType)); //adding child to the right side

		if (!ValidTypesExpression(expressionType, lhsType, rhsType)) //type checks the symbols being looked at in the expression tree
		{
			std::cout << "ERROR, invalid types on line: " << scanner->getLineNum() << std::endl;
		}

		expr.printTree(); //check the tree

		Token exprOp = scanner->CallScanner(false);

		if (tempToken.type == key_and || tempToken.type == key_or || tempToken.type == key_not) //if the next token is an expression, we recursively call it
		{
			return Expression(expressionType, expr);
		}
		else
		{
			return expr;
		}
	}
	else //if it is not, return it and the expression type
	{
		expressionType = definition(lhsType);
		return arith;
	}
}

Symbol Parser::ArithOp(definition &arithType, Symbol relation)
{
	definition lhsType;
	if (relation.getIdentifer() == "")
	{
		relation = Relation(lhsType);
	}
	else
	{
		lhsType = definition(relation.getType());
	}

	Symbol arith = Symbol();

	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == add_op || tempToken.type == sub_op)
	{
		arith.setIdentifier(tempToken.val.stringVal);
		arith.setType(tempToken.type);

		arith.addChild(relation);
		token = scanner->CallScanner(true);
		definition rhsType;

		arith.addChild(Relation(rhsType));

		if (!ValidTypesExpression(arithType, lhsType, rhsType))
		{
			std::cout << "ERROR, invalid types on line: " << scanner->getLineNum() << std::endl;
		}

		arith.printTree();

		Token arithOp = scanner->CallScanner(false);

		if (arithOp.type == add_op || arithOp.type == sub_op)
		{
			return ArithOp(arithType, arith);
		}
		else
		{
			return arith;
		}
	}
	else
	{
		arithType = definition(lhsType);
		return relation;
	}

}

Symbol Parser::Relation(definition &relationType, Symbol term)
{
	definition lhsType;
	if (term.getIdentifer() == "")
	{
		term = Term(lhsType);
	}
	else
	{
		lhsType = definition(term.getType());
	}

	Symbol relation = Symbol();

	Token tempToken = scanner->CallScanner(false);

	if (isRelation(tempToken.type))
	{
		relation.setIdentifier(tempToken.val.stringVal);
		relation.setType(tempToken.type);

		relation.addChild(term);
		token = scanner->CallScanner(true); //scanning the relation in
		definition rhsType;

		relation.addChild(Term(rhsType));

		if (!ValidTypesExpression(relationType, lhsType, rhsType))
		{
			std::cout << "ERROR, invalid types on line: " << scanner->getLineNum() << std::endl;
		}

		relation.printTree();

		Token relationOp = scanner->CallScanner(false);

		if (isRelation(relationOp.type))
		{
			return Relation(relationType, relation);
		}
		else
		{
			return relation;
		}
	}
	else
	{
		relationType = definition(lhsType);
		return term;
	}
}

bool Parser::isRelation(definition type) //bool that checks if the token is of a type relation, only did it with relation for the expression tree since the others have less options
{
	if (type >= sym_less && type <= sym_doubEqual)
	{
		return true;
	}
	return false;
}

Symbol Parser::Term(definition &termType, Symbol factor)
{
	definition lhsType;
	if (factor.getIdentifer() == "")
	{
		factor = Factor(lhsType);
	}
	else
	{
		lhsType = definition(factor.getType());
	}

	Symbol product = Symbol();

	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == mult_op || tempToken.type == div_op)
	{
		product.setIdentifier(tempToken.val.stringVal);
		product.setType(tempToken.type);

		product.addChild(factor);
		token = scanner->CallScanner(true); //scanning the operator in
		definition rhsType;
		Symbol rhs = Factor(rhsType);
		product.addChild(rhs);

		if (!ValidTypesExpression(termType, lhsType, rhsType))
		{
			std::cout << "ERROR, invalid types on line: " << scanner->getLineNum() << std::endl;
		}

		Token termOp = scanner->CallScanner(false);

		product.printTree(); //debug

		if (termOp.type == mult_op || termOp.type == div_op)
		{
			return Term(termType, product);
		}
		else
		{
			
			return product;
		}
	}
	else 
	{
		termType = definition(lhsType);
		return factor;
	}
}

bool Parser::ValidTypesExpression(definition& returnDef, int lhs, int rhs)
{ //type checks the symbols being looked at in the expression tree
	if (lhs == rhs) 
	{
		returnDef = definition(lhs);
		return true;
	}
	else if ((lhs == literal_int && (rhs == bool_true || rhs == bool_false || isRelation(definition(rhs)))) || (rhs == literal_int && (lhs == bool_true || lhs == bool_false || isRelation(definition(rhs)))))
	{
		returnDef = literal_int;
		return true;
	}
	else if ((lhs == literal_int && rhs == literal_float) || (lhs == literal_float && rhs == literal_int))
	{
		returnDef = literal_float;
		return true;
	}
	else if ((lhs == bool_true && (rhs == bool_false || isRelation(definition(rhs))) || (lhs == bool_false && (rhs == bool_true || isRelation(definition(rhs))))))
	{
		returnDef = literal_int;
		return true;
	}
	else if (isRelation(definition(lhs)) && (rhs == bool_true || rhs == bool_false || isRelation(definition(rhs))))
	{
		returnDef = literal_int;
		return true;
	}

	return false;
}

Symbol Parser::Factor(definition &factorType)
{ //checks all of the factors and returns back up the tree from the bottom
	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == sym_lparen)
	{
		token = scanner->CallScanner(true);
		definition expressionType;
		Symbol expr = Expression(expressionType);

		tempToken = scanner->CallScanner(false);
		if (tempToken.type == sym_rparen)
		{
			token = scanner->CallScanner(true);
			return expr;
		}
		else
		{
			std::cout << "ERROR, we were expecting a right parentheses on line: " << scanner->getLineNum() << std::endl;
		}
	}
	else if (tempToken.type == id)
	{
		token = scanner->CallScanner(true); //scan in the id

		tempToken = scanner->CallScanner(false);

		if (tempToken.type == sym_lparen)
		{ 
			Symbol procedure = ProcedureCall();
			factorType = MapVariableToLiteral(procedure.getType());
			return procedure;
		}
		else if (tempToken.type == sym_lbrack)
		{
			Symbol arr = SymTab.FindSymbol(token.val.stringVal, SymTab.GetScope(tempScope.name));
			factorType = MapVariableToLiteral(arr.getType());

			token = scanner->CallScanner(true); //scan in brack after assignment to symbol

			tempToken = scanner->CallScanner(false);

			if (tempToken.type == literal_int)
			{
				token = scanner->CallScanner(true);
				Number();
				
				tempToken = scanner->CallScanner(false);

				if (tempToken.type == sym_rbrack)
				{
					token = scanner->CallScanner(true);
					return arr;
				}
			}
			else
			{
				std::cout << "ERROR, integers are the only type compatible with arrays. Line: " << scanner->getLineNum() << std::endl;
			}
		}
		Symbol id = SymTab.FindSymbol(token.val.stringVal, SymTab.GetScope(tempScope.name));
		factorType = MapVariableToLiteral(id.getType());
		return id;
	}
	else if (tempToken.type == sub_op)
	{
		//check if procedure call or name
		token = scanner->CallScanner(true);

		tempToken = scanner->CallScanner(false);

		if (tempToken.type == id)
		{
			token = scanner->CallScanner(true);
			Symbol name = Name();
			factorType = MapVariableToLiteral(name.getType());
			return name;
		}
		else if (tempToken.type == literal_int || tempToken.type == literal_float)
		{
			token = scanner->CallScanner(true);
			factorType = token.type;
			return Number();
		}
	}
	else if (tempToken.type == literal_int || tempToken.type == literal_float)
	{
		token = scanner->CallScanner(true);
		factorType = token.type;
		return Number();
	}
	else if (tempToken.type == literal_string)
	{
		token = scanner->CallScanner(true);
		factorType = token.type;
		return String();
	}
	else if (tempToken.type == bool_true)
	{
		token = scanner->CallScanner(true);
		factorType = token.type;
		Symbol boolT;
		boolT.setIdentifier(token.val.stringVal);
		boolT.setType(token.type);

		return boolT;
	}
	else if (tempToken.type == bool_false)
	{
		token = scanner->CallScanner(true);
		factorType = token.type;
		Symbol boolF;
		boolF.setIdentifier(token.val.stringVal);
		boolF.setType(token.type);

		return boolF;
	}
	else
	{
		std::cout << "ERROR, a wrong type was presented in your factor. Line: " << scanner->getLineNum() << std::endl;
	}
	
}

definition Parser::MapVariableToLiteral(int def)
{ //mapping our variable to a literal for type checking
	switch (def) {
	case num_integer:
		return literal_int;
	case num_float:
		return literal_float;
	case str:
		return literal_string;
	case boolean:
		return bool_true;
	}
}

Symbol Parser::ProcedureCall() //at this point the id is scanned in
{
	bool isIn = SymTab.LookupSymbol(token.val.stringVal);
	Symbol procedureCall; //make a symbol
	if (isIn)
	{
		if (SymTab.GetScope(token.val.stringVal).name != "") //this check is for the global built in functions
		{
			//if the token has a function call, we get the scope and find it
			Scope procScope = SymTab.GetScope(token.val.stringVal);
			procedureCall = SymTab.FindSymbol(token.val.stringVal, procScope);
			
		}
		else
		{
			procedureCall = SymTab.FindSymbol(token.val.stringVal, SymTab.GetScope("global")); //then global
		}

		token = scanner->CallScanner(true); //already know it is a procedure call

		Token tempToken = scanner->CallScanner(false);
		if (tempToken.type != sym_rparen)
		{
			ArgumentList();
		}
		
		token = scanner->CallScanner(true);
	}

	return procedureCall;
}

void Parser::ArgumentList()
{
	definition expressionType;
	Expression(expressionType);

	Token tempToken = scanner->CallScanner(false);
	if (tempToken.type == sym_comma)
	{
		token = scanner->CallScanner(true);

		ArgumentList();
	}
	else if (tempToken.type == sym_rparen)
	{
		token = scanner->CallScanner(true);
		return;
	}
	else
	{
		std::cout << "ERROR, either a comma or a right parentheses was expected in your argument list. Line: " << scanner->getLineNum() << std::endl;
	}
}

Symbol Parser::Name()
{
	if (token.type == id)
	{
		return SymTab.FindSymbol(token.val.stringVal, SymTab.GetScope(tempScope.name));
	}
	else
	{
		std::cout << "ERROR, an identifier was expected in your variable declaration. Line: " << scanner->getLineNum() << std::endl;
	}
}

Symbol Parser::Number()
{

	if (token.type == literal_int)
	{
		Symbol nSym;
		nSym.setIdentifier(std::to_string(token.val.intVal));
		nSym.setIsArr(false);
		nSym.setType(token.type);

		return nSym;
	}
	else if (token.type == literal_float)
	{
		Symbol nSym;
		nSym.setIdentifier(std::to_string(token.val.floatVal));
		nSym.setIsArr(false);
		nSym.setType(token.type);

		return nSym;
	}
}

Symbol Parser::String()
{
	if (token.type == literal_string)
	{
		Symbol sSym;
		sSym.setIdentifier(token.val.stringVal);
		sSym.setType(token.type);

		return sSym;
	}
}

void Parser::IfStatement()
{
	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == sym_lparen)
	{
		token = scanner->CallScanner(true);

		definition expressionType;
		Expression(expressionType); //calls the expression tree

		tempToken = scanner->CallScanner(false);

		if (tempToken.type == sym_rparen)
		{
			token = scanner->CallScanner(true);

			tempToken = scanner->CallScanner(false);
			if (tempToken.type == key_then)
			{
				token = scanner->CallScanner(true);

				Token tempToken = scanner->CallScanner(false);
				while (tempToken.type != key_end && tempToken.type != key_else)
				{
					Statement(); //checking for multiple statements in the if block

					tempToken = scanner->CallScanner(false);
				}

				if (tempToken.type == key_end)
				{
					token = scanner->CallScanner(true);

					tempToken = scanner->CallScanner(false);
					if (tempToken.type == key_if)
					{
						token = scanner->CallScanner(true);
						//scanned the entire if
						return;
					}
					else
					{
						std::cout << "ERROR, an if was expected at the end of it.. Line: " << scanner->getLineNum() << std::endl;
					}
				}
				else if (tempToken.type == key_else) //else block
				{
					token = scanner->CallScanner(true);

					Token tempToken = scanner->CallScanner(false);
					while (tempToken.type != key_end)
					{ //scanning else statements
						Statement(); 

						tempToken = scanner->CallScanner(false);
					}
					
					token = scanner->CallScanner(true);

					tempToken = scanner->CallScanner(false);
					if (tempToken.type == key_if)
					{
						token = scanner->CallScanner(true);

						return;
					}
					else
					{
						std::cout << "ERROR, an if was expected at the end of it. Line: " << scanner->getLineNum() << std::endl;
					}
				}
				else
				{
					std::cout << "ERROR, an else was expected at the end of it. Line: " << scanner->getLineNum() << std::endl;
				}
			}
			else
			{
				std::cout << "ERROR, a then was expected at the end of it. Line: " << scanner->getLineNum() << std::endl;
			}
		}
		else
		{
			std::cout << "ERROR, a right parentheses was expected at the end of it. Line: " << scanner->getLineNum() << std::endl;
		}
	}
	else
	{
		std::cout << "ERROR, a left parentheses was expected here. Line: " << scanner->getLineNum() << std::endl;
	}

}

void Parser::LoopStatement()
{
	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == sym_lparen)
	{
		token = scanner->CallScanner(true);
		
		tempToken = scanner->CallScanner(false);

		if (tempToken.type == id)
		{
			token = scanner->CallScanner(true);

			AssignmentStatement(); //checks the assignment statement

			tempToken = scanner->CallScanner(false);

			if (tempToken.type == sym_sc)
			{
				token = scanner->CallScanner(true);
				
				definition expressionType;
				Expression(expressionType); //checks the expression

				tempToken = scanner->CallScanner(false);

				if (tempToken.type == sym_rparen)
				{
					token = scanner->CallScanner(true);

					//while loop for statements
					Token tempToken = scanner->CallScanner(false);
					while (tempToken.type != key_end)
					{
						Statement(); //runs thru the for loop statements

						tempToken = scanner->CallScanner(false);
					}

					tempToken = scanner->CallScanner(false);

					if (tempToken.type == key_end)
					{
						token = scanner->CallScanner(true);

						tempToken = scanner->CallScanner(false);

						if (tempToken.type == key_for)
						{
							token = scanner->CallScanner(true);

							return;
						}
						else
						{
							std::cout << "ERROR, a for was expected at the end of it. Line: " << scanner->getLineNum() << std::endl;
						}
					}
					else
					{
						std::cout << "ERROR, an end was expected at the end of it. Line: " << scanner->getLineNum() << std::endl;
					}
				}
				else
				{
					std::cout << "ERROR, a right parentheses was expected here. Line: " << scanner->getLineNum() << std::endl;
				}
			}
		}
	}
}

void Parser::ReturnStatement()
{ //just calls an expression
	definition expressionType;
	Expression(expressionType);
}

void Parser::BuiltInFunctionDeclarations()
{ //declaring the built in functions, need in the symbol table to compile the correct programs
	Symbol getBool;
	getBool.setIdentifier("getbool");
	getBool.setIsGlobal(true);
	getBool.setIsProcedure(true);
	getBool.setScopeName("global");
	getBool.setType(4);
	SymTab.InsertSymbol(getBool);

	Symbol getInteger;
	getInteger.setIdentifier("getinteger");
	getInteger.setIsGlobal(true);
	getInteger.setIsProcedure(true);
	getInteger.setScopeName("global");
	getInteger.setType(5);
	SymTab.InsertSymbol(getInteger);

	Symbol getFloat;
	getFloat.setIdentifier("getfloat");
	getFloat.setIsGlobal(true);
	getFloat.setIsProcedure(true);
	getFloat.setScopeName("global");
	getFloat.setType(6);
	SymTab.InsertSymbol(getFloat);

	Symbol getString;
	getString.setIdentifier("getstring");
	getString.setIsGlobal(true);
	getString.setIsProcedure(true);
	getString.setScopeName("global");
	getString.setType(3);
	SymTab.InsertSymbol(getString);

	Symbol putBool;
	putBool.setIdentifier("putbool");
	putBool.setIsGlobal(true);
	putBool.setIsProcedure(true);
	putBool.setScopeName("global");
	putBool.setType(4);
	SymTab.InsertSymbol(putBool);

	Symbol putInteger;
	putInteger.setIdentifier("putinteger");
	putInteger.setIsGlobal(true);
	putInteger.setIsProcedure(true);
	putInteger.setScopeName("global");
	putInteger.setType(4);
	SymTab.InsertSymbol(putInteger);

	Symbol putFloat;
	putFloat.setIdentifier("putfloat");
	putFloat.setIsGlobal(true);
	putFloat.setIsProcedure(true);
	putFloat.setScopeName("global");
	putFloat.setType(4);
	SymTab.InsertSymbol(putFloat);

	Symbol putString;
	putString.setIdentifier("putstring");
	putString.setIsGlobal(true);
	putString.setIsProcedure(true);
	putString.setScopeName("global");
	putString.setType(4);
	SymTab.InsertSymbol(putString);

	Symbol sqrt;
	sqrt.setIdentifier("sqrt");
	sqrt.setIsGlobal(true);
	sqrt.setIsProcedure(true);
	sqrt.setScopeName("global");
	sqrt.setType(6);
	SymTab.InsertSymbol(sqrt);
}