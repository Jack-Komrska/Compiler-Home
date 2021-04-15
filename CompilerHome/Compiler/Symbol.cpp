#include "Symbol.h"
#pragma once

Symbol::Symbol()
{
	identifier = "";
	scopeName = "";
	type = unknown;
	isProc = false;
	isGlob = false;
	isArr = false;
}

std::string Symbol::getIdentifer()
{
	return identifier;
}

std::string Symbol::getScopeName()
{
	return scopeName;
}

int Symbol::getType()
{
	return type;
}

bool Symbol::getProcedure()
{
	return isProc;
}

bool Symbol::getGlobal()
{
	return isGlob;
}

bool Symbol::getArray()
{
	return isArr;
}

void Symbol::setIdentifier(std::string str)
{
	identifier = str;
}

void Symbol::setScopeName(std::string str)
{
	scopeName = str;
}

void Symbol::setType(int input)
{
	type = input;
}

void Symbol::setIsProcedure(bool proc)
{
	isProc = proc;
}

void Symbol::setIsGlobal(bool global)
{
	isGlob = global;
}

void Symbol::setIsArr(bool arr)
{
	isArr = arr;
}
