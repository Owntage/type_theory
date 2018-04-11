//
// Created by Owntage on 4/10/2018.
//

#ifndef MATH_LOGIC_HW_LAMBDA_PARSER_H
#define MATH_LOGIC_HW_LAMBDA_PARSER_H

#include <string>
#include <sstream>
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

	void substitute(std::string var, LambdaExpr* expr);
	LambdaExpr* reduce();
	std::string toString();

	bool isAbstraction() { return _isAbstraction; }
	bool isApplication() { return _isApplication; }
	bool isVariable() { return !isAbstraction() && !isApplication(); }

	template<typename Stream>
	static Stream& operator<<(Stream& stream, LambdaExpr expr)
	{
		if (expr.isAbstraction())
		{
			stream << "\\" << expr.value << ".(" << *expr.left << ")";
		}
		if (expr.isApplication())
		{
			stream << "(" << *expr.left << ")(" << *expr.right << ")";
		}
	}

private:
	bool _isAbstraction;
	bool _isApplication;
};


#endif //MATH_LOGIC_HW_LAMBDA_PARSER_H
