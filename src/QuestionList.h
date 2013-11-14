/*
 * QuestionList.h
 *
 *  Created on: 10-nov.-2013
 *      Author: arno
 */
#include "Question.h"
#include <iostream>
#include <fstream>
#include <uuid/uuid.h>
#ifndef QUESTIONLIST_H_
#define QUESTIONLIST_H_

class QuestionList {
public:
	QuestionList(std::string& filename);
	~QuestionList();

	void list();
	void add(Question::QuestionType type, std::string question_string, int position);
	void add(Question::QuestionType type, std::string question_string);
	void edit(int question_number, std::string new_question_string);
	void edit_choice(int question_number, std::string new_answers[]);
	void delete_question(int question_number);
	void save();


private:
	bool dirty_;
	std::string filename_;
	Question * questions_;
	uuid_t uuid_;


	void read_from_file(std::ifstream * input_file);
	void write_to_file();

};

#endif /* QUESTIONLIST_H_ */
