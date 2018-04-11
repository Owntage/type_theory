//
// Created by Owntage on 4/10/2018.
//

#include <iostream>
#include <cassert>
#include "lambda_parser.h"

using namespace std;

int main()
{
	auto expr = LambdaExpr::createAbstraction("x", 
											  LambdaExpr::createApplication(
													  LambdaExpr::createVar("x"),
													  LambdaExpr::createVar("y")
											  ));
	
	cout << "expression: " << *expr << endl;

	cout << "parsedExpression: " << *LambdaExpr::parse("a b c\\y.\\x.x y") << endl;

	//parsing test
	LambdaExpr* expr1 = LambdaExpr::parse("(\\y.\\x.x (y z))");
	LambdaExpr* expr2 = LambdaExpr::parse(expr1->toString());
	assert(expr1->toString() == expr2->toString());
	cout << "expression, checked for equality: " << *expr1 << endl;

	//substitution test
	LambdaExpr* initExpr = LambdaExpr::parse("\\x.y y");
	LambdaExpr* targetExpr = LambdaExpr::parse("\\x.z z");
	initExpr->substitute("x", LambdaExpr::createVar("z"));
	assert (initExpr->toString() == targetExpr->toString());

	cout << "all tests passed" << endl;
	return 0;
}