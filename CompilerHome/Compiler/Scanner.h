#pragma once
#include "Token.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>

class Scanner {

	std::map<std::string, definition> map; // the map

	char currCh; // the current character
	char nextCh; // the next character
	bool progName = true;
	std::ifstream file; // the file
	std::string fileName; // file name - may turn into path
	int lineNum; //line number
	
public:
	Scanner(std::string fName)
	{
		lineNum = 1;
		fileName = fName;
	}

	void openFile()
	{
		file.open(fileName);
	}

	void closeFile()
	{
		file.close();
	}

	void addMaps()
	{
		map["."] = key_period;
		map["program"] = key_program;
		map["is"] = key_is;
		map["begin"] = key_progBegin;
		map["end"] = key_progEnd;
		map["variable"] = key_variable;
		map["global"] = key_global;
		map["procedure"] = key_procedure;
		map["for"] = key_for;
		map["if"] = key_if;
		map["return"] = key_return;
		map["else"] = key_else;
		map["or"] = key_or;
		map["and"] = key_and;
		map["const"] = key_const;
		map["int"] = num_integer;
		map["bool"] = boolean;
		map["double"] = num_double;
		map["char"] = character;
		map["string"] = str;
		map["=="] = equal_op; //not searching for symbols atm, may need to at some point
		map[":="] = sym_colEqual;
		map["*"] = mult_op;
		map["+"] = add_op;
		map["-"] = sub_op;
		map["/"] = div_op;
		map["%"] = rem_op;
		map["["] = sym_lbrack;
		map["]"] = sym_rbrack;
		map["("] = sym_lparen;
		map[")"] = sym_rparen;
		map["<"] = sym_less;
		map[">"] = sym_great;
		map["<="] = sym_lessEqual;
		map[">="] = sym_greatEqual;
		
	}

	Token ScanToken()
	{
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
				token->type = definition::div_op;
				token->val.stringVal[0] = currCh;
			}
			break;

		case '(': case ')': case '.': case '=': case ';': case '+': case '*': case '-': case '%': case '[': case ']': case ':': case '<': case '>':
			if (currCh == '(')
			{
				token->type = definition::sym_lparen;
				token->val.stringVal[0] = currCh;
			}
			else if (currCh == ')')
			{
				token->type = definition::sym_rparen;
				token->val.stringVal[0] = currCh;
			}
			else if (currCh == '.')
			{
				token->type = definition::key_period;
				token->val.stringVal[0] = currCh;
			}
			else if (currCh == '=')
			{
				nextCh = file.get();
				if (nextCh == '=')
				{
					token->type = definition::equal_op;
					token->val.stringVal[0] = currCh;
					token->val.stringVal[1] = nextCh;
				}
			}
			else if (currCh == ':')
			{
				nextCh = file.get();
				if (nextCh == '=')
				{
					token->type = definition::sym_colEqual;
					token->val.stringVal[0] = currCh;
					token->val.stringVal[1] = nextCh;
				}
				else
				{
					file.unget();
					token->type = definition::sym_colon;
					token->val.stringVal[0] = currCh;
				}
			}
			else if (currCh == ';')
			{
				token->type = definition::sym_sc;
				token->val.stringVal[0] = currCh;
			}
			else if (currCh == '+')
			{
				token->type = definition::add_op;
				token->val.stringVal[0] = currCh;
			}
			else if (currCh == '-')
			{
				token->type = definition::sub_op;
				token->val.stringVal[0] = currCh;
			}
			else if (currCh == '*')
			{
				token->type = definition::mult_op;
				token->val.stringVal[0] = currCh;
			}
			else if (currCh == '%')
			{
				token->type = definition::rem_op;
				token->val.stringVal[0] = currCh;
			}
			else if (currCh == '[')
			{
				token->type = definition::sym_lbrack;
				token->val.stringVal[0] = currCh;
			}
			else if (currCh == ']')
			{
				token->type = definition::sym_rbrack;
				token->val.stringVal[0] = currCh;
			}
			else if (currCh == '<')
			{
				nextCh = file.get();
				if (nextCh == '=')
				{
					token->type = definition::sym_lessEqual;
					token->val.stringVal[0] = currCh;
					token->val.stringVal[1] = nextCh;
				}
				else
				{
					token->type = definition::sym_less;
					token->val.stringVal[0] = currCh;
				}
			}
			else if (currCh == '>')
			{
				nextCh = file.get();
				if (nextCh == '=')
				{
					token->type = definition::sym_greatEqual;
					token->val.stringVal[0] = currCh;
					token->val.stringVal[1] = nextCh;
				}
				else
				{
					token->type = definition::sym_great;
					token->val.stringVal[0] = currCh;
				}
			}
			break;

		case '"':
		{
			token->type = literal_string;
			int j = 0;
			currCh = file.get();
			while (currCh != '"')
			{
				token->val.stringVal[j] = currCh;
				j++;
				currCh = file.get();
			}
		}
		break;

		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
		{
			token->val.stringVal[0] = currCh;
			std::string word = std::to_string(currCh);
			for (int i = 1; isalpha(currCh = file.get()); i++)
			{
				token->val.stringVal[i] = std::tolower(currCh);
				word.append(std::to_string(currCh));
			}
			file.unget();

			for (int i = 0; i < word.size(); i++)
			{
				tolower(word[i]);
			}

			if (map.count(word) > 0)
			{
				token->type = map.find(word)->second;
			}
			else
			{
				token->type = id;
			}

		}
		break;

		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
		{
			token->val.stringVal[0] = currCh;
			std::string word;
			word.push_back(currCh);
			for (int i = 1; isalpha(currCh = file.get()); i++)
			{
				token->val.stringVal[i] = std::tolower(currCh);
				word.push_back(currCh);
			}
			//maybe check if there are numbers in the identifier here?
			file.unget();

			for (int i = 0; i < word.size(); i++)
			{
				tolower(word[i]);
			}

			if (map.count(word) > 0)
			{
				token->type = map.find(word)->second;
			}
			else
			{
				if (progName)
				{
					token->type = key_progName;
					progName = false;
				}
				else
				{
					token->type = id;
				}

			}

		}
		break;

		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		{
			token->type = definition::literal_int;
			token->val.intVal = currCh - '0';
			while (isdigit(currCh = file.get()))
				token->val.intVal = token->val.intVal * 10 + currCh - '0';
			file.unget();
		}
			break;

		case EOF:
		{
			token->type = definition::eof;
		}
			break;

		default:
		{
			token->type = definition::unknown;
			std::cout << "Unknown character: " << currCh << ", on line: " << lineNum << '\n';
		}
			break;
		}

		if (token->type != eof)
		{
			std::cout << '<' << token->type << ',';

			if (token->type == literal_int)
			{
				std::cout << token->val.intVal << '>';
			}
			else if (token->type == literal_double)
			{
				std::cout << token->val.doubleVal << '>';
			}
			else
			{
				std::cout << token->val.stringVal << '>';
			}
			std::cout << '\n';
		}
		return *token;
	}
};