/*
 * syntax.h
 *
 *  Created on: 1 mar 2016
 *      Author: lulewiczg
 */

#ifndef TYPES_SYNTAX_H_
#define TYPES_SYNTAX_H_

class Syntax {
protected:
	static string keyWord;
	static string all;
public:
	virtual bool parseLine(string str)=0;
	virtual ~Syntax() {
	}
};
string Syntax::all = "*";
string Syntax::keyWord;

class Procedure: public Syntax {
public:
	Procedure() {
		keyWord = "procedure";
	}

	bool parseLine(string str) {
		return str.find(keyWord) != std::string::npos;
	}
};

#endif /* TYPES_SYNTAX_H_ */
