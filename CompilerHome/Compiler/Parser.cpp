#pragma once
#include "Parser.h"
#include "Scanner.h"
#include <iostream>


Parser::Parser()
{
	scanner = new Scanner("test.txt");

	Program();

	scanner->closeFile();
}

void Parser::Program()
{
	ProgramHeader();

	ProgramBody();
}

void Parser::ProgramHeader() // add elses for errors
{
	token = scanner->ScanToken();

	if (token.type == key_program)
	{
		token = scanner->ScanToken();

		if (token.type == key_progName)
		{
			token = scanner->ScanToken();

			if (token.type == key_is)
			{
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

	if (token.type == key_progBegin) //second check
	{
		Statement();
	}
}

void Parser::Declaration() //calls either procedure/variable declaration
{
	while (token.type != key_progBegin) 
	{
		token = scanner->ScanToken(); //scans in the scope of variable/procedure

		if (token.type == key_progBegin)
		{
			break;
		}

		//will interact with symbol table and check the scope

		token = scanner->ScanToken(); //determines if it is either a procedure/variable

		if (token.type == key_procedure)
		{
			ProcedureDeclaration();
		}
		else if (token.type == key_variable)
		{
			VariableDeclaration();
		}
		else
		{
			std::cout << "Error, the token we were expecting was not a procedure or a variable.\n";
		}
	}
	return;
}

void Parser::ProcedureDeclaration()
{

}

void Parser::VariableDeclaration()
{
	token = scanner->ScanToken(); //scanning for the identifier

	if (token.type == id)
	{
		token = scanner->ScanToken(); //scanning for the colon

		if (token.type == sym_colon)
		{
			token = scanner->ScanToken(); //scanning for the type mark
			definition type = token.type;

			if (TypeMark(token.type))
			{
				token = scanner->ScanToken();

				if (token.type == sym_lbrack)
				{
					token = scanner->ScanToken();

					if (token.type == literal_int)
					{
						token = scanner->ScanToken();

						if (token.type == sym_rbrack)
						{
							return;
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
				else if (token.type == sym_sc) //end the declaration, semi colon
				{
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
	token = scanner->ScanToken();

	if (token.type == id)
	{
		AssignmentStatement();
	}
	else if (token.type == key_if)
	{
		IfStatement();
	}
	else if (token.type == key_for)
	{
		LoopStatement();
	}
	else if (token.type == key_return)
	{
		ReturnStatement();
	}
	else
	{
		std::cout << "Error, a statement declaration was expected.\n";
	}
}

void Parser::AssignmentStatement()
{
	//check if the types match

	Destination();
	
	Expression();
}

void Parser::Destination()
{
	token = scanner->ScanToken();

	if (token.type == id)
	{

	}
	else
	{
		std::cout << "Error, we were expecting an identifier for a variable declaration.\n";
	}
}

void Parser::Expression()
{

}

void Parser::SubExpression()
{

}

void Parser::ArithOp()
{

}

void Parser::SubArithOp()
{

}

void Parser::Relation()
{

}

void Parser::SubRelation()
{

}

void Parser::Term()
{
	Factor();

	//token = scanner->ScanToken();
	
	//if (token.type == mult_op || token.type == div_op)
	//{
	SubTerm();
	//}
}

void Parser::SubTerm()
{
	token = scanner->ScanToken();

	if (token.type == mult_op || token.type == div_op)
	{
		Factor();

		SubTerm();
		
		return;
	}

	std::cout << "Error, .\n";
}

void Parser::Factor()
{
	token = scanner->ScanToken();

	if (token.type == sym_lparen)
	{
		Expression();

		token = scanner->ScanToken();
		if (token.type == sym_rparen)
		{
			return;
		}
		else
		{
			//error missing
		}
	}
	else if (token.type == id)
	{
		//check if it is either a procedure call or a name
		return;
	}
	else if (token.type == sub_op)
	{
		//check if procedure call or name
		token = scanner->ScanToken();

		if (token.type == id)
		{
			Name();
		}
		else if (token.type == literal_int || token.type == literal_float)
		{
			return;
		}
	}
	else if (token.type == literal_string)
	{
		return;
	}
	else if (token.type == bool_true || token.type == bool_false)
	{
		return;
	}
	else
	{
		std::cout << "You done messed up.\n";
	}
	
}

void Parser::Name()
{
	token = scanner->ScanToken();

	if (token.type == id)
	{

	}
	else
	{
		std::cout << "Error, we were expecting an identifier for a variable declaration.\n";
	}
}

void Parser::Number()
{
	token = scanner->ScanToken();

	if (token.type == literal_int || token.type == literal_float)
	{
		return;
	}
}

void Parser::String()
{
	token = scanner->ScanToken();

	if (token.type == literal_string)
	{
		return;
	}
}

void Parser::IfStatement()
{

}

void Parser::LoopStatement()
{

}

void Parser::ReturnStatement()
{
	
}