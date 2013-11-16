/*
 * Parser.cpp
 *
 *  Created on: 14-nov.-2013
 *      Author: arno
 */
#include <iostream> //input output
#include <algorithm> //to lower case
#include <string> //::tolower
#include "Parser.h"
#include "QuestionList.h" //QuestionList
#include "Question.h" //QuestionTypes
Parser::Parser(std::istream * in, std::ostream * out, QuestionList * ql) :
		in_(in), out_(out), ql_(ql) {
	parse_next();
}

Parser::~Parser() {

}

void Parser::parse_next() {
	while (parser_code_ != EXIT) {
		if (parser_code_ == WRONG_MESSAGE) {
			*out_ << "Onbekend commando '" << message_ << "'" << std::endl;
		}
		getline(*in_, message_);
		parse_dispatch();
	}
}

void Parser::parse_dispatch() {
	std::stringstream ss;
	std::string command;

	ss.str(message_);
	ss >> command;

	std::transform(command.begin(), command.end(), command.begin(), ::tolower);

	if (command.compare("list") == 0) {
		ql_->list(out_);
	} else if (command.compare("add") == 0) {
		std::string question_type, question;
		int index(0);
		ss >> question_type;
		ss.ignore();
		getline(ss, question);
		if (question_type.compare(
				Question::get_type_string(Question::CHOICE)) == 0) {
			std::string * answers = prompt_for_choices();
			index = ql_->add(Question::CHOICE, question, answers,
					current_amount_of_answers_);
		} else if (question_type.compare(
				Question::get_type_string(Question::TEXT)) == 0) {
			index = ql_->add(Question::TEXT, question, NULL, 0);
		} else {
			*out_ << "Niet gekend vraag type: " << question_type << std::endl;
		}

		*out_ << "Vraag (" << question << ") toegevoegd op plaats " << index
				<< "." << std::endl;
	} else if (command.compare("insert") == 0) {
		std::string question_type, question;
		int position;
		ss >> position;
		ss >> question_type;
		ss.ignore();
		getline(ss, question);

		if (question_type.compare(
				Question::get_type_string(Question::CHOICE))) {
			std::string * answers = prompt_for_choices();
			ql_->add(Question::CHOICE, question, answers,
					current_amount_of_answers_, position);
		} else if (question_type.compare(
				Question::get_type_string(Question::TEXT))) {
			ql_->add(Question::TEXT, question, NULL, 0, position);
		} else {
			*out_ << "Niet gekend vraag type" << question_type << std::endl;
		}

		*out_ << "Vraag (" << question << ") toegevoegd op plaats " << position
				<< "." << std::endl;
	} else if (command.compare("exit") == 0) {
		if (ql_->dirty) {
			if (prompt_save()) {
				ql_->save();
				*out_ << "Bestand bewaard." << std::endl;
			}
		}
		parser_code_ = EXIT;
	} else if (command.compare("save") == 0) {
		if(ql_->dirty){
			ql_->save();
		}
	} else {
		parser_code_ = WRONG_MESSAGE;
	}
}

std::string* Parser::prompt_for_choices() {
	//todo fix error when more than 5 answers
	int array_size(5), amount_of_answers(0);
	std::string * answers = new std::string[array_size];
	std::string current_read;
	getline(*in_, current_read);
	while (!(current_read.compare(".") == 0)) {
		amount_of_answers++;
		if (amount_of_answers == array_size) {
			array_size = array_size * 2;
			std::string * temp = answers;
			answers = new std::string[array_size];
			for (int shift = 0; shift < array_size; ++shift) {
				answers[shift] = temp[shift];
			}
			delete[] temp;
		}
		answers[amount_of_answers] = current_read;
		getline(*in_, current_read);
	}
	current_amount_of_answers_ = amount_of_answers;
	if (amount_of_answers == 0) {
		*out_ << "Niet genoeg geldige antwoorden." << std::endl;
		answers = prompt_for_choices();
	}
	return answers;
}

bool Parser::prompt_save(){
	*out_ << "Er zijn onbewaarde wijzigingen. Moeten deze opgeslagen worden? (j/n)" << std::endl;
	std::string userInput;
	getline(*in_, userInput);
	while((userInput.compare("j") != 0) && (userInput.compare("n") != 0)){
		*out_ << "Geef een juist antwoord (j/n)" << std::endl;
		getline(*in_, userInput);
	}
	return userInput.compare("j") == 0;
}

