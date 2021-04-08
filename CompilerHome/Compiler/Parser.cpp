#pragma once
#include "Parser.h"
#include "Scanner.h"
#include <iostream>

//*********ERRORS GRAB LINENUM FROM THE SCANNER**********
//*********ADD TEMP TOKEN EVERYWHERE ELSE****************


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

		token = scanner->CallScanner(true);

		//will interact with symbol table and check the scope

		tempToken = scanner->CallScanner(false); //determines if it is either a procedure/variable

		if (tempToken.type == key_procedure)
		{
			token = scanner->CallScanner(true);
			ProcedureDeclaration();
		}
		else if (tempToken.type == key_variable)
		{
			token = scanner->CallScanner(true);
			VariableDeclaration();
		}
		else
		{
			std::cout << "Error, the token we were expecting was not a procedure or a variable.\n";
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
		token = scanner->CallScanner(true); //scanning for the colon

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

					tempToken = scanner->CallScanner(false);

					if (tempToken.type == literal_int) //bound check
					{
						token = scanner->CallScanner(true);

						Number();

						tempToken = scanner->CallScanner(false);

						if (tempToken.type == sym_rbrack)
						{
							token = scanner->CallScanner(true);

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
				else if (tempToken.type == sym_sc) //end the declaration, semi colon
				{
					token = scanner->CallScanner(true);

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

	Destination();
	
	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == sym_colEqual)
	{
		token = scanner->CallScanner(true);
	}
	else
	{
		//error :=
	}

	Expression();
}

void Parser::Destination()
{
	if (token.type == id) //already scanned
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
	//std::cout << "Expression\n"; //for testing
	ArithOp();

	SubExpression();
}

void Parser::SubExpression()
{
	//std::cout << "SubExpression\n";
	Token tempToken = scanner->CallScanner(false);
	
	if (tempToken.type == key_and)
	{
		token = scanner->CallScanner(true);

		ArithOp();

		return;
	}
	else if (tempToken.type == key_or)
	{
		token = scanner->CallScanner(true);

		ArithOp();

		return;
	}
	else if (tempToken.type == key_not)
	{
		token = scanner->CallScanner(true);

		ArithOp();

		return;
	}
	/*else
	{
		std::cout << "Error, we were expecting a form of expression.\n";
	}*/
}

void Parser::ArithOp()
{
	Relation();

	SubArithOp();
}

void Parser::SubArithOp()
{
	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == add_op)
	{
		token = scanner->CallScanner(true);

		Relation();

		return;
	}
	else if (tempToken.type == sub_op)
	{
		token = scanner->CallScanner(true);

		Relation();

		return;
	}
	/*else
	{
		std::cout << "Error, we were expecting an arithmetic operator.\n";
	}
	*/
}

void Parser::Relation()
{
	Term();

	SubRelation();
}

void Parser::SubRelation()
{
	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == sym_less)
	{
		token = scanner->CallScanner(true);

		Term();

		return;
	}
	else if (tempToken.type == sym_lessEqual)
	{
		token = scanner->CallScanner(true);

		Term();

		return;
	}
	else if (tempToken.type == sym_great)
	{
		token = scanner->CallScanner(true);

		Term();

		return;
	}
	else if (tempToken.type == sym_greatEqual)
	{
		token = scanner->CallScanner(true);

		Term();

		return;
	}
	else if (tempToken.type == equal_op)
	{
		token = scanner->CallScanner(true);

		Term();

		return;
	}
	else if (tempToken.type == sym_notEqual)
	{
		token = scanner->CallScanner(true);

		Term();

		return;
	}
}

void Parser::Term()
{
	Factor();

	SubTerm();
}

void Parser::SubTerm()
{
	Token tempToken = scanner->CallScanner(false);

	if (tempToken.type == mult_op || tempToken.type == div_op)
	{
		token = scanner->CallScanner(true);

		Factor();

		SubTerm();
		
		return;
	}
}

void Parser::Factor()
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
			return;
		}
		else
		{
			std::cout << "Error, we were expecting a ')'.\n";
		}
	}
	else if (tempToken.type == id)
	{
		//check if it is either a procedure call or a name, symbol table look up
		token = scanner->CallScanner(true);



		return;
	}
	else if (tempToken.type == sub_op)
	{
		//check if procedure call or name
		token = scanner->CallScanner(true);

		tempToken = scanner->CallScanner(false);

		if (tempToken.type == id)
		{
			token = scanner->CallScanner(true);
			Name();
		}
		else if (tempToken.type == literal_int || tempToken.type == literal_float)
		{
			token = scanner->CallScanner(true);

			Number();

			return;
		}
	}
	else if (tempToken.type == literal_int || tempToken.type == literal_float)
	{
		token = scanner->CallScanner(true);

		Number();

		return;
	}
	else if (tempToken.type == literal_string)
	{
		token = scanner->CallScanner(true);

		String();

		return;
	}
	else if (tempToken.type == bool_true || tempToken.type == bool_false)
	{
		token = scanner->CallScanner(true);

		return;
	}
	else
	{
		std::cout << "Error, Factor.\n";
	}
	
}

void Parser::Name()
{
	if (token.type == id)
	{

		return;
	}
	else
	{
		std::cout << "Error, we were expecting an identifier for a variable declaration.\n";
	}
}

void Parser::Number()
{

	if (token.type == literal_int || token.type == literal_float)
	{
		
		return;
	}
}

void Parser::String()
{
	if (token.type == literal_string)
	{

		return;
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