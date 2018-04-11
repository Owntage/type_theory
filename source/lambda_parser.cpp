//
// Created by Owntage on 4/10/2018.
//

#include "lambda_parser.h"

LambdaExpr* LambdaExpr::createAbstraction(std::string var, LambdaExpr* expr)
{
	LambdaExpr* result = new LambdaExpr();
	result->value = var;
	result->left = expr;
	result->_isAbstraction = true;
	result->_isApplication = false;
}

