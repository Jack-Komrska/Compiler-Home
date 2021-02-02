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

	while (!file.eof())
	{
		currCh = file.get(); //get each character individually

		switch (currCh) {
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
			token->type = id;
			token->val.stringVal[0] = currCh;
		}

		//file.unget(); //to make the reader go back the previous character

		if (currCh == '\n')
		{
			//cout << " - " << linenum; //prints the current line number
			lineNum += 1; //updates the current line number
		}

		cout << currCh;
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
		Token temp;
		temp = ScanToken(file, lineNum);
		cout << temp.type;
		cout << temp.val.stringVal;
	}
	
	file.close();
}

