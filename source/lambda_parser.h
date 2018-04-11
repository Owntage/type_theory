//
// Created by Owntage on 4/10/2018.
//

#ifndef MATH_LOGIC_HW_LAMBDA_PARSER_H
#define MATH_LOGIC_HW_LAMBDA_PARSER_H

#include <string>


struct LambdaExpr
{
	std::string value;
	LambdaExpr* left;
	LambdaExpr* right;

	static LambdaExpr* createAbstraction(std::string value, LambdaExpr* expr)
	{
		LambdaExpr* result = new LambdaExpr();
		result->value = value;
		result->left = expr;
		result->isAbstraction = true;
		result->isApplication = false;
	}

	static LambdaExpr* createApplication(LambdaExpr* left, LambdaExpr* right)
	{
		LambdaExpr* result = new LambdaExpr();
	}

private:
	bool isAbstraction;
	bool isApplication;
};

namespace LambdaCalculus
{

}

#endif //MATH_LOGIC_HW_LAMBDA_PARSER_H
