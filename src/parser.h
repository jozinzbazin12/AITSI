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
	vector<Syntax*> parsers;

	bool parseLine(string str) {
		for (vector<Syntax*>::iterator it = parsers.begin(); it != parsers.end(); ++it) {
			bool result = (*it)->parseLine(str);
			if (result) {
				return true;
			}
		}
		return false;
	}

public:
	bool fileExists = false;

	Parser(string fileName) {
		stream.open(fileName.c_str());
		if (stream.good() && stream.is_open()) {
			fileExists = true;
			cout << "File " + fileName + " found!\n";
			//Tu dodawac parsery polecen
			parsers.push_back(new Procedure());
		}
	}

	bool parse() {
		string line;
		bool result = false;
		do {
			getline(stream, line);
			result = parseLine(line);
			if (!result) {
				return false;
			}
		} while (!stream.eof());
		return true;
	}
};

#endif /* PARSER_H_ */
