/*
 * syntax.h
 *
 *  Created on: 1 mar 2016
 *      Author: lulewiczg
 */

#ifndef TYPES_SYNTAX_H_
#define TYPES_SYNTAX_H_

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
			throwException("Nie znaleziono nastepnego slowa " + word);
		}
		pos--;
		if (lSeparator == space && s[currentPos - 1] != ' ') {
			throwException("Niepoprawny lewy separator slowa " + word);
		}
		if (rSeparator == space && s[pos + currentPos] != ' ') {
			throwException("Niepoprawny prawy separator slowa " + word);
		}
		return pos + currentPos + 1;

	}
	int match(string s, int currentPos, Matcher* next) {
		if (word == any || word == anyWord) {
			return matchWildcard(s, currentPos, next);
		}
		s = toLower(s);
		int pos = s.substr(currentPos, getEnd(currentPos, s)).find(word.c_str());
		if (pos == std::string::npos) {
			throwException("Nie znaleziono slowa " + word);
		}
		if (lSeparator == space && s[pos - 1] != ' ') {
			throwException("Niepoprawny lewy separator slowa " + word);
		}
		if (rSeparator == space && s[pos + s.length()] != ' ') {
			throwException("Niepoprawny prawy separator slowa " + word);
		}
		return pos + word.length() + 1;
	}

	Matcher(string word, string lSeparator, string rSeparator) {
		this->word = word;
		this->lSeparator = lSeparator;
		this->rSeparator = rSeparator;
		if (word == any && rSeparator == space) {
			cout << "Matcher " << word << " jest bez sensu" << endl;
		}
	}
};
string Matcher::space = " ";
//string Matcher::nothing = "";
string Matcher::any = "**";
string Matcher::anyWord = "*";

class Syntax {
protected:
	bool multiLine = false;
	bool semicolon = true;
	static string anyWord;
	static string any;
	vector<Matcher> syntax;
	vector<Syntax*> possibleChildren;

	void throwException(string msg, bool runtime = false) {
		Exception e;
		if (runtime) {
			e = RuntimeException();
		} else {
			e = Exception();
		}
		e.line = currLine;
		e.parser = keyWord;
		e.msg = msg;
		throw e;
	}

	vector<string> match(string str) {
		vector < string > vars;
		int position = 0;
		int newPosition = 0;
		if (semicolon) {
			int semicolonPos = str.find(";");
			int bracePos = str.find("}");
			if (bracePos != string::npos && bracePos > semicolonPos) {
				string end = str.substr(semicolonPos, bracePos);
				removeWhitespaces(end);
				if (end.length() > 2) {
					throwException("niedowzolone znaki po sredniku");
				}
			} else if (semicolonPos == string::npos) {
				throwException("brakuje srednika");
			} else if (semicolonPos != str.length() - 1) {
				throwException("niedowzolone znaki po sredniku");
			}

		}
		for (vector<Matcher>::iterator it = syntax.begin(); it != syntax.end(); ++it) {
			Matcher* m = &*(it + 1);
			newPosition = it->match(str, position, m);
			if (it->word == any || it->word == anyWord) {
				string result = str.substr(position, newPosition - position);
				trim(result);
				vars.push_back(result);
			}
			position = newPosition;
		}
		return vars;
	}

	vector<string> split(string s) {
		istringstream iss(s);
		vector < string > tokens;
		copy(istream_iterator < string > (iss), istream_iterator<string>(), back_inserter(tokens));
		return tokens;
	}

public:
	static int currLine;
	string keyWord;
	virtual Node* parseLine(string str)=0;
	virtual ~Syntax() {
	}
}
;
string Syntax::anyWord = "*";
string Syntax::any = "**";
int Syntax::currLine = 0;

class ProcedureSyntax: public Syntax {
private:
	static map<string, Procedure*> procedures;

public:
	ProcedureSyntax() {
		keyWord = StatementType::PROCEDURE;
		syntax = {Matcher(StatementType::PROCEDURE,Matcher::anyWord, Matcher::any),
			Matcher(Matcher::anyWord,Matcher::space, Matcher::anyWord), Matcher("{", Matcher::anyWord, Matcher::anyWord)};
		semicolon = false;
	}

	Node* parseLine(string str) {
		vector < string > splitStr = split(str);
		if (toLower(splitStr[0]) != keyWord) {
			return NULL;
		}
		vector < string > args = match(str);
		if (procedures[args[0]]) {
			throwException("procedura " + args[0] + " juz istnieje!", true);
		}
		Procedure* p = new Procedure(args[0]);
		procedures[args[0]] = p;
		return p;
	}
}
;
map<string, Procedure*> ProcedureSyntax::procedures;

class AssingmentSyntax: public Syntax {
protected:
	bool isLetter(char c) {
		return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
	}

	bool isNumber(char c) {
		int number = c - 48;
		return number >= 0 && number <= 9;
	}

	bool isVar(string name) {
		if (!isLetter(name[0])) {
			return false;
		}
		for (unsigned i = 1; i < name.size(); i++) {
			if (!isNumber(name[i]) && !isLetter(name[i])) {
				return false;
			}
		}
		return true;
	}
public:
	AssingmentSyntax() {
		keyWord = StatementType::ASSIGN;
		syntax = {Matcher(Matcher::anyWord, Matcher::anyWord, Matcher::anyWord),
			Matcher("=", Matcher::anyWord, Matcher::anyWord), Matcher(any, Matcher::anyWord, Matcher::anyWord)};
		multiLine = true;
	}

	Node* parseLine(string str) {
		if (str.find(keyWord) == string::npos) {
			return NULL;
		}
		vector < string > args = match(str);
		string name = args.front();
		if (!isVar(name)) {
			throwException(name + " nie jest nazwa zmiennej!", true);
		}
		args.erase(args.begin());
		///rekurencja tu ma byc
		return new Assignment();
	}
}
;

#endif /* TYPES_SYNTAX_H_ */
