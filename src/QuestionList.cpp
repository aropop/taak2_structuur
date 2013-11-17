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
	//ifstream opbouwen
	std::ifstream input_file(filename_.c_str());
	dirty = false;
	//verschil tussen een bestaand bestand en een niet bestaand bestand
	if (!input_file) {
		//bestand bestaat niet
		uuid_generate(uuid_);
		amount_of_questions_ = 0;
		questions_ = NULL;
	} else {
		//probeer te lezen van bestand
		try {
			read_from_file(&input_file);
		} catch (std::string& e) {
			//fout in bestand
			std::cout << e << std::endl;
		}
		//file sluiten
		input_file.close();
	}

}

QuestionList::~QuestionList(void) {
	//verwijder de questions
	delete[] questions_;
}

//list commando
void QuestionList::list(std::ostream * out) {
	//over vragen lopen en ze uitprinten
	for (int i = 0; i < amount_of_questions_; ++i) {
		*out << questions_[i] << std::endl;
	}
}

//add commando met een positie
int QuestionList::add(Question::QuestionType type, std::string& question_string,
		std::string *answers, int amount_of_answers, int position) {
	//pointer bij houden naaar het oudestuk geheugen
	Question* old_questions(questions_);
	//hulp variabelen
	int index(0);
	bool skipped(false);
	//nieuw stuk geheugen alloceren
	questions_ = new Question[amount_of_questions_ + 1];
	//over de vragen lopen
	while (index < (amount_of_questions_ + 1)) {
		//vraag moet ingevoegd worden
		if (index == (position - 1)) {
			Question q = Question(index + 1, type, question_string,
					answers, amount_of_answers);
			questions_[index] = q;
			//ervoor zorgen dat de destructor de antwoorden niet dealloceert
			q.copied = true;
			//we hebben een index geskipped in de oude array
			skipped = true;
		} else {
			if (skipped) {
				questions_[index] = old_questions[index - 1]; //index loopt gewoon door dus moet je 1 achteruit in de oude kijken
				questions_[index].increase_id(); //vragen moeten nu een nummer hoger hebben
				old_questions[index - 1].copied = true;
			} else {
				questions_[index] = old_questions[index];
			//we moet de antwoorden niet verwijderen want dat zou een probleem zijn
				old_questions[index].copied = true;
			}
		}
		index++;
	}
	amount_of_questions_++;
	dirty = true;
	//oude vragen dealloceren
	delete[] old_questions;
	//positie terug geven voor het antwoord op het commando
	return position;
}

//overloaden om add zonder positie mogelijk te maken
int QuestionList::add(Question::QuestionType type, std::string& question_string,
		std::string *answers, int amount_of_answers) {
	return add(type, question_string, answers, amount_of_answers,
			amount_of_questions_ + 1);
}

//hulp functie die uuid omzet
std::string uuid_to_string(char * character_array){
	std::string uuid_string;
	uuid_string.append(character_array, 36);
	return uuid_string;
}

//save commando
void QuestionList::save() {
	//output file openen
	std::ofstream output_file(filename_.c_str());
	char uuid_str[36];
	uuid_unparse(uuid_, uuid_str);
	//header wegschrijven
	output_file << "VERSION 1" << std::endl << "ID " << uuid_to_string(uuid_str)
			<< std::endl << "STEPS " << amount_of_questions_ << std::endl;
	//vragen wegschrijven
	for (int i = 0; i < amount_of_questions_; ++i) {
		output_file << questions_[i].get_question_file_string();
	}
	//sluiten
	output_file.close();
	//ql is niet meer dirty want is juist weggeschreven
	dirty = false;
}

//hulp functie die zegt of een bepaalde positie binnen het bereik ligt
bool QuestionList::in_range(int position) {
	return ((0 < position) && (position < (amount_of_questions_ + 1)));
}

//geeft de question string door
std::string QuestionList::get_question_string(int index) {
	return questions_[index].question_string;
}

//edit commando voor question strings
void QuestionList::edit(int question_number, std::string& new_question_string) {
	questions_[question_number].set_question_string(new_question_string);
	dirty = true;
}

//edit commando voor choices
void QuestionList::edit_choice(int question_number, std::string* new_answers,
		int amount) {
	questions_[question_number].set_answers(new_answers, amount);
	dirty = true;
}

//getter voor amount of questions
int QuestionList::getAmountOfQuestions() const {
	return amount_of_questions_;
}

//delete commando
void QuestionList::delete_question(int question_number) {
	Question * new_question_mem = new Question[amount_of_questions_ - 1];
	bool passed(false);
	for (int i = 0; i < amount_of_questions_; ++i) {
		if (i == question_number) {
			//skips copying this question
			passed = true;
		} else {
			if (passed) {
				//has skipped the deleted one
				new_question_mem[i - 1] = questions_[i];
				//copied so don't delete answers array
				questions_[i].copied = true;
				//decrease id
				new_question_mem[i - 1].decrease_id();
			} else {
				//hasn't skipped the deleted one
				new_question_mem[i] = questions_[i];
				questions_[i].copied = true;
			}
		}
	}
	//delete old questions
	delete[] questions_;
	//set new questions
	questions_ = new_question_mem;
	//het aantal questions daalt met 1
	amount_of_questions_--;
}

//functie die het bestand uitleest en het object invult
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
			int counter(1);
			//deze dan verder invullen
			while (amount_of_questions_ >= counter) {

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
					Question current_question(question_number, Question::CHOICE,
							question_string, answers, amount_of_answers);
					questions_[question_number - 1] = current_question;
					//scope zal de current_question destructen maar we hebben ze gekopieerd naar het geheugen
					current_question.copied = true;
				} else if (question_type.compare(
						Question::get_type_string(Question::TEXT)) == 0) {
					ss.ignore();
					getline(ss, question_string);
					//moet alleen vraag uitlezen en question object aanmaken
					Question current_question(question_number, Question::TEXT,
							question_string);

					questions_[question_number - 1] = current_question;

					ss.clear();

				} else {
					//kent het question type niet
					throw std::string("Unknown question type");
				}
				counter++;

			}

		} else {
			throw std::string("Corrupt file");
		}
	} else {
		throw std::string("Corrupt file");
	}

}

