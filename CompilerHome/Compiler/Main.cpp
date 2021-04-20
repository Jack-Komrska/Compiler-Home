#include <iostream>
//#include "Token.h"
//#include "Scanner.h"
#include <fstream>
#include <string>
#include "Parser.h"

using namespace std;

int main()
{
	cout << "Please input the path of the file and its name: ";
	string file;
	cin >> file;
	Parser* parser = new Parser(file);
}
