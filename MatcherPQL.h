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
		return (elem.find(".varname") < elem.length());
	}

	bool checkProcName(string elem) {
		return (elem.find(".procname") < elem.length());
	}

	bool checkStmt_(string elem) {
		return (elem.find(".stmt#") < elem.length());
	}

	bool checkValue(string elem) {
		return (elem.find(".value") < elem.length());
	}

	bool checkAll(string elem) {
		if (!checkVarName(elem) && !checkProcName(elem) && !checkStmt_(elem) && !checkValue(elem))
			return false;
		else
			return true;
	}

	vector<string> getTokensList()
	{
		return {"assign","stmtlst","stmt","while","variable","constant","prog_line","if","call","procedure"};
		//plus, minus, times - brakuje
	}

	string checkSuchThatType(string suchThatPart) {
		if(checkTokens(suchThatPart,"parent")) return "parent";
		if(checkTokens(suchThatPart,"follows")) return "follows";
		if(checkTokens(suchThatPart,"modifies")) return "modifies";
		if(checkTokens(suchThatPart,"uses")) return "uses";
		if(checkTokens(suchThatPart,"calls")) return "calls";
		if(checkTokens(suchThatPart,"next")) return "next";
		if(checkTokens(suchThatPart,"affects")) return "affects";

		return "";
	}

	bool isString(string elem)
	{
		int pos1 = elem.find("\"");
		int pos2 = elem.find("\"",pos1+1);
		int pos3 = elem.find("\"",pos2+1);

		if(pos1 < pos2 && pos1 + 1 != pos2 && pos3 > elem.length())
			return true;
		else
			return false;
	}

	bool isStar(string elem, int position)
	{
		if(elem.find("*",position) < elem.length() && elem.find("*",position) == position)
			return true;
		else
			return false;
	}

	bool hasTwoElem(string elem)
	{
		int pos1 = elem.find(",");
		int pos2 = elem.find(",",pos1+1);

		if(pos1 < elem.length() && pos2 > elem.length())
			return true;
		else
			return false;
	}

	bool withHasTwoElem(string elem)
	{
		int pos1 = elem.find("=");
		int pos2 = elem.find("=",pos1+1);

		if(pos1 < elem.length() && pos2 > elem.length())
			return true;
		else
			return false;
	}

	bool isBracket(string elem)
	{
		int pos1 = elem.find("(");
		int pos2 = elem.find(")");

		if(pos1 == 0 && pos2 == elem.length() - 1)
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
		if(elem.find("_") < elem.length())
			return true;
		else
			return false;
	}

	bool isNumber(string elem)
	{
		for(int i = 0 ; i < elem.length() ; i ++)
		{
			if(!isdigit(elem[i]))
				return false;
		}
		return true;
	}
};
#endif /* SRC_PARSERPQL_MATCHERPQL_H_ */
