/*
 * matchers.h
 *
 *  Created on: 22 mar 2016
 *      Author: lulewiczg
 */

#ifndef PARSER_MATCHERS_H_
#define PARSER_MATCHERS_H_

class Matcher {
private:
	int getEnd(int pos, string str) {
		int len = str.length() - pos;
		if (!pos) {
			len--;
		}
		return len;
	}

	void throwException(string msg) {
		Exception e = Exception();
		e.line = -1;
		e.parser = "?";
		e.msg = msg;
		throw e;
	}
protected:
	Matcher() {
	}
public:
	static string space;
//	static string nothing;
	static string any;
	static string anyWord;
	string word;
	string rSeparator = "";
	string lSeparator = "";

	int matchWildcard(string s, int currentPos, Matcher* next) {
		if (word == any || (word == anyWord && !next)) {
			return s.length() - 1;
		}
		int pos = s.substr(currentPos, getEnd(currentPos, s)).find(next->word.c_str());
		if (pos == std::string::npos) {
			throwException("word not found: " + word);
		}
		pos--;
		if (lSeparator == space && s[currentPos] != ' ') {
			throwException("invalid left word separator: " + word);
		}
		if (rSeparator == space && s[pos + currentPos] != ' ') {
			throwException("invalid right word separator: " + word);
		}
		return pos + currentPos;

	}

	int match(string s, int currentPos, Matcher* next) {
		if (word == any || word == anyWord) {
			return matchWildcard(s, currentPos, next);
		}
		s = toLower(s);
		int pos = s.substr(currentPos, getEnd(currentPos, s)).find(word.c_str());
		if (pos == std::string::npos) {
			throwException("word not found: " + word);
		}
		if (lSeparator == space && s[pos - 1] != ' ') {
			throwException("invalid left word separator: " + word);
		}
		if (rSeparator == space && s[pos + word.length()] != ' ') {
			throwException("invalid right word separator: " + word);
		}
		return currentPos + pos + word.length();
	}

	Matcher(string word, string lSeparator = anyWord, string rSeparator = anyWord) {
		this->word = word;
		this->lSeparator = lSeparator;
		this->rSeparator = rSeparator;
		if (word == any && rSeparator == space) {
			cout << "Matcher " << lSeparator << word << rSeparator << " has no sense" << endl;
		}
	}
};
string Matcher::space = " ";
//string Matcher::nothing = "";
string Matcher::any = "**";
string Matcher::anyWord = "*";

class MultiMatcher: public Matcher {
public:
	string words[];

	MultiMatcher(string words[], string lSeparator = anyWord, string rSeparator = anyWord) {
		this->lSeparator = lSeparator;
		this->rSeparator = rSeparator;
		if (word == any && rSeparator == space) {
			cout << "Matcher " << lSeparator << word << rSeparator << " has no sense" << endl;
		}
	}

};

#endif /* PARSER_MATCHERS_H_ */
