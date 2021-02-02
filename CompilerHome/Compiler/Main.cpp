#include <iostream>
#include "Token.h"
#include <fstream>
#include <string>

using namespace std;

int main()
{
	char currCh;
	char nextCh;
	int lineNum = 1;
	ifstream file;
	file.open("test.txt");
	if (file.is_open())
	{
		while (!file.eof())
		{
			currCh = file.get(); //get each character individually
			
			//file.unget(); //to make the reader go back the previous character

			if (currCh == '\n')
			{
				//cout << " - " << lineNum; //prints the current line number
				lineNum += 1; //updates the current line number
			}

			cout << currCh;
		}
	}
	
	file.close();
}
