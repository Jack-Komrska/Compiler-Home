#pragma once
//operators, keyword, identifier, numeral, character, str
enum definition
{
	id, num_integer, num_float, /*character,*/ str, boolean, literal_int, literal_float, literal_char, literal_string, bool_true, bool_false, equal_op, mult_op, add_op, sub_op, exp_op, div_op, rem_op, key_period, key_program, key_progName, key_is, key_progBegin, key_end, key_variable, key_global, key_procedure, key_for, key_if, key_then, key_return, key_else, key_not, key_and, key_or, key_const, sym_sc, sym_colon, sym_colEqual, sym_lbrack, sym_rbrack, sym_lparen, sym_rparen, sym_less, sym_great, sym_lessEqual, sym_greatEqual, sym_notEqual, eof, comment, unknown
};

class Token { //token type class, holds what token it is and the value
public:
	definition type;
	union { //holds the value depending on the type
		char stringVal[256];
		int intVal;
		double doubleVal;
	} val;
};