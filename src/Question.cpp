/*
 * Question.cpp
 *
 *  Created on: 10-nov.-2013
 *      Author: arno
 */

#include "Question.h"
#include <iostream>

//Constructor meestal gebruikt voor CHOICE type
Question::Question(int id, QuestionType type, std::string& question,
		std::string * answers, int amount_of_answers) :
		question_string(question), copied(false), id_(id), type_(type), answers_(
				answers), amount_of_answers_(amount_of_answers) {

}
//Constructor meestal gebruikt voor TEXT type
Question::Question(int id, QuestionType type, std::string& question) :
		question_string(question), copied(false), id_(id), type_(type), answers_(
				NULL), amount_of_answers_(0) {

}
//voor initialisatie
Question::Question(void) :
		copied(false), answers_(NULL) {

}

Question::~Question() {
	if (answers_ != NULL && !copied) {
		delete[] answers_;
		//is niet gekopieerd dus niet zijn verantwoordelijkheid
	}
}

//verander de question string
void Question::set_question_string(std::string& new_question) {
	question_string = new_question;
}

//veranderd de pointer van de antwoorden
void Question::set_answers(std::string * answers, int amount) {
	if (type_ == CHOICE) {
		delete[] answers_;
		answers_ = answers;
		amount_of_answers_ = amount;
	} else {
		throw std::string("Kan de antwoorden van een CHOICE vraag niet aanpassen!");
	}
}

//hulp functies voor het aanpassen van het id
int Question::increase_id() {
	return ++id_;
}

int Question::decrease_id() {
	return --id_;
}

//returns the amount of answers for this question
const int Question::number_of_answers() {
	return amount_of_answers_;
}

//statische functie die de string voor het questiontype terug geeft
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
	file_stringstream << question_string << std::endl;

	//bij choice horen de antwoorden ook nog
	if (type_ == CHOICE) {
		for (int i = 0; i < number_of_answers(); i++) {
			file_stringstream << answers_[i] << std::endl;
		}
	}
	return file_stringstream.str();
}

std::string Question::get_string() {
	//string stream om gemakkelijk te werken
	std::stringstream file_stringstream;
	//standaard gedeelte invullen
	file_stringstream << id_ << " " << Question::get_type_string(type_) << " ";
	//specifiek gedeelte
	if (type_ == Question::CHOICE) {
		file_stringstream << number_of_answers() << " ";
	}
	//vraag toevoegen
	file_stringstream << question_string;
	return file_stringstream.str();

}
//zorgt voor een gemakkelijk output
std::ostream& operator<<(std::ostream& out, Question& q) {
	out << q.get_string();
	return out;
}

