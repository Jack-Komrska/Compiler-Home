#pragma once
#include "Parser.h"
#include "Scanner.h"
#include <iostream>


Parser::Parser()
{
	scanner = new Scanner("tst.txt");

	Program();

	scanner->closeFile();
}

void Parser::Program()
{
	ProgramHeader();

	//ProgramBody();
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
		}
	}
}

