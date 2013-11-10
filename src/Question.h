/*
 * Question.h
 *
 *  Created on: 10-nov.-2013
 *      Author: arno
 */
#include <iostream>
#include <sstream>

#ifndef QUESTION_H_
#define QUESTION_H_


class Question {
public:
	enum QuestionType {
		CHOICE,
		TEXT
	};


	Question(int id, QuestionType type, std::string question, std::string answers[]);
	void set_answers(std::string answers[]);
	void set_question_string(std::string new_question);
	std::string get_question_file_string();
	static std::string get_type_string(QuestionType type);


private:
	int id_;
	QuestionType type_;
	std::string question_string_;
	std::string * answers_;

	int number_of_answers();


};

#endif /* QUESTION_H_ */
