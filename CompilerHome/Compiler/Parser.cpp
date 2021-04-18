#pragma once
#include "Parser.h"
#include "Scanner.h"
#include <iostream>

//*********ERRORS GRAB LINENUM FROM THE SCANNER**********


Parser::Parser()
{
	scanner = new Scanner("test.txt");

	Program();

	scanner->closeFile();
}

void Parser::Program()
{
	Scope local; //the default local scope
	local.name = "local";
	local.scopeLoc = SymTab.getScopeLoc();
	SymTab.AddScope(local); //adding the global scope at the very beginning
	tempScope.name = "local";

	ProgramHeader();

	ProgramBody();

	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == key_period)
	{
		token = scanner->CallScanner(true);

		std::cout << "End of program.\n";
	}
	else
	{
		//error .
	}
}

void Parser::ProgramHeader() // add elses for errors
{
	Token tempToken = scanner->CallScanner(false);

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
				std::cout << "Error finding the key 'is', in program.\n";
				//return; //need to not run the body
			}
		}
		else
		{
			std::cout << "Error finding the program name, in program.\n";
		}
	}
	else
	{
		std::cout << "Error finding the key 'program', in program.\n";
	}
}

void Parser::ProgramBody() //contains declarations and statements
{
	Declaration();

	
	Token tempToken = scanner->CallScanner(false);
	while (tempToken.type != key_end) 
	{
		Statement(); //need some form of a while to keep on reading in statements
		
		tempToken = scanner->CallScanner(false);
	}
	

	//tempToken = scanner->CallScanner(false);

	if (tempToken.type == key_end)
	{
		std::cout << "This should be end\t";
		token = scanner->CallScanner(true);

		tempToken = scanner->CallScanner(false);
		
		if (tempToken.type == key_program)
		{
			std::cout << "This should be program\t"; 
			token = scanner->CallScanner(true);
			return;
		}
		else
		{
			//error looking for program
		}
	}
	else
	{
		//error looking for end
	}
}

void Parser::Declaration() //calls either procedure/variable declaration
{
	while (token.type != key_begin) 
	{
		Token tempToken = scanner->CallScanner(false); //scans in the scope of variable/procedure

		if (tempToken.type == key_begin)
		{
			token = scanner->CallScanner(true);
			break;
		}
		else if (tempToken.type == key_global)
		{
			token = scanner->CallScanner(true);
			if (SymTab.getScopeLoc() < 2)
			{
				Scope global;
				global.name = "global";
				global.scopeLoc = SymTab.getScopeLoc();
				SymTab.AddScope(global);
			}

			tempToken = scanner->CallScanner(false);
			tempScope.name = "global";

			if (tempToken.type == key_variable)
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
			//SymTab.AddScope();
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
			std::cout << "Error, declaration.\n";
		}
	}
}

void Parser::ProcedureDeclaration()
{
	ProcedureHeader();

	ProcedureBody();
}

void Parser::ProcedureHeader()
{
	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == id)
	{
		token = scanner->CallScanner(true);

		Symbol procedure;
		procedure.setIdentifier(token.val.stringVal);
		procedure.setIsProcedure(true);
		tempScope.name = token.val.stringVal;

		tempToken = scanner->CallScanner(false);

		if (tempToken.type == sym_colon)
		{
			token = scanner->CallScanner(true);

			tempToken = scanner->CallScanner(false);

			if (TypeMark(tempToken.type))
			{
				token = scanner->CallScanner(true);
				procedure.setType(token.type);

				tempToken = scanner->CallScanner(false);

				if (tempToken.type == sym_lparen)
				{
					token = scanner->CallScanner(true);

					ParameterList();

					tempToken = scanner->CallScanner(false);

					if (tempToken.type == sym_rparen)
					{
						token = scanner->CallScanner(true);

						return;
					}
					else
					{
						//error )
					}
				}
				else
				{
					//error (
				}
			}
			else
			{
				//error type mark
			}
		}
		else
		{
			//error :
		}
	}
	else
	{
		//error id
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

		ParameterList();
	}
}

void Parser::Parameter()
{
	VariableDeclaration();
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
			Statement(); //need some form of a while to keep on reading in statements

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

			return;
		}
		else
		{
			//error looking for procedure
		}
	}
	else
	{
		//error looking for end
	}
}

void Parser::VariableDeclaration()
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
		
		id.setScopeName(tempScope.name);

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
								return;
							}
							else
							{
								//error sc
							}
						}
						else
						{
							std::cout << "Error, we were expecting a right bracket.\n";
						}
					}
					else
					{
						std::cout << "Error, we were expecting an integer.\n";
					}
				}
				else if (tempToken.type == sym_sc || tempToken.type == sym_rparen || tempToken.type == sym_comma)
				{
					token = scanner->CallScanner(true);
					id.setIsArr(false);
					SymTab.InsertSymbol(id);
					return;
				}
				else
				{
					std::cout << "Error, we were expecting a semi colon or a bracket.\n";
				}
			}
			else
			{
				std::cout << "Error, we were expecting a type mark.\n";
			}
		}
		else
		{
			std::cout << "Error, we were expecting a colon.\n";
		}
	}
	else
	{
		std::cout << "Error, we were expecting an identifier for a variable declaration.\n";
	}
}

