//
// Created by Owntage on 4/10/2018.
//

#include <axe.h>
#include <vector>
#include "lambda_parser.h"

using namespace axe;
using namespace std;

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

typedef std::string::iterator str_it;

auto variable = r_any('a', 'z') & *(r_any('a', 'z') | r_any('0', '9'));
r_rule<str_it> expression;
auto atom = ('(' & expression & ')') | variable;
auto application = r_many(atom, " ");

void initExpr()
{
	static bool isInitiated = false;

	if (!isInitiated)
	{
		expression = ~application & '\\' & variable & '.' & expression;
		isInitiated = true;
	}
}

LambdaExpr* parseExpression(const std::string& str);

LambdaExpr* parseAtom(const std::string& str)
{
	if (str[0] == '(' && str.back() == ')')
	{
		std::string exprStr = str;
		exprStr = exprStr.substr(1);
		exprStr.pop_back();
		return parseExpression(exprStr);
	}
	return LambdaExpr::createVar(str);
}

LambdaExpr* parseApplication(const std::string& str)
{

}

LambdaExpr* parseExpression(const std::string& str)
{

}

LambdaExpr* parse(const std::string& str)
{
	initExpr();
	return parseExpression(str);
}
