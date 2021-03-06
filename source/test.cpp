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
	assert(normalizedExpr->toString() == targetExpr->toString());

	//normalization test 2
	initExpr = LambdaExpr::parse("(\\x.\\y.y) a");
	targetExpr = LambdaExpr::parse("\\y.y");
	normalizedExpr = initExpr->reduce();
	cout << "normalization result 2: " << *normalizedExpr << endl;
	assert(targetExpr->toString() == normalizedExpr->toString());

	//normalization test 3
	initExpr = LambdaExpr::parse("(\\x.x y) ((\\z.z) a)");
	normalizedExpr = initExpr->reduce();
	targetExpr = LambdaExpr::parse("a y");
	while (normalizedExpr->isReducable())
	{
		normalizedExpr = normalizedExpr->reduce();
	}
	assert(targetExpr->toString() == normalizedExpr->toString());
	cout << "normalization result 3: " << *normalizedExpr << endl;
	cout << "normalization 3 infix: " << normalizedExpr->toInfixStr() << endl;

	//normalization test 4
	initExpr = LambdaExpr::parse("(\\f.\\x.f (f x)) (\\n.\\f.\\x.n (\\g.\\h.h (g f)) (\\u.x) (\\u.u)) (\\f.\\x.f (f (f x)))");
	std::cout << "reducing: " << std::endl;
	while (initExpr->isReducable())
	{
		initExpr = initExpr->reduce();
	}
	cout << "normalization result 4: " << *initExpr << endl;

	//normalization test 5
	initExpr = LambdaExpr::parse("\\10.((\\11.((\\12.((11)))))) x y z");
	initExpr = initExpr->reduce();
	std::cout << "normalization resut 5:: " << *initExpr << std::endl;

	cout << "all tests passed" << endl;


	//parse2 test:

	initExpr = LambdaExpr::parse2("\\x.x x");
	cout << "parse2: " <<  initExpr->toInfixStr() << endl;

	getch();
	return 0;
}