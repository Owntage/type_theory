//
// Created by Owntage on 4/10/2018.
//

#include "lambda_parser.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main()
{
	string inputFilename;
	cout << "enter input filename: " << endl;
	cin >> inputFilename;

	string outputFilename;
	cout << "enter output filename: " << endl;
	cin >> outputFilename;

	ifstream t(inputFilename);
	string str((std::istreambuf_iterator<char>(t)),
					std::istreambuf_iterator<char>());


	LambdaExpr* expr = LambdaExpr::parse(str);
	while (expr->isReducable())
	{
		expr = expr->reduce();
	}

	ofstream output_f(outputFilename);
	output_f << *expr;

	return 0;
}

