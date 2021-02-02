#include <string>

using namespace std;

enum partOfSpeech {operators, keyword, identifier, numeral, character, str};

class TokenType { //token type class, holds what token it is and the value
	partOfSpeech token;
	union { //holds the value depending on the type
		char stringVal[256];
		int intVal;
		double doubleVal;
	} val;
};