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
//variabelen initialiseren en parse loop starten
Parser::Parser(std::istream * in, std::ostream * out, QuestionList * ql) :
		in_(in), out_(out), ql_(ql) {
	parse_next();
}

Parser::~Parser() {

}

//parsed telkens voort
void Parser::parse_next() {
	while (parser_code_ != EXIT) { //stoppen bij een exit message
		if (parser_code_ == WRONG_MESSAGE) {
			*out_ << "Onbekend commando '" << message_ << "'" << std::endl;
		}
		//resetten zodat enkel bij een "fout" er een set van de parser message moet gebeuren
		reset_parser_code();
		//inlezen en dispathen
		getline(*in_, message_);
		parse_dispatch();
	}
}

//dispatched de commando's
void Parser::parse_dispatch() {
	std::stringstream ss;
	std::string command;

	ss.str(message_);
	ss >> command;

	//toelaten dat List en list hetzelfde zijn
	std::transform(command.begin(), command.end(), command.begin(), ::tolower);

	if (command.compare("list") == 0) {
		//list commando
		ql_->list(out_);
	} else if (command.compare("add") == 0) {
		//add commando
		std::string question_type, question;
		int index(0);
		ss >> question_type;
		ss.ignore();
		getline(ss, question);
		//add is verschillend voor choice als voor tekst
		if (question_type.compare(Question::get_type_string(Question::CHOICE))
				== 0) {
			//eerst nieuwe antwoorden prompten
			std::string * answers = prompt_for_choices();
			//nieuwe toevoegen geeft de index terug
			index = ql_->add(Question::CHOICE, question, answers,
					current_amount_of_answers_);
			print_add_text(question, index);
		} else if (question_type.compare(
				Question::get_type_string(Question::TEXT)) == 0) {
			//nieuwe toevoegen en commando afronden
			index = ql_->add(Question::TEXT, question, NULL, 0);
			print_add_text(question, index);
		} else {
			//ongekend vraag type
			*out_ << "Niet gekend vraag type: " << question_type << std::endl;
		}

	} else if (command.compare("insert") == 0) {
		//insert commando
		std::string question_type, question;
		int position;
		ss >> position;
		//kijken of positie in range is
		if (ql_->in_range(position)) {
			ss >> question_type;
			ss.ignore();
			getline(ss, question);
			//bij choice moeten er nog antwoorden worden gevraagd
			if (question_type.compare(
					Question::get_type_string(Question::CHOICE)) == 0) {
				//choice antwoorden vragen en question toevoegen
				std::string * answers = prompt_for_choices();
				ql_->add(Question::CHOICE, question, answers,
						current_amount_of_answers_, position);
				print_add_text(question, position);
			} else if (question_type.compare(
					Question::get_type_string(Question::TEXT)) == 0) {
				//TEXT question bouwen en toevoegen
				ql_->add(Question::TEXT, question, NULL, 0, position);
				print_add_text(question, position);
			} else {
				//onbekend type
				*out_ << "Niet gekend vraag type" << question_type << std::endl;
			}
		} else {
			//out of biound error printen
			print_out_of_bounds(position);
		}
	} else if (command.compare("edit") == 0) {
		//edit heeft 2 vormen
		ss.ignore();
		if ((ss.peek() >= '0') && (ss.peek() <= '9')) {
			//edit de vraagtekst
			int index;
			ss >> index;
			//kijken binnen range
			if (ql_->in_range(index)) {
				std::string new_question_string =
						prompt_for_new_question_string(index - 1);
				ql_->edit(index - 1, new_question_string);
				*out_ << "Vraagtekst voor vraag " << index << " aangepast."
						<< std::endl;
			} else {
				//out of bounds error
				print_out_of_bounds(index);
			}
		} else {
			//edit de choices
			std::string ignore;
			int index;
			ss >> ignore;
			ss >> index;
			if (ql_->in_range(index)) {
				//answers vragen en choices aanpassen
				*out_ << "Nieuwe antwoorden voor vraag 2 ("
						<< ql_->get_question_string(index - 1) << ")"
						<< std::endl;
				std::string * answers = prompt_for_choices();
				//edit zou een error kunnen throwen wanneer dit een choice is
				try {
					ql_->edit_choice(index - 1, answers,
							current_amount_of_answers_); //-1 is het verschil tussen vraag nummer en index nummer
					*out_ << "Antwoorden voor vraag " << index << " aangepast."
							<< std::endl;
				} catch (std::string& e) {
					//de error gewoon printen
					*out_ << e << std::endl;
				}
			} else {
				//out of bounds error printen
				print_out_of_bounds(index);
			}
		}

	} else if (command.compare("remove") == 0) {
		//remove commando
		int index;
		ss >> index;
		//out of bounds
		if(ql_->in_range(index)){
			//text vragen voor verwijderen
			std::string text(ql_->get_question_string(index - 1));
			ql_->delete_question(index - 1);
			*out_ << "Vraag " << index << " (" << text << ") verwijderd." << std::endl;
		}else{
			//error weergeven
			print_out_of_bounds(index);
		}

	} else if (command.compare("exit") == 0) {
		//exit commando
		//alleen vragen voor een save als er aanpassing zijn
		if (ql_->dirty) {
			if (prompt_save()) { //geeft bool terug
				ql_->save();
				*out_ << "Bestand bewaard." << std::endl;
			}
		}
		parser_code_ = EXIT; //parser_code op exit zetten om te stoppen
	} else if (command.compare("save") == 0) {
		//save commando
		if (ql_->dirty) {
			ql_->save();
		}
	} else {
		//verkeerde message
		parser_code_ = WRONG_MESSAGE;
	}
}

