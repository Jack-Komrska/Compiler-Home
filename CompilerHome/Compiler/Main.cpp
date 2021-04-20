#include <iostream>
#include "Token.h"
#include "Scanner.h"
#include <fstream>
#include <string>
#include "Parser.h"

using namespace std;

int main()
{
	//needs to work in a linux environment
	cout << "Please input the path of the file and its name: ";
	string file;
	cin >> file;
	Parser* parser = new Parser(file);

}
