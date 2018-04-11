//
// Created by Owntage on 4/10/2018.
//

#include <iostream>
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
	
	return 0;
}