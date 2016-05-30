/*
* Matcher.h
*
*  Created on: 31 mar 2016
*      Author: Popek
*/

#ifndef SRC_PARSERPQL_MATCHERPQL_H_
#define SRC_PARSERPQL_MATCHERPQL_H_

#include <vector>
#include <iostream>
#include "../PQL/Field.h"

using namespace std;

class MatcherPQL {
public:
	MatcherPQL();
	virtual ~MatcherPQL();
	vector<int> findPosition(vector<string> &elems, string token);
	bool checkTokens(string elem, string token) {
		//cout << elem.find(token) << endl;
		if (elem.find(token) > elem.length())
			return false;
		else
			return true;
	}

	bool checkVarName(string elem) {
		return (elem.find(".varName") < elem.length());
	}

	bool checkProcName(string elem) {
		return (elem.find(".procName") < elem.length());
	}

	bool checkStmt_(string elem) {
		return (elem.find(".stmt#") < elem.length());
	}

	bool checkValue(string elem) {
		return (elem.find(".value") < elem.length());
	}

	bool checkBOOLEAN(string elem) {
		return (elem.find("BOOLEAN") < elem.length());
	}

	bool checkAll(string elem) {
		if (!checkVarName(elem) && !checkProcName(elem) && !checkStmt_(elem) && !checkValue(elem) && !checkBOOLEAN(elem))
			return false;
		else
			return true;
	}

	bool checkWithAttributes(Field* field1, Field* field2)
	{
		string type1 = "error";
		if (field1->getType() == "procedure" && field1->isProcName()) type1 = "string";
		if (field1->getType() == "variable" && field1->isVarName()) type1 = "string";
		if (field1->getType() == "constant") type1 = "integer";
		if ((field1->getType() == "stmt"
			|| field1->getType() == "assign"
			|| field1->getType() == "while"
			|| field1->getType() == "if"
			|| field1->getType() == "call"
			|| field1->getType() == "prog_line") && field1->isStmt()) type1 = "integer";
		if (field1->getType() == "string") type1 = "string";

		string type2 = "error";
		if (field2->getType() == "procedure" && field2->isProcName()) type2 = "string";
		if (field2->getType() == "variable" && field2->isVarName()) type2 = "string";
		if (field2->getType() == "constant") type2 = "integer";
		if ((field2->getType() == "stmt"
			|| field2->getType() == "assign"
			|| field2->getType() == "while"
			|| field2->getType() == "if"
			|| field2->getType() == "call"
			|| field2->getType() == "prog_line") && field2->isStmt()) type2 = "integer";
		if (field2->getType() == "string") type2 = "string";

		if (type1 == "string" && type2 == "string")
			return true;
		else if (type1 == "integer" && type2 == "integer")
			return true;
		else
			return false;
	}

	vector<string> getTokensList()
	{
		return{ "assign","stmtlst","stmt","while","variable","constant","prog_line","if","call","procedure" };
		//plus, minus, times - brakuje
	}

	string checkSuchThatType(string suchThatPart) {
		if (checkTokens(suchThatPart, "Parent")) return "parent";
		if (checkTokens(suchThatPart, "Follows")) return "follows";
		if (checkTokens(suchThatPart, "Modifies")) return "modifies";
		if (checkTokens(suchThatPart, "Uses")) return "uses";
		if (checkTokens(suchThatPart, "Calls")) return "calls";
		if (checkTokens(suchThatPart, "Next")) return "next";
		if (checkTokens(suchThatPart, "Affects")) return "affects";

		return "";
	}

	bool isString(string elem)
	{
		int pos1 = elem.find("\"");
		int pos2 = elem.find("\"", pos1 + 1);
		int pos3 = elem.find("\"", pos2 + 1);

		if (pos1 < pos2 && pos1 + 1 != pos2 && pos3 > elem.length())
			return true;
		else
			return false;
	}

	bool isStar(string elem, int position)
	{
		if (elem.find("*", position) < elem.length() && elem.find("*", position) == position)
			return true;
		else
			return false;
	}

	bool hasTwoElem(string elem)
	{
		int pos1 = elem.find(",");
		int pos2 = elem.find(",", pos1 + 1);

		if (pos1 < elem.length() && pos2 > elem.length())
			return true;
		else
			return false;
	}

	bool withHasTwoElem(string elem)
	{
		int pos1 = elem.find("=");
		int pos2 = elem.find("=", pos1 + 1);

		if (pos1 < elem.length() && pos2 > elem.length())
			return true;
		else
			return false;
	}

	bool isBracket(string elem)
	{
		int pos1 = elem.find("(");
		int pos2 = elem.find(")");

		if (pos1 == 0 && pos2 == elem.length() - 1)
			return true;
		else
			return false;
	}

	int getPosition(string elem, string token)
	{
		return elem.find(token);
	}

	bool is_(string elem)
	{
		if (elem.find("_") < elem.length())
			return true;
		else
			return false;
	}

	bool isNumber(string elem)
	{
		for (int i = 0; i < elem.length(); i++)
		{
			if (!isdigit(elem[i]))
				return false;
		}
		return true;
	}
};
#endif /* SRC_PARSERPQL_MATCHERPQL_H_ */
