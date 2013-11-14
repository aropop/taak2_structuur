/*
 * QuestionList.cpp
 *
 *  Created on: 10-nov.-2013
 *      Author: arno
 */

#include "QuestionList.h"
#include "Question.h"
#include <iostream>
#include <fstream>
#include <uuid/uuid.h>

QuestionList::QuestionList(std::string& filename) :
		filename_(filename) {
	std::ifstream input_file(filename_.c_str());
	dirty_ = false;
	if (!input_file) {
		//bestand bestaat niet
		uuid_generate(uuid_);
	} else {
		try {
			read_from_file(&input_file);
		} catch (std::string& e) {
			std::cout << e << std::endl;
		}
		input_file.close();
	}

}

QuestionList::~QuestionList(void){
	delete [] questions_;
}

void QuestionList::read_from_file(std::ifstream * input_file) {
	std::stringstream ss;
	std::string current_line, current_identifier;
	getline(*input_file, current_line);

	if (current_line.compare("VERSION 1") == 0) {
		getline(*input_file, current_line);
		ss.clear();
		ss.str(current_line);
		ss >> current_identifier;
		if (current_identifier.compare("ID") == 0) {
			//uuid van tekst naar uuid_t omzetten
			char uuid_txt[36];
			ss >> uuid_txt;
			uuid_parse(uuid_txt, uuid_);

			//size van de lijst uitlezen
			int amount_of_questions;
			getline(*input_file, current_line);
			ss.clear();
			ss.str(current_line);
			ss >> current_identifier;
			ss >> amount_of_questions;
			questions_ = new Question[amount_of_questions];

			while (!(*input_file).eof()) {

				getline(*input_file, current_line);
				ss.clear();
				ss.str(current_line);

				int question_number;
				std::string question_string, question_type;

				ss >> question_number;
				ss >> question_type;


				if (question_type.compare(
						Question::get_type_string(Question::CHOICE)) == 0) {

					int amount_of_answers;
					ss >> amount_of_answers;
					getline(ss, question_string);
					std::string* answers = new std::string[amount_of_answers];
					ss.clear();


					for (int i = 0; i < amount_of_answers; ++i) {
						getline(*input_file, answers[i]);
					}

					Question current_question (question_number,
							Question::CHOICE, question_string, answers);
					questions_[question_number - 1] = current_question;
				} else if (question_type.compare(
						Question::get_type_string(Question::TEXT)) == 0) {

					getline(ss, question_string);

					Question current_question (question_number,
							Question::TEXT, question_string);

					questions_[question_number - 1] = current_question;

					ss.clear();

				} else {
					throw std::string("Unknown question type");
				}

			}

		} else {
			throw std::string("Corrupt file");
		}
	} else {
		throw std::string("Corrupt file");
	}

}

