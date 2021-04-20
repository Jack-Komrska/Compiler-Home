#pragma once
#include "Token.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>

class Scanner 
{
	std::map<std::string, definition> map; // the map

	char currCh; // the current character
	char nextCh; // the next character
	bool progName = true;
	std::ifstream file; // the file
	std::string fileName; // file name - may turn into path
	int lineNum; //line number
	int numGet;
	
public:
	Scanner() {};

	Scanner(std::string fName) 
	{
		lineNum = 1;
		numGet = 0;
		fileName = fName;
		file.open(fileName);
		addMaps();
	};

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
		map["begin"] = key_begin;
		map["end"] = key_end;
		map["variable"] = key_variable;
		map["global"] = key_global;
		map["procedure"] = key_procedure;
		map["for"] = key_for;
		map["if"] = key_if;
		map["then"] = key_then;
		map["not"] = key_not;
		map["return"] = key_return;
		map["else"] = key_else;
		map["|"] = key_or;
		map["&"] = key_and;
		map["const"] = key_const;
		map["integer"] = num_integer;
		map["int"] = num_integer;
		map["bool"] = boolean;
		map["float"] = num_float;
		//map["char"] = character;
		map["string"] = str;
		map["true"] = bool_true;
		map["false"] = bool_false;
		map["=="] = sym_doubEqual; //not searching for symbols atm, may need to at some point
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
		map["!-="] = sym_notEqual;
		map[","] = sym_comma;
	}

	int getLineNum()
	{
		return lineNum;
	}

	bool isAlphaNum(char cha)
	{
		if (isalpha(cha))
		{
			return true;
		}
		else if (isdigit(cha))
		{
			return true;
		}
		else if (cha == '_')
		{
			return true;
		}
		return false;
	}

	void getEndLine()
	{
		while (currCh != '\n') //get to the end of the line
		{
			currCh = file.get();
			numGet++;
		}
	}

	Token CallScanner(bool isScan) //if true scan, if false peek
	{ //middle man for ignoring comments
		Token temp;
		
		do {
			temp = ScanToken(isScan);
			
		} while (temp.type == comment); //only reruns the loop if the type is a comment
		
		return temp;
	}

	Token ScanToken(bool peek) // I know all of the bools for peek should techinically be flipped but I do not want to mess everything up
	{
		Token* token = new Token();

		currCh = file.get(); //get each character individually
		if (!peek) { numGet++; }

		if (currCh == '\n' && peek)
		{
			lineNum += 1;
		}

		while (isspace(currCh))
		{
			currCh = file.get();
			if (!peek) { numGet++; }
		}

		switch (currCh) {
		case '/':
		{
			{
				nextCh = file.get();
				numGet++;
				if (nextCh == '/') //finds the end of the line
				{
					token->type = comment;

					getEndLine();

					while (isspace(currCh))
					{
						currCh = file.get();
						numGet++;
					}

					file.unget();
					numGet--;
				}
				else if (nextCh == '*') //finds the end of the block comment
				{
					token->type = comment;

					int commentBlocks = 1;

					while (commentBlocks > 0)
					{
						currCh = file.get();
						numGet++;
						if (currCh == '/')
						{
							nextCh = file.get();
							numGet++;
							if (nextCh == '*')
							{
								commentBlocks++;
							}
						}
						else if (currCh == '*')
						{
							nextCh = file.get();
							numGet++;
							if (nextCh == '/')
							{
								commentBlocks--;
							}
						}
					}
				}
				else
				{
					file.unget();
					numGet--;
					token->type = definition::div_op;
					token->val.stringVal[0] = currCh;
				}
			}
		}
		break;

		case '(': case ')': case '.': case '=': case ';': case '+': case '*': case '-': case '%': case '[': case ']': case ':': case '<': case '>': case ',':
		{
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
				if (!peek) { numGet++; }
				if (nextCh == '=')
				{
					token->type = definition::sym_doubEqual;
					token->val.stringVal[0] = currCh;
					token->val.stringVal[1] = nextCh;
					if (peek) { numGet--; }
				}
			}
			else if (currCh == ':')
			{
				nextCh = file.get();
				if (!peek) { numGet++; }
				if (nextCh == '=')
				{
					token->type = definition::sym_colEqual;
					token->val.stringVal[0] = currCh;
					token->val.stringVal[1] = nextCh;
					if (peek) { numGet--; }
				}
				else
				{
					file.unget();
					numGet--;
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
				if (!peek) { numGet++; }
				if (nextCh == '=')
				{
					token->type = definition::sym_lessEqual;
					token->val.stringVal[0] = currCh;
					token->val.stringVal[1] = nextCh;
					if (peek) { numGet--; }
				}
				else
				{
					file.unget();
					numGet--;
					token->type = definition::sym_less;
					token->val.stringVal[0] = currCh;
				}
			}
			else if (currCh == '>')
			{
				nextCh = file.get();
				if (!peek) { numGet++; }
				if (nextCh == '=')
				{
					token->type = definition::sym_greatEqual;
					token->val.stringVal[0] = currCh;
					token->val.stringVal[1] = nextCh;
					if (peek) { numGet--; }
				}
				else
				{
					file.unget();
					numGet--;
					token->type = definition::sym_great;
					token->val.stringVal[0] = currCh;
				}
			}
			else if (currCh == ',')
			{
				token->type = definition::sym_comma;
				token->val.stringVal[0] = currCh;
			}
		}
		break;

		case '"':
		{
			token->type = literal_string;
			int j = 0;
			currCh = file.get();
			if (!peek) { numGet++; }
			while (currCh != '"')
			{
				token->val.stringVal[j] = currCh;
				j++;
				currCh = file.get();
				if (!peek) { numGet++; }
			}
		}
		break;

		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
		{
			token->val.stringVal[0] = tolower(currCh);
			std::string word;
			word.push_back(tolower(currCh));
			for (int i = 1; isAlphaNum(currCh = file.get()); i++) //make a function that checks it is either a number or letter (should be pretty simple)
			{
				currCh = tolower(currCh);
				token->val.stringVal[i] = std::tolower(currCh);
				word.push_back(currCh);
				if (!peek) { numGet++; }
			}
			//maybe check if there are numbers in the identifier here?
			file.unget();
			if (map.count(word) > 0)
			{
				token->type = map.find(word)->second;
			}
			else
			{
				if (progName && peek)
				{
					token->type = key_progName;
					progName = false;
				}
				else if (progName && !peek)
				{
					token->type = key_progName;
				}
				else
				{
					token->type = id;
				}
			}
		}
		break;

		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
		{
			token->val.stringVal[0] = tolower(currCh);
			std::string word;
			word.push_back(tolower(currCh));
			for (int i = 1; isAlphaNum(currCh = file.get()); i++) //make a function that checks it is either a number or letter (should be pretty simple)
			{
				token->val.stringVal[i] = tolower(currCh);
				word.push_back(tolower(currCh));
				if (!peek) { numGet++; }
			}
			//maybe check if there are numbers in the identifier here?
			file.unget();

			if (map.count(word) > 0)
			{
				token->type = map.find(word)->second;
			}
			else
			{
				if (progName && peek)
				{
					token->type = key_progName;
					progName = false;
				}
				else if (progName && !peek)
				{
					token->type = key_progName;
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
			std::string number;
			number.push_back(currCh);
			bool isFloat = false;
			while (isdigit(currCh) || currCh == '.')
			{
				currCh = file.get();
				if (currCh == '.')
				{
					token->type = definition::literal_float;
					isFloat = true;
					number.push_back(currCh);
				}
				else
				{
					number.push_back(currCh);
				}
				if (!peek && (isdigit(currCh) || currCh == '.')) { numGet++; }
			}
			file.unget();
			//if (!peek) { numGet--; }

			if (isFloat)
			{
				token->val.floatVal = std::stof(number);
			}
			else
			{
				token->val.intVal = std::stoi(number);
			}
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
		
		if (token->type != eof && token->type != comment)
		{
			if (!peek) { std::cout << "peek: "; }
			std::cout << '<' << token->type << ',';

			if (token->type == literal_int)
			{
				std::cout << token->val.intVal << '>';
			}
			else if (token->type == literal_float)
			{
				std::cout << token->val.floatVal << '>';
			}
			else
			{
				std::cout << token->val.stringVal << '>';
			}
			std::cout << '\n';
		}
		
		if (token->type != comment)
		{
			for (int z = 0; z < numGet; z++)
			{
				file.unget();
			}
		}

		numGet = 0;

		return *token;
	}
};