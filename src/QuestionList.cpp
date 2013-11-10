/*
 * QuestionList.cpp
 *
 *  Created on: 10-nov.-2013
 *      Author: arno
 */

#include "QuestionList.h"
#include <iostream>
#include <fstream>
#include <uuid/uuid.h>

QuestionList::QuestionList(std::string filename): filename_(filename) {
	std::ifstream input_file(filename);
	if(!input_file){
		//bestand bestaat niet
		uuid_generate(uuid_);
	}else{
		read_from_file(&input_file);
		input_file.close();
	}

}

QuestionList::~QuestionList() {
	// TODO Auto-generated destructor stub
}

void QuestionList::read_from_file(std::ifstream * input_file){

}

