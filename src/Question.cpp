/*
 * Question.cpp
 *
 *  Created on: 10-nov.-2013
 *      Author: arno
 */

#include "Question.h"

Question::Question(int id, QuestionType type, std::string& question,
		std::string * answers) :
		id_(id), type_(type), question_string_(question), answers_(answers) {

}

Question::Question(int id, QuestionType type, std::string& question) :
		id_(id), type_(type), question_string_(question) {
	answers_ = NULL;
}

Question::Question(void): answers_(NULL)  {

}

Question::~Question() {
	if (answers_ != NULL) {
		delete[] answers_;
	}
}

void Question::set_question_string(std::string new_question) {
	question_string_ = new_question;
}

void Question::set_answers(std::string answers[]) {
	if (type_ == CHOICE) {
		answers_ = answers;
	} else {
		throw std::string("Cannot set answers for an not CHOICE question");
	}
}

int Question::number_of_answers() {
	return 0;
}

std::string Question::get_type_string(Question::QuestionType type) {
	std::string return_string;
	switch (type) {
	case Question::CHOICE:
		return_string = std::string("CHOICE");
		break;
	case Question::TEXT:
		return_string = std::string("TEXT");
		break;
	}
	return return_string;
}

std::string Question::get_question_file_string() {
	//string stream om gemakkelijk te werken
	std::stringstream file_stringstream;
	//standaard gedeelte invullen
	file_stringstream << id_ << " " << Question::get_type_string(type_) << " ";
	//specifiek gedeelte
	if (type_ == CHOICE) {
		file_stringstream << number_of_answers() << " ";
	}
	//vraag toevoegen
	file_stringstream << question_string_ << std::endl;

	//bij choice horen de antwoorden ook nog
	if (type_ == CHOICE) {
		for (int i = 0; i < number_of_answers(); i++) {
			file_stringstream << answers_[i] << std::endl;
		}
	}
	return file_stringstream.str();
}

