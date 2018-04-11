//
// Created by Owntage on 4/10/2018.
//

#include "lambda_parser.h"

LambdaExpr::LambdaExpr() :
	_isAbstraction(false),
	_isApplication(false),
	left(nullptr),
	right(nullptr)
{}

LambdaExpr* LambdaExpr::createAbstraction(std::string var, LambdaExpr* expr)
{
	LambdaExpr* result = new LambdaExpr();
	result->value = var;
	result->left = expr;
	result->_isAbstraction = true;
	result->_isApplication = false;
	return result;
}

LambdaExpr* LambdaExpr::createApplication(LambdaExpr* left, LambdaExpr* right)
{
	LambdaExpr* result = new LambdaExpr();
	result->left = left;
	result->right = right;
	result->_isAbstraction = false;
	result->_isApplication = true;
	return result;
}

LambdaExpr* LambdaExpr::createVar(std::string var)
{
	LambdaExpr* result = new LambdaExpr();
	result->value = var;
	return result;
}

std::string LambdaExpr::toString()
{
	std::stringstream s;
	s << *this;
	return s.str();
}



