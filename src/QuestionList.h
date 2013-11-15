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

	void list(std::ostream * out);
	int add(Question::QuestionType type, std::string& question_string,
			std::string *answers, int amount_of_answers);
	int add(Question::QuestionType type, std::string& question_string,
			std::string *answers, int amount_of_answers, int position);
	void edit(int question_number, std::string new_question_string);
	void edit_choice(int question_number, std::string new_answers[]);
	void delete_question(int question_number);
	void save();
	bool dirty;

private:
	std::string filename_;
	Question * questions_;
	uuid_t uuid_;

	int amount_of_questions_;

	void read_from_file(std::ifstream * input_file);
	void write_to_file();

};

#endif /* QUESTIONLIST_H_ */
