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
	dirty = false;
	if (!input_file) {
		//bestand bestaat niet
		uuid_generate(uuid_);
	} else {
		//probeer te lezen van bestand
		try {
			read_from_file(&input_file);
		} catch (std::string& e) {
			//fout in bestand
			std::cout << e << std::endl;
		}
		input_file.close();
	}

}

QuestionList::~QuestionList(void){
	//verwijder de questions
	delete [] questions_;
}

void QuestionList::list(std::ostream * out) {
	for (int i = 0; i < amount_of_questions_; ++i) {
		*out << questions_[i].get_question_file_string().c_str();
	}
}

int QuestionList::add(Question::QuestionType type, std::string& question_string,
		 std::string *answers, int amount_of_answers,  int position) {
	Question* old_questions(questions_);
	int index(0);
	bool skipped(false);
	questions_ = new Question[amount_of_questions_ + 1];
	while(index < (amount_of_questions_ + 1)){
		if(index == (position - 1)){
			questions_[index] = Question(index + 1, type, question_string, answers, amount_of_answers);
		}else{
			if(skipped){
				questions_[index + 1] = old_questions[index];
			}else {
				questions_[index] = old_questions[index];
			}
			//we moet de antwoorden niet verwijderen want dat zou een probleem zijn
			old_questions[index].copied = true;
		}
		index++;
	}
	amount_of_questions_++;
	dirty = true;
	delete [] old_questions;
	return position;
}

int QuestionList::add(Question::QuestionType type, std::string& question_string, std::string *answers, int amount_of_answers) {
	return add(type, question_string, answers, amount_of_answers, amount_of_questions_ + 1);
}

void QuestionList::save() {

}

void QuestionList::read_from_file(std::ifstream * input_file) {
	std::stringstream ss;
	std::string current_line, current_identifier;
	getline(*input_file, current_line);
	//bekijk de versie
	if (current_line.compare("VERSION 1") == 0) {
		getline(*input_file, current_line);
		ss.clear();
		ss.str(current_line);
		ss >> current_identifier;
		//bekijk het id
		if (current_identifier.compare("ID") == 0) {
			//uuid van tekst naar uuid_t omzetten
			char uuid_txt[36];
			ss >> uuid_txt;
			uuid_parse(uuid_txt, uuid_);

			//size van de lijst uitlezen

			getline(*input_file, current_line);
			ss.clear();
			ss.str(current_line);
			ss >> current_identifier;
			ss >> amount_of_questions_;
			//nieuwe questions array aanmaken op de heap
			questions_ = new Question[amount_of_questions_];
			//deze dan verder invullen
			while (!(*input_file).eof()) {

				getline(*input_file, current_line);
				ss.clear();
				ss.str(current_line);

				int question_number;
				std::string question_string, question_type;

				ss >> question_number;
				ss >> question_type;

				//verschil tussen choice en text questions
				if (question_type.compare(
						Question::get_type_string(Question::CHOICE)) == 0) {

					int amount_of_answers;
					ss >> amount_of_answers;
					ss.ignore();
					getline(ss, question_string);
					std::string* answers = new std::string[amount_of_answers];
					ss.clear();

					//lees de choices uit
					for (int i = 0; i < amount_of_answers; ++i) {
						getline(*input_file, answers[i]);
					}
					//maak de vraag aan
					Question current_question (question_number,
							Question::CHOICE, question_string, answers, amount_of_answers);
					questions_[question_number - 1] = current_question;
				} else if (question_type.compare(
						Question::get_type_string(Question::TEXT)) == 0) {
					ss.ignore();
					getline(ss, question_string);
					//moet alleen vraag uitlezen en question object aanmake
					Question current_question (question_number,
							Question::TEXT, question_string);

					questions_[question_number - 1] = current_question;

					ss.clear();

				} else {
					//kent het question type niet
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

