/*
 * parser.h
 *
 *  Created on: 1 mar 2016
 *      Author: lulewiczg
 */

#ifndef PARSER_H_
#define PARSER_H_

class Parser {
private:
	fstream stream;
	map<string, Syntax*> parsers;
	int count = 0;

	vector<string> split(string s) {
		istringstream iss(s);
		vector < string > tokens;
		copy(istream_iterator < string > (iss), istream_iterator<string>(), back_inserter(tokens));
		return tokens;
	}

	void parseLine(string str) {
		vector < string > splitLine = split(str);
		Syntax* syntax = parsers[toLower(splitLine[0])];
		if (!syntax) {
			syntax = parsers[toLower(splitLine[1])];
			if (!syntax) {
				stringstream ss;
				ss << "Nie mozna sparsowac linii " << str << "(linia " << count << ")";
				throw ss.str();
			}
		}
		Syntax::currLine = count;
		syntax->parseLine(splitLine);
	}

	void addSyntax(Syntax* s) {
		parsers[s->keyWord] = s;
	}

public:
	bool fileExists = false;

	Parser(string fileName) {
		stream.open(fileName.c_str());
		if (stream.good() && stream.is_open()) {
			fileExists = true;
			cout << "File " + fileName + " found!\n";
			//Tu dodawac parsery polecen
			addSyntax(new ProcedureSyntax());
			addSyntax(new AssingmentSyntax());
		}
	}

	void parse() {
		string line;
		do {
			getline(stream, line);
			count++;
			parseLine(line);
		} while (!stream.eof());
	}
}
;

#endif /* PARSER_H_ */
