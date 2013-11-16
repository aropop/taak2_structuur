/*
 * editor.cpp
 *
 *  Created on: 10-nov.-2013
 *      Author: arno
 */

#include "QuestionList.h"
#include "Parser.h"
#include <iostream>

int main(int argc, char * args[]){


	std::string fn ("src/vragen.ens");
	QuestionList ql = QuestionList(fn);
	Parser p(&std::cin, &std::cout, &ql);
	return 0;
}
