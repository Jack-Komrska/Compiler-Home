#include <string>

using namespace std;
//operators, keyword, identifier, numeral, character, str
enum reservedWord {id, num_integer, num_double, character, str, assign_op, mult_op, add_op, sub_op, exp_op, div_op, rem_op, key_for, key_if, key_return, key_else, key_and, key_or, key_const, sym_sc, sym_lparen, sym_rparen, eof, unknown};

class Token { //token type class, holds what token it is and the value
public:
	reservedWord type;
	union { //holds the value depending on the type
		char stringVal[256];
		int intVal;
		double doubleVal;
	} val;
};