bool Parser::TypeMark(definition type)
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
		std::cout << "Error, a statement declaration was expected.\n";
	}

	tempToken = scanner->CallScanner(false);

	if (tempToken.type != sym_sc)
	{
		std::cout << "not a ;\n";
	}

	token = scanner->CallScanner(true);
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
		//error :=
	}

	definition expressionType;
	Symbol exp = Expression(expressionType);
	
	//type check here
	definition typeReturn;
	if (!ValidTypesAssignment(typeReturn, dest.getType(), exp.getType()))
	{
		std::cout << "Invalid types at assignment\n.";
	}
	else
	{
		std::cout << dest.getType() << " and " << exp.getType() << ", are valid types.\n";
	}
}

bool Parser::ValidTypesAssignment(definition &returnDef, int lhs, int rhs)
{
	if (lhs == num_integer && (rhs == literal_int || rhs == literal_float || rhs == bool_true || rhs == bool_false))
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
		return SymTab.FindSymbol(token.val.stringVal);
	}
	else
	{
		std::cout << "Error, we were expecting an identifier for a variable declaration.\n";
		return Symbol();
	}
}

Symbol Parser::Expression(definition& expressionType, Symbol arith)
{
	definition lhsType;
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

	if (tempToken.type == key_and || tempToken.type == key_or || tempToken.type == key_not)
	{
		expr.setIdentifier(tempToken.val.stringVal);
		expr.setType(tempToken.type);

		expr.addChild(arith);
		token = scanner->CallScanner(true);
		definition rhsType;

		arith.addChild(ArithOp(rhsType));

		if (!ValidTypesExpression(expressionType, lhsType, rhsType))
		{
			std::cout << "Error, invalid types.\n";
		}

		expr.printTree();

		Token exprOp = scanner->CallScanner(false);

		if (tempToken.type == key_and || tempToken.type == key_or || tempToken.type == key_not)
		{
			return Expression(expressionType, expr);
		}
		else
		{
			return expr;
		}
	}
	else
	{
		expressionType = definition(arith.getType());
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
			std::cout << "Error, invalid types.\n";
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
		arithType = definition(relation.getType());
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
			std::cout << "Error, invalid types.\n";
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
		relationType = definition(term.getType());
		return term;
	}
}

bool Parser::isRelation(definition type)
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
			std::cout << "Error, invalid types.\n";
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
		termType = definition(factor.getType());
		return factor;
	}
}

bool Parser::ValidTypesExpression(definition& returnDef, int lhs, int rhs)
{
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
{
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
			std::cout << "Error, we were expecting a ')'.\n";
		}
	}
	else if (tempToken.type == id)
	{
		token = scanner->CallScanner(true);

		tempToken = scanner->CallScanner(false);

		if (tempToken.type == sym_lparen)
		{ 
			return ProcedureCall();
		}

		return SymTab.FindSymbol(token.val.stringVal);
	}
	else if (tempToken.type == sub_op)
	{
		//check if procedure call or name
		token = scanner->CallScanner(true);

		tempToken = scanner->CallScanner(false);

		if (tempToken.type == id)
		{
			token = scanner->CallScanner(true);
			return Name();
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
		std::cout << "Error, Factor.\n";
	}
	
}

Symbol Parser::ProcedureCall() //at this point the id is scanned in
{
	bool isIn = SymTab.LookupSymbol(token.val.stringVal);
	Symbol procedureCall;
	if (isIn)
	{
		procedureCall = SymTab.FindSymbol(token.val.stringVal);
		
		//HELP

		token = scanner->CallScanner(true); //already know it is a procedure call

		Token tempToken = scanner->CallScanner(false);
		if (tempToken.type != sym_rparen)
		{
			ArgumentList();
		}
		
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
	else if (token.type == sym_rparen)
	{
		return;
	}
	else
	{
		std::cout << "Error, not a comma or a ).\n";
	}
}

Symbol Parser::Name()
{
	if (token.type == id)
	{
		return SymTab.FindSymbol(token.val.stringVal);
	}
	else
	{
		std::cout << "Error, we were expecting an identifier for a variable declaration.\n";
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
		Expression(expressionType);

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
					Statement();

					tempToken = scanner->CallScanner(false);
				}

				if (tempToken.type == key_end) //will need to account for multiple statements and to account for potential else statements
				{
					token = scanner->CallScanner(true);

					tempToken = scanner->CallScanner(false);
					if (tempToken.type == key_if)
					{
						token = scanner->CallScanner(true);

						return;
					}
					else
					{
						//error looking for if
					}
				}
				else if (tempToken.type == key_else)
				{
					token = scanner->CallScanner(true);

					Token tempToken = scanner->CallScanner(false);
					while (tempToken.type != key_end)
					{
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
						//error looking for if
					}
				}
				else
				{
					//error no sc or else
				}
			}
			else
			{
				//error no then
			}
		}
		else
		{
			std::cout << "Error, we were expecting a ')'.\n";
		}
	}
	else
	{
		std::cout << "Error, we were expecting a '('.\n";
	}

}

void Parser::LoopStatement()
{
	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == sym_lparen)
	{
		token = scanner->CallScanner(true);

		AssignmentStatement();

		definition expressionType;
		Expression(expressionType);

		tempToken = scanner->CallScanner(false);

		if (tempToken.type == sym_rparen)
		{
			token = scanner->CallScanner(true);

			//while loop for statements
			Token tempToken = scanner->CallScanner(false);
			while (tempToken.type != key_end)
			{
				Statement(); //need some form of a while to keep on reading in statements

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
					//error for
				}
			}
			else
			{
				//error end
			}
		}
		else
		{
			//error )
		}
	}
}

void Parser::ReturnStatement()
{
	definition expressionType;
	Expression(expressionType);
}