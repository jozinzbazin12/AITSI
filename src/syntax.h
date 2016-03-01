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
	static list<Syntax> parsers;
	static void init() {
//		parsers.push_front(Procedure());
	}
public:
	bool parseLine(string str) {
		return false;
	}

	static bool parse(string str) {
		for (list<Syntax>::iterator it = parsers.begin(); it != parsers.end(); ++it) {
			bool result = it->parseLine(str);
			if (result) {
				break;
			}
		}
		return false;
	}

	Syntax() {
		if (parsers.size() == 0) {
			init();
		}
	}

};
string Syntax::all = "*";

//class Procedure: public Syntax {
//
//public:
//	Procedure() {
//		keyWord = "procedure";
//	}
//
//	bool parseLine(string str) {
//		return false;
//	}
//};

#endif /* TYPES_SYNTAX_H_ */
