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

	LambdaExpr();

	bool isReducable();
	LambdaExpr* substitute(std::string var, LambdaExpr* expr);
	LambdaExpr* reduce();
	LambdaExpr* createCopy();
	std::string toString();
	std::string toInfixStr();

	bool isAbstraction() const { return _isAbstraction; }
	bool isApplication() const { return _isApplication; }
	bool isVariable() const { return !isAbstraction() && !isApplication(); }

	template<typename Stream>
	friend Stream& operator<<(Stream& stream, const LambdaExpr& expr)
	{
		if (expr.isAbstraction())
		{
			stream << "\\" << expr.value << ".(" << *expr.left << ")";
		}
		if (expr.isApplication())
		{
			stream << "(" << *expr.left << ") (" << *expr.right << ")";
		}
		if (expr.isVariable())
		{
			stream << expr.value;
		}
		return stream;
	}

private:
	bool _isAbstraction;
	bool _isApplication;
};


#endif //MATH_LOGIC_HW_LAMBDA_PARSER_H