//hulp functie die een pointer naar een stuk geheugen op de heap
//hierin zit een array met de hoeveelheid antwoorden
//current_amount_of_answers_ bevat dan het aantal antwoorden dat de gebruiker opgeeft
std::string* Parser::prompt_for_choices() {
	//variabelen initialiseren
	int array_size(5), amount_of_answers(0);
	std::string * answers = new std::string[array_size];
	std::string current_read;
	getline(*in_, current_read);
	//lopen tot de punt
	while (!(current_read.compare(".") == 0)) {
		amount_of_answers++;
		if (amount_of_answers > array_size) {
			//het stuk gealloceerde geheugen is niet groot genoeg
			//groter stuk aanvragen en opvullen
			array_size = array_size * 2;
			std::string * temp = answers;
			answers = new std::string[array_size];
			for (int shift = 0; shift < (array_size / 2); ++shift) {
				answers[shift] = temp[shift];
			}
			//oud stuk deleten
			delete[] temp;
		}
		//opvullen
		answers[amount_of_answers - 1] = current_read;
		getline(*in_, current_read);
	}
	current_amount_of_answers_ = amount_of_answers;
	//test of er genoeg antwoorden zijn
	if (amount_of_answers == 0) {
		*out_ << "Niet genoeg geldige antwoorden." << std::endl;
		answers = prompt_for_choices();
	}
	return answers;
}

//hulp functie die vraagt of een dirty lijst moet worden gesaved
bool Parser::prompt_save() {
	*out_
			<< "Er zijn onbewaarde wijzigingen. Moeten deze opgeslagen worden? (j/n)"
			<< std::endl;
	std::string userInput;
	getline(*in_, userInput);
	while ((userInput.compare("j") != 0) && (userInput.compare("n") != 0)) {
		*out_ << "Geef een juist antwoord (j/n)" << std::endl;
		getline(*in_, userInput);
	}
	return userInput.compare("j") == 0;
}

//hulp functie die de parser code reset
inline void Parser::reset_parser_code() {
	parser_code_ = CORRECT;
}

//hulp functie die de nieuwe vraagtekst terug geeft
std::string Parser::prompt_for_new_question_string(int index) {
	*out_ << "Nieuwe vraagtekst voor vraag " << index << " ("
			<< ql_->get_question_string(index) << ")" << std::endl;
	std::string new_question_string;
	getline(*in_, new_question_string);
	while (new_question_string.empty()) {
		*out_ << "Geef een niet lege vraag in!" << std::endl;
		getline(*in_, new_question_string);
	}
	return new_question_string;
}

//inline print functies om code reproductie te vermijden
const inline void Parser::print_add_text(std::string& question, int position) {
	*out_ << "Vraag (" << question << ") toegevoegd op plaats " << position
			<< "." << std::endl;
}

inline const void Parser::print_out_of_bounds(int index) {
	*out_ << "Ongeldige invoer ("  << index << "), N=" << ql_->getAmountOfQuestions() << std::endl;
}

