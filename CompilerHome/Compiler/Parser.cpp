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
	Declaration(); //need some form of a while to keep on reading in declarations

	
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

		tempToken = scanner->CallScanner(false);

		if (tempToken.type == sym_colon)
		{
			token = scanner->CallScanner(true);

			tempToken = scanner->CallScanner(false);

			if (TypeMark(tempToken.type))
			{
				token = scanner->CallScanner(true);

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
	Declaration(); //need some form of a while to keep on reading in declarations

	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == key_begin) //second check
	{
		token = scanner->CallScanner(true);

		Statement(); //need some form of a while to keep on reading in statements
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
		else
		{
			id.setIsGlobal(false);
		}
		id.setIsProcedure(false);
		id.setType(0);
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
				else if (tempToken.type == sym_sc) //end the declaration, semi colon
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

	Symbol exp = Expression();
	
	//type check here
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

Symbol Parser::Expression()
{
	//std::cout << "Expression\n";
	Symbol expr;

	ArithOp();

	SubExpression();

	return expr;
}

Symbol Parser::SubExpression()
{
	//std::cout << "SubExpression\n";
	Token tempToken = scanner->CallScanner(false);
	
	if (tempToken.type == key_and)
	{
		token = scanner->CallScanner(true);

		//ArithOp();

		return ArithOp();
	}
	else if (tempToken.type == key_or)
	{
		token = scanner->CallScanner(true);

		//ArithOp();

		return ArithOp();
	}
	else if (tempToken.type == key_not)
	{
		token = scanner->CallScanner(true);

		//ArithOp();

		return ArithOp();;
	}

	return Symbol();
}

Symbol Parser::ArithOp()
{
	Relation();

	SubArithOp();

	return Symbol();
}

Symbol Parser::SubArithOp()
{
	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == add_op)
	{
		token = scanner->CallScanner(true);

		//Relation();

		return Relation();
	}
	else if (tempToken.type == sub_op)
	{
		token = scanner->CallScanner(true);

		//Relation();

		return Relation();
	}
	return Symbol();
}

Symbol Parser::Relation(Symbol term)
{
	if (term.getIdentifer() == "")
	{
		term = Term();
	}

	Symbol relation = Symbol();

	Token tempToken = scanner->CallScanner(false);

	if (isRelation(tempToken.type))
	{
		relation.setIdentifier(tempToken.val.stringVal);
		relation.setType(tempToken.type);

		relation.addChild(term);
		token = scanner->CallScanner(true); //scanning the relation in

		relation.addChild(Term());

		relation.printTree();

		Token relationOp = scanner->CallScanner(false);

		if (isRelation(relationOp.type))
		{
			return Relation(relation);
		}
		else
		{
			return relation;
		}
	}
	else
	{
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

Symbol Parser::Term(Symbol factor)
{
	if (factor.getIdentifer() == "")
	{
		factor = Factor();
	}

	Symbol product = Symbol();

	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == mult_op || tempToken.type == div_op)
	{
		product.setIdentifier(tempToken.val.stringVal);
		product.setType(tempToken.type);

		product.addChild(factor);
		token = scanner->CallScanner(true); //scanning the operator in
		Symbol rhs = Factor();
		product.addChild(rhs);

		Token termOp = scanner->CallScanner(false);

		product.printTree(); //debug

		if (termOp.type == mult_op || termOp.type == div_op)
		{
			return Term(product);
		}
		else
		{
			return product;
		}
	}
	else 
	{
		return factor;
	}
}


Symbol Parser::Factor()
{
	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == sym_lparen)
	{
		token = scanner->CallScanner(true);
		Symbol expr = Expression();

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

			return Number();
		}
	}
	else if (tempToken.type == literal_int || tempToken.type == literal_float)
	{
		
		token = scanner->CallScanner(true);

		return Number();
	}
	else if (tempToken.type == literal_string)
	{
		token = scanner->CallScanner(true);

		return String();
	}
	else if (tempToken.type == bool_true)
	{
		token = scanner->CallScanner(true);

		Symbol boolT;
		boolT.setIdentifier(token.val.stringVal);
		boolT.setType(token.type);

		return boolT;
	}
	else if (tempToken.type == bool_false)
	{
		token = scanner->CallScanner(true);

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
	Expression();

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

		Expression();

		tempToken = scanner->CallScanner(false);

		if (tempToken.type == sym_rparen)
		{
			token = scanner->CallScanner(true);

			tempToken = scanner->CallScanner(false);
			if (tempToken.type == key_then)
			{
				token = scanner->CallScanner(true);

				Statement();

				tempToken = scanner->CallScanner(false);

				if (tempToken.type == sym_sc) //will need to account for multiple statements and to account for potential else statements
				{
					token = scanner->CallScanner(true);

					tempToken = scanner->CallScanner(false);

					if (tempToken.type == key_end)
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
					else
					{
						//error end
					}
				}
				else if (tempToken.type == key_else) //while loop for multiple elses? i think
				{
					token = scanner->CallScanner(true);

					Statement();

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

		tempToken = scanner->CallScanner(false);

		if (tempToken.type == sym_sc)
		{
			token = scanner->CallScanner(true);

			Expression();

			tempToken = scanner->CallScanner(false);

			if (tempToken.type == sym_rparen)
			{
				token = scanner->CallScanner(true);

				//while loop for statements
				Statement();

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
		else
		{
			//error sc
		}
	}
}

void Parser::ReturnStatement()
{
	Expression();
}