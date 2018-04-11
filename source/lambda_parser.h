//
// Created by Owntage on 4/10/2018.
//

#ifndef MATH_LOGIC_HW_LAMBDA_PARSER_H
#define MATH_LOGIC_HW_LAMBDA_PARSER_H

#include <string>
#include "lambda_parser.h"

struct LambdaExpr
{
	std::string value;
	LambdaExpr* left;
	LambdaExpr* right;

	static LambdaExpr* createAbstraction(std::string var, LambdaExpr* expr);
	static LambdaExpr* createApplication(LambdaExpr* left, LambdaExpr* right);
	static LambdaExpr* createVar(std::string var);

	static LambdaExpr* parse(const std::string& input);

	template<typename Stream>
	static Stream& operator<<(Stream& stream, LambdaExpr expr)
	{

	}

	void substitute(std::string var, LambdaExpr* expr);
	LambdaExpr* reduce();



	bool isAbstraction()
	{
		return _isAbstraction;
	}

	bool isApplication()
	{
		return _isApplication;
	}

	bool isVariable()
	{
		return !isAbstraction() && !isApplication();
	}

private:
	bool _isAbstraction;
	bool _isApplication;
};


#endif //MATH_LOGIC_HW_LAMBDA_PARSER_H
