//
// Created by Owntage on 4/10/2018.
//

#include <axe.h>
#include <map>
#include <iostream>
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

LambdaExpr* LambdaExpr::substitute(std::string var, LambdaExpr* expr)
{
	if (isVariable())
	{
		*this = *expr;
	}
	else
	{
		if (isAbstraction())
		{
			left->substitute(var, expr);
		}
		else
		{
			left->substitute(var, expr);
			right->substitute(var, expr);
		}
	}
	return this;
}

LambdaExpr* LambdaExpr::createCopy()
{
	LambdaExpr* res = new LambdaExpr(*this);
	if (left != nullptr) res->left = left->createCopy();
	if (right != nullptr) res->right = right->createCopy();

	return res;
}

map<string, LambdaExpr*> reductionCache;

LambdaExpr* LambdaExpr::reduce()
{
	string exprStr = toString();
	if (reductionCache.find(exprStr) != reductionCache.end())
	{
		return reductionCache[exprStr];
	}

	LambdaExpr* res;
	if (isApplication())
	{
		if (left->isAbstraction())
		{
			res = left->left->createCopy()->substitute(left->value, right);
			reductionCache[exprStr] = res;
			return res;
		}
		else
		{
			res = createApplication(left->reduce(), right->reduce())->reduce();
			reductionCache[exprStr] = res;
			return res;
		}
	}
	if (isVariable())
	{
		return this;
	}
	if (isAbstraction())
	{
		return createAbstraction(value, left->reduce());
	}

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
r_rule<str_it> abstraction;
r_rule<str_it> atom;

auto application = r_many(atom, " ");

void initExpr()
{
	static bool isInitiated = false;

	if (!isInitiated)
	{
		abstraction = ~application & '\\' & variable & '.' & expression;
		expression = abstraction | application;
		atom = ("(" & expression & ")") | variable | ("(" & atom & ")");
		isInitiated = true;
	}
}

LambdaExpr* parseExpression(std::string str);

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

LambdaExpr* parseApplication(std::string str)
{
	vector<string> atoms;
	auto atomRule = atom >> e_push_back(atoms);
	auto applicationRule = r_many(atomRule, " ");

	applicationRule(str.begin(), str.end());

	assert(atoms.size() > 0);

	LambdaExpr* result = parseAtom(atoms[0]);
	for (int i = 1; i < atoms.size(); i++)
	{
		result = LambdaExpr::createApplication(result, parseAtom(atoms[i]));
	}

	return result;
}

LambdaExpr* parseExpression(std::string str)
{
	if (abstraction(str.begin(), str.end()).matched)
	{
		string appStr;
		string varStr;
		auto applicationRule = application >> appStr;
		auto variableRule = variable >> varStr;

		auto rule = ~applicationRule & "\\" & variableRule & ".";
		auto matchRes = rule(str.begin(), str.end());

		string innerExprStr(matchRes.position, str.end());

		if (appStr.size() > 0)
		{
			return LambdaExpr::createApplication(
					parseApplication(appStr),
					LambdaExpr::createAbstraction(
							varStr,
							parseExpression(innerExprStr))
			);
		}
		else
		{
			return LambdaExpr::createAbstraction(varStr, parseExpression(innerExprStr));
		}

	}
	else
	{
		return parseApplication(str);
	}
}


LambdaExpr* LambdaExpr::parse(const std::string& str)
{
	initExpr();
	return parseExpression(str);
}
