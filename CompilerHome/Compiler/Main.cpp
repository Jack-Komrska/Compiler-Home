#include <iostream>
#include "Token.h"
#include "Scanner.h"
#include <fstream>
#include <string>

using namespace std;

int main()
{
	Scanner test("test.txt"); //initializes the object, need to add the user selecting the file
	test.addMaps(); // adds the maps
	test.openFile(); // opens the file
	while (test.ScanToken().type != eof)
	{

	};
	test.closeFile(); // closes the file
}

