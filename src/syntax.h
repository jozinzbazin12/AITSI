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
	bool multiLine = false;
	static string anyWord;
	static string any;
	vector<string> syntax;
	vector<Syntax*> possibleChildren;

	string getError() {
		stringstream ss;
		ss << "Wystapil blad w linii " << currLine << ", " << keyWord << ", ";
		return ss.str();
	}

	vector<string> match(vector<string> str) {
		int lineSize = str.size();
		int syntaxSize = syntax.size();
		if (!multiLine && lineSize != syntaxSize) {
			stringstream ss;
			ss << getError() << "niepoprawna ilosc slow (" << lineSize << " vs " << syntaxSize << ")";
			throw ss.str();
		}
		vector < string > vars;
		for (unsigned i = 0; i < str.size(); i++) {
			if (syntax[i] == any) {
				for (unsigned j = i; j < str.size(); j++) {
					vars.push_back(str[j]);
				}
				break;
			} else if (syntax[i] == anyWord) {
				vars.push_back(str[i]);
			} else if (toLower(str[i]) != toLower(syntax[i])) {
				string msg = getError() + "slowa nie pokrywaja sie (" + str[i] + " vs " + syntax[i] + ")";
			}
		}
		return vars;
	}
public:
	static int currLine;
	string keyWord;
	virtual void parseLine(vector<string> strs)=0;
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
		syntax = {"procedure", "*", "{"};
	}

	void parseLine(vector<string> strs) {
		vector<string> args=match(strs);
		if(procedures[args[0]]) {
			throw getError() +"procedura "+ args[0] +" juz istnieje!";
		}
		Procedure* p=new Procedure(args[0]);
		procedures[args[0]]=p;
	}
}
;
map<string, Procedure*> ProcedureSyntax::procedures;

class AssingmentSyntax: public Syntax {
protected:
	vector<string> varNames;

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
		syntax = {"*", "=", "**"};
		multiLine = true;
	}

	void parseLine(vector<string> strs) {
		string last = strs.back();
//		if (last.find(";") != last.length() - 1) {
//			throw getError() + "brak ; na koncu linii";
//		}
		vector < string > args = match(strs);
		string name = args.front();
		if (!isVar(name)) {
			throw getError() + name + " nie jest nazwa zmiennej!";
		}
		varNames.push_back(name);
		args.erase(args.begin());
	}
}
;


#endif /* TYPES_SYNTAX_H_ */
