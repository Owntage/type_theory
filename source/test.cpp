//
// Created by Owntage on 4/10/2018.
//

#include <iostream>
#include <cassert>
#include <conio.h>
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
	LambdaExpr* expr1 = LambdaExpr::parse("(\\f.\\x.f (f x)) (\\n.\\f.\\x.n (\\g.\\h.h (g f)) (\\u.x) (\\u.u)) (\\f.\\x.f (f (f x)))");
	LambdaExpr* expr2 = LambdaExpr::parse(expr1->toString());
	assert(expr1->toString() == expr2->toString());
	cout << "expression, checked for equality: " << *expr1 << endl;

	//substitution test
	LambdaExpr* initExpr = LambdaExpr::parse("\\x.y y");
	LambdaExpr* targetExpr = LambdaExpr::parse("\\x.z z");
	initExpr->substitute("y", LambdaExpr::createVar("z"));
	assert (initExpr->toString() == targetExpr->toString());

	//normalization test
	initExpr = LambdaExpr::parse("(\\x.x) y");
	targetExpr = LambdaExpr::parse("y");
	LambdaExpr* normalizedExpr = initExpr->reduce();
	cout << "normalization result: " << *normalizedExpr << endl;
	assert (normalizedExpr->toString() == targetExpr->toString());

	//normalization test 2
	initExpr = LambdaExpr::parse("(\\x.\\y.y) a");
	targetExpr = LambdaExpr::parse("\\y.y");
	normalizedExpr = initExpr->reduce();
	cout << "normalization result 2: " << *normalizedExpr << endl;
	assert(targetExpr->toString() == normalizedExpr->toString());

	//normalization test 3
	initExpr = LambdaExpr::parse("(\\x.x y) ((\\z.z) a)");
	normalizedExpr = initExpr->reduce();
	cout << "normalization result 3: " << *normalizedExpr << endl;

	cout << "all tests passed" << endl;
	cout.flush();
	getch();
	return 0;
}