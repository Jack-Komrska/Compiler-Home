#include <iostream>
#include "Token.h"
#include <fstream>
#include <string>

using namespace std;


// not printing each of the identifiers individually, (needs to stop printing out the entire file), need to make a reserved table of some sorts (ask Nick, read some more), add more cases to switch
// and need to add proper logic for each of the cases

static Token ScanToken(ifstream& file, int& lineNum)
{
	char currCh;
	char nextCh;
	Token* token = new Token();
	currCh = file.get(); //get each character individually

	if (currCh == '\n')
	{
		lineNum += 1;
	}

	while (isspace(currCh))
		currCh = file.get();

	switch (currCh) {
	case '/':
		nextCh = file.get();
		if (nextCh == '/') //finds the end of the line
		{
			file.seekg('\n');
		}
		else if (nextCh == '*') //finds the end of the block comment
		{
			file.seekg('*/');
		}
		else
		{
			file.unget();
			token->type = div_op;
			token->val.stringVal[0] = '/';
		}
		break;

	case '(': case ')': case '.': case '=': case ';': case '+': case '*': case '-': case '%':
		if (currCh == '(')
		{
			token->type = sym_lparen;
			token->val.stringVal[0] = '(';
		}
		else if (currCh == ')')
		{
			token->type = sym_rparen;
			token->val.stringVal[0] = ')';
		}
		else if (currCh == '.')
		{
			token->type = dot_op;
			token->val.stringVal[0] = '.';
		}
		else if (currCh == '=')
		{
			nextCh = file.get();
			if (nextCh == '=')
			{
				token->type = equal_op;
				token->val.stringVal[0] = '=';
				token->val.stringVal[1] = '=';
			}
			else
			{
				file.unget();
				token->type = assign_op;
				token->val.stringVal[0] = '=';
			}
		}
		else if (currCh == ';')
		{
			token->type = sym_sc;
			token->val.stringVal[0] = ';';
		}
		else if (currCh == '+')
		{
			token->type = add_op;
			token->val.stringVal[0] = '+';
		}
		else if (currCh == '-')
		{
			token->type = sub_op;
			token->val.stringVal[0] = '-';
		}
		else if (currCh == '*')
		{
			token->type = mult_op;
			token->val.stringVal[0] = '*';
		}
		else if (currCh == '%')
		{
			token->type = rem_op;
			token->val.stringVal[0] = '%';
		}
		break;

	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
		token->type = id;
		token->val.stringVal[0] = currCh;
		break;

	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
		token->type = id;
		token->val.stringVal[0] = currCh;
		break;

	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		token->type = num_integer;
		token->val.intVal = currCh - '0';
		while (isdigit(currCh = file.get()))
			token->val.intVal = token->val.intVal * 10 + currCh - '0';
		file.unget();
		break;

	case EOF:
		token->type = eof;
		break;

	default:
		token->type = unknown;
		cout << "Unknown character: " << currCh << ", on line: " << lineNum << '\n';
		break;
	}

	//cout << currCh;
	if (token->type != eof)
	{
		cout << '<' << token->type << ',';

		if (token->type == num_integer)
		{
			cout << token->val.intVal << '>';
		}
		else if (token->type == num_double)
		{
			cout << token->val.doubleVal << '>';
		}
		else
		{
			cout << token->val.stringVal << '>';
		}
		cout << '\n';
	}
	return *token;
}

int main()
{
	int lineNum = 1;
	ifstream file;
	
	file.open("test.txt");
	if (file.is_open())
	{
		while (ScanToken(file, lineNum).type != eof);
	}
	
	file.close();
}

