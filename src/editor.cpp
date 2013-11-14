/*
 * editor.cpp
 *
 *  Created on: 10-nov.-2013
 *      Author: arno
 */

#include "QuestionList.h"
#include <iostream>

int main(int argc, char * args[]){

	std::string fn ("src/vragen.ens");
	QuestionList ql = QuestionList(fn);

	std::cout << "test";
	return 0;
}
