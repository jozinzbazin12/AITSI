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
	bool brace = false;
	bool multiLine = false;
	bool semicolon = true;
	static string anyWord;
	static string any;
	vector<Matcher> syntax;

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

	bool isNumber(string name) {
		for (unsigned i = 1; i < name.size(); i++) {
			if (!isNumber(name[i])) {
				return false;
			}
		}
		return true;
	}

	bool getBrace() {
		bool b = brace;
		brace = false;
		return b;
	}

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
					throwException("denied characters after semicolon");
				}
				brace = true;
				str.erase(bracePos, 1);
			} else if (semicolonPos == string::npos) {
				throwException("no semicolon found");
			} else if (semicolonPos != str.length() - 1) {
				throwException("denied characters after semicolon");
			}
			str.erase(semicolonPos, 1);
		}
		for (vector<Matcher>::iterator it = syntax.begin(); it != syntax.end(); ++it) {
			Matcher* m = &*(it + 1);
			newPosition = it->match(str, position, m);
			if (!it->strict && (it->word == any || it->word == anyWord)) {
				string result = str.substr(position, newPosition - position + 1);
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
	vector<Syntax*> possibleChildren;
	static map<string, Syntax*> allSynstax;
	static int currLine;
	string keyWord;
	virtual ASTTree* parseLine(string str)=0;
	virtual ~Syntax() {

	}
};
string Syntax::anyWord = "*";
string Syntax::any = "**";
int Syntax::currLine = 0;
map<string, Syntax*> Syntax::allSynstax;

class RecursiveSyntax: public Syntax {
protected:

	string toString(vector<string> str) {
		stringstream ss;
		for (size_t i = 0; i < str.size(); ++i) {
			ss << str[i] << " ";
		}
		return ss.str();
	}

	ASTTree* parse(vector<string> line) {
		string str = toString(line);
		trim(str);
		if (!str.length()) {
			return NULL;
		}
		return parse(str);
	}

	ASTTree* parse(string str) {
		ASTTree* node = NULL;
		for (vector<Syntax*>::iterator it = parsers.begin(); it != parsers.end(); ++it) {
			try {
				node = (*it)->parseLine(str);
				if (node) {
					break;
				}
			} catch (Exception &e) {
				if (debug) {
					e.print();
				}
			}
		}
		if (!node) {
			RuntimeException e = RuntimeException();
			e.line = currLine;
			e.parser = keyWord;
			e.msg = "could not parse string " + str;
			throw e;
		}
		return node;
	}

public:
	vector<Syntax*> parsers;

};

class ProcedureSyntax: public Syntax {
private:
	static map<string, tree_node_<ASTNode*>*> procedures;

public:
	ProcedureSyntax() {
		keyWord = StatementType::PROCEDURE;
		syntax = {Matcher(StatementType::PROCEDURE,Matcher::anyWord, Matcher::space), Matcher(Matcher::anyWord,Matcher::space, Matcher::anyWord), Matcher("{")};
		semicolon = false;
	}

	ASTTree* parseLine(string str) {
		vector < string > splitStr = split(str);
		if (toLower(splitStr[0]) != keyWord) {
			return NULL;
		}
		vector < string > args = match(str);
		if (procedures[args[0]]) {
			throwException("procedure " + args[0] + " already exists!", true);
		}
		ASTTree* node = NodeUtil::createProcedureNode(args[0], currLine);
		procedures[args[0]] = *(node->getRoot());
		return node;
	}
};

map<string, tree_node_<ASTNode*>*> ProcedureSyntax::procedures;

class OperandSyntax: public Syntax {
public:
	OperandSyntax() {
		keyWord = "op";
		semicolon = false;
	}

	ASTTree* parseLine(string str) {
		if (isVar(str) || isNumber(str)) {
			return NodeUtil::createOperandNode(str, currLine);
		}
		return NULL;
	}
};

class MathSyntax: public RecursiveSyntax {

public:
	MathSyntax(string expr, bool strict = false) {
		semicolon = false;
		keyWord = expr;
		syntax = {Matcher(Matcher::anyWord), Matcher(keyWord, Matcher::anyWord, Matcher::anyWord, strict), Matcher(Matcher::any)};
		multiLine = true;
		parsers.push_back(allSynstax["op"]);
	}

	ASTTree* parseLine(string str) {
		if (str.find(keyWord) == string::npos) {
			return NULL;
		}
		vector < string > args = match(str);
		ASTTree* n = NodeUtil::createExpressionNode(keyWord, currLine);
		ASTTree* lResult = parse(args[0]);
		if (lResult) {
			NodeUtil::appendChild(n, lResult);
		}

		ASTTree* rResult = parse(args[1]);
		if (lResult) {
			NodeUtil::appendChild(n, rResult);
		}
		if (!lResult || !rResult) {
			throwException("no argument for operand " + keyWord, true);
		}
		return n;
	}
}
;

class AssingmentSyntax: public RecursiveSyntax {
public:
	AssingmentSyntax() {
		keyWord = StatementType::ASSIGN;
		syntax = {Matcher(Matcher::anyWord), Matcher(keyWord), Matcher(any)};
		multiLine = true;
		parsers.push_back(allSynstax["op"]);
	}

	ASTTree* parseLine(string str) {
		if (str.find(keyWord) == string::npos) {
			return NULL;
		}
		vector < string > args = match(str);
		string name = args.front();
		if (!isVar(name)) {
			throwException(name + " is not variable name!", true);
		}
		args.erase(args.begin());
		ASTTree* n = NodeUtil::createAssignmentNode(currLine);
		(*(n->getRoot()))->data->last = getBrace();
		ASTTree* result = parse(args);
		if (result) {
			NodeUtil::appendChild(n, result);
		}
		return n;
	}
};

#endif /* TYPES_SYNTAX_H_ */
