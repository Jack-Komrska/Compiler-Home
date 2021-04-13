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

void Symbol::setType(int type)
{
	type = type;
}

void Symbol::setIsProcedure(bool isProc)
{
	isProc = isProc;
}

void Symbol::setIsGlobal(bool isGlob)
{
	isGlob = isGlob;
}

void Symbol::setIsArr(bool isArr)
{
	isArr = isArr;
}
