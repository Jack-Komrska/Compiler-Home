#include "Symbol.h"
#include <iostream>
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

std::vector<Symbol> Symbol::getChildren()
{
	return children;
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

void Symbol::addChild(Symbol child)
{
	children.push_back(child);
}

void Symbol::printTree()
{
	std::cout << identifier + ": ";
	for (auto child : children)
	{
		std::cout << child.getIdentifer() << ", ";
	}
	std::cout << std::endl;
}