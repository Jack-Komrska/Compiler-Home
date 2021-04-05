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

	token = scanner->CallScanner();

	if (token.type == key_period)
	{
		std::cout << "End of program.\n";
	}
	else
	{
		//error .
	}
}

void Parser::ProgramHeader() // add elses for errors
{
	token = scanner->CallScanner();

	if (token.type == key_program)
	{
		token = scanner->CallScanner();

		if (token.type == key_progName)
		{
			token = scanner->CallScanner();

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
	Declaration(); //need some form of a while to keep on reading in declarations

	if (token.type == key_progBegin) //second check
	{
		Statement(); //need some form of a while to keep on reading in statements
	}

	token = scanner->CallScanner();

	if (token.type == key_end)
	{
		token = scanner->CallScanner();

		if (token.type == key_program)
		{
			
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
	while (token.type != key_progBegin) 
	{
		token = scanner->CallScanner(); //scans in the scope of variable/procedure

		if (token.type == key_progBegin)
		{
			break;
		}

		//will interact with symbol table and check the scope

		token = scanner->CallScanner(); //determines if it is either a procedure/variable

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
	token = scanner->CallScanner(); //scanning for the identifier

	if (token.type == id)
	{
		token = scanner->CallScanner(); //scanning for the colon

		if (token.type == sym_colon)
		{
			token = scanner->CallScanner(); //scanning for the type mark
			definition type = token.type;

			if (TypeMark(token.type))
			{
				token = scanner->CallScanner();

				if (token.type == sym_lbrack)
				{
					token = scanner->CallScanner();

					if (token.type == literal_int)
					{
						token = scanner->CallScanner();

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
	token = scanner->CallScanner();

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
	token = scanner->CallScanner();

	if (token.type == id)
	{


		return;
	}
	else
	{
		std::cout << "Error, we were expecting an identifier for a variable declaration.\n";
	}
}

void Parser::Expression()
{
	ArithOp();

	SubExpression();
}

void Parser::SubExpression()
{
	token = scanner->CallScanner();

	if (token.type == key_and)
	{

		return;
	}
	else if (token.type == key_or)
	{

		return;
	}
	else if (token.type == key_not)
	{

		return;
	}
	else
	{
		std::cout << "Error, we were expecting a form of expression.\n";
	}
}

void Parser::ArithOp()
{
	Relation();

	SubArithOp();
}

void Parser::SubArithOp()
{
	token = scanner->CallScanner();

	if (token.type == add_op)
	{

		return;
	}
	else if (token.type == sub_op)
	{

		return;
	}
	else
	{
		std::cout << "Error, we were expecting an arithmetic operator.\n";
	}
}

void Parser::Relation()
{
	Term();

	SubRelation();
}

void Parser::SubRelation()
{
	token = scanner->CallScanner();

	if (token.type == sym_less)
	{

		return;
	}
	else if (token.type == sym_lessEqual)
	{

		return;
	}
	else if (token.type == sym_great)
	{

		return;
	}
	else if (token.type == sym_greatEqual)
	{

		return;
	}
	else if (token.type == equal_op)
	{

		return;
	}
	else if (token.type == sym_notEqual)
	{
		
		return;
	}
}

void Parser::Term()
{
	Factor();

	//token = scanner->CallScanner();
	
	//if (token.type == mult_op || token.type == div_op)
	//{
	SubTerm();
	//}
}

void Parser::SubTerm()
{
	token = scanner->CallScanner();

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
	token = scanner->CallScanner();

	if (token.type == sym_lparen)
	{
		Expression();

		token = scanner->CallScanner();
		if (token.type == sym_rparen)
		{
			return;
		}
		else
		{
			std::cout << "Error, we were expecting a ')'.\n";
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
		token = scanner->CallScanner();

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
	token = scanner->CallScanner();

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
	token = scanner->CallScanner();

	if (token.type == literal_int || token.type == literal_float)
	{
		return;
	}
}

void Parser::String()
{
	token = scanner->CallScanner();

	if (token.type == literal_string)
	{
		return;
	}
}

void Parser::IfStatement()
{
	token = scanner->CallScanner();

	if (token.type == sym_lparen)
	{
		Expression();

		token = scanner->CallScanner();

		if (token.type == sym_rparen)
		{
			token = scanner->CallScanner();

			if (token.type == key_then)
			{
				Statement();

				token = scanner->CallScanner();

				if (token.type == sym_sc) //will need to account for multiple statements and to account for potential else statements
				{
					token = scanner->CallScanner();

					if (token.type == key_end)
					{
						token = scanner->CallScanner();

						if (token.type == key_if)
						{

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
				else if (token.type == key_else) //while loop for multiple elses? i think
				{
					Statement();

					token = scanner->CallScanner();

					if (token.type == sym_sc)
					{

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
	token = scanner->CallScanner();

	if (token.type == sym_lparen)
	{
		AssignmentStatement();

		token = scanner->CallScanner();

		if (token.type == sym_sc)
		{
			Expression();

			token = scanner->CallScanner();

			if (token.type == sym_rparen)
			{
				//while loop for statements
				Statement();

				token = scanner->CallScanner();

				if (token.type == key_end)
				{
					token = scanner->CallScanner();

					if (token.type == key_for)
					{

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