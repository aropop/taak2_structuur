/*
 * Question.cpp
 *
 *  Created on: 10-nov.-2013
 *      Author: arno
 */

#include "Question.h"
#include <iostream>

Question::Question(int id, QuestionType type, std::string& question,
		std::string * answers, int amount_of_answers) :
		copied(false), id_(id), type_(type), question_string_(question), answers_(
				answers), amount_of_answers_(amount_of_answers) {

}

Question::Question(int id, QuestionType type, std::string& question) :
		copied(false), id_(id), type_(type), question_string_(question), amount_of_answers_(
				0) {
	answers_ = NULL;

}

Question::Question(void) :
		copied(false), answers_(NULL) {

}

Question::~Question() {
	if (answers_ != NULL && !copied) {
		delete[] answers_;
		//is niet gekopieerd dus niet zijn verantwoordelijkheid
	}
}

void Question::set_question_string(std::string& new_question) {
	question_string_ = new_question;
}

void Question::set_answers(std::string * answers) {
	if (type_ == CHOICE) {
		delete[] answers_;
		answers_ = answers;
	} else {
		throw std::string("Cannot set answers for an not CHOICE question");
	}
}

int Question::number_of_answers() {
	return amount_of_answers_;
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


std::string Question::get_string(){
	//string stream om gemakkelijk te werken
	std::stringstream file_stringstream;
	//standaard gedeelte invullen
	file_stringstream << id_ << " " << Question::get_type_string(type_) << " ";
	//specifiek gedeelte
	if (type_ == Question::CHOICE) {
		file_stringstream << number_of_answers() << " ";
	}
	//vraag toevoegen
	file_stringstream << question_string_;
	return file_stringstream.str();

}
std::ostream& operator<<(std::ostream& out, Question& q){
	out << q.get_string();
	return out;
}



