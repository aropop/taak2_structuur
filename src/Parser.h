/*
 * Parser.h
 *
 *  Created on: 14-nov.-2013
 *      Author: arno
 */

#include <iostream>
#include "QuestionList.h"

#ifndef PARSER_H_
#define PARSER_H_

class Parser {
public:
	Parser(std::istream * in, std::ostream * out, QuestionList * ql);
	~Parser();
	void parse_next();

private:
	std::string message_;
	std::istream * in_;
	std::ostream * out_;
	QuestionList * ql_;

	enum ParserCode {
		EXIT, WRONG_MESSAGE, CORRECT
	};

	ParserCode parser_code_;
	int current_amount_of_answers_;

	std::string * prompt_for_choices();
	void parse_dispatch();
	bool prompt_save();
};

#endif /* PARSER_H_ */
