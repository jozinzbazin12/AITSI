/*
 * parser.h
 *
 *  Created on: 1 mar 2016
 *      Author: lulewiczg
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <string>

class Parser {
private:
	fstream stream;

public:
	bool fileExists = false;
	Parser(string fileName) {
		stream.open(fileName.c_str());
		if (stream.good() && stream.is_open()) {
			fileExists = true;
		}
	}

	void parse() {
		string line;
		do {
			getline(stream, line);
		} while (!stream.eof());
	}
}
;

#endif /* PARSER_H_ */
