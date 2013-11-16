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


	Question(int id, QuestionType type, std::string& question, std::string * answers, int amount_of_answers);
	Question(int id, QuestionType type, std::string& question);
	Question();
	~Question();
	void set_answers(std::string* answers);
	void set_question_string(std::string& new_question);
	std::string get_question_file_string();
	std::string get_string();
	static std::string get_type_string(QuestionType type);


	bool copied;

private:
	int id_;
	QuestionType type_;
	std::string question_string_;
	std::string * answers_;
	int amount_of_answers_;
	int number_of_answers();


};

std::ostream& operator<< (std::ostream& out, Question& q);

#endif /* QUESTION_H_ */
