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

set<string> LambdaExpr::getFreeVariables()
{
	if (isVariable())
	{
		set<string> result;
		result.insert(value);
		return result;
	}
	if (isAbstraction())
	{
		set<string> result = left->getFreeVariables();
		result.erase(value);
		return result;
	}
	if (isApplication())
	{
		set<string> result = left->getFreeVariables();
		set<string> result2 = right->getFreeVariables();
		for (auto it = result2.begin(); it != result2.end(); it++)
		{
			result.insert(*it);
		}
		return result;
	}
	assert (false);
}

LambdaExpr* LambdaExpr::substitute(std::string var, LambdaExpr* expr)
{
	static int counter = 0;
	if (isVariable())
	{
		if (value == var)
		{
			*this = *expr;
		}
	}
	else
	{
		if (isAbstraction())
		{
			set<string> freeVars = expr->getFreeVariables();
			if (freeVars.find(value) != freeVars.end())
			{
				expr = expr->createCopy();
				expr->substitute(value, createVar(value + to_string(counter++)));
			}

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

bool LambdaExpr::isReducable()
{
	if (isApplication() && left->isAbstraction()) return true;
	if (isVariable()) return false;
	if (isAbstraction()) return left->isReducable();
	if (isApplication())
	{
		return left->isReducable() || right->isReducable();
	}
	assert(false); //should not be reached
	return false;
}

LambdaExpr* LambdaExpr::reduce()
{
	if (!isReducable()) return this;
	//string exprStr = toString();
	//if (reductionCache.find(exprStr) != reductionCache.end())
	//{
	//	return reductionCache[exprStr];
	//}

	LambdaExpr* res;
	if (isApplication())
	{
		if (left->isAbstraction())
		{
			res = left->left->createCopy()->substitute(left->value, right);
			//reductionCache[exprStr] = res;
			return res;
		}
		else
		{
			res = createApplication(left->reduce(), right->reduce())->reduce();
			//reductionCache[exprStr] = res;
			return res;
		}
	}
	if (isVariable())
	{
		return createCopy();
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

std::string LambdaExpr::toInfixStr()
{
	if (isAbstraction()) return "abs(" + left->toInfixStr() + ")";
	if (isApplication()) return "app(" + left->toInfixStr()+ ", " + right->toInfixStr() + ")";
	if (isVariable()) return value;
}

typedef std::string::iterator str_it;

//variable | (*r_str("!") & "(" & implication & ")") | ("(" & implication & ")") | (*r_str("!") & variable)

auto variableChar = (r_any('a', 'z') | r_any('0', '9'));
auto variable = variableChar & *variableChar;
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
	std::cout << "parsing appl" << std::endl;

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
	std::cout << "parsing..." << std::endl;
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


LambdaExpr* LambdaExpr::parse(std::string str)
{
	initExpr();
	auto inputRule = expression & r_end();
	std::cout << "started checking input grammar" << std::endl;
	assert(inputRule(str.begin(), str.end()).matched);
	std::cout << "finished checking input grammar" << std::endl;
	return parseExpression(str);
}
