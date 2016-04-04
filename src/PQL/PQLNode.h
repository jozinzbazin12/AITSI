/*
 * PQLNode.h
 *
 *  Created on: 30 mar 2016
 *      Author: Popek
 */

#ifndef SRC_PQL_PQLNODE_H_
#define SRC_PQL_PQLNODE_H_

#include <string>
#include "Field.h"

using namespace std;

class PQLNode {
public:
	string type; // query (root), result, suchthat, with, pattern
	string nodeType;
};

class QueryNode: public PQLNode {
public:
	QueryNode() {
		this->type = "query";
		this->nodeType = "query";
	}

	void dupa()
	{
		cout << " DUPA ";
	}
};

class ResultMainNode: public PQLNode {
public:
	ResultMainNode() {
		this->type = "mainResult";
		this->nodeType = "mainResult";
	}
};

class ResultNode: public ResultMainNode {
public:
	ResultNode() {
		this->type = "result";
		this->nodeType = "result";
	}

	ResultNode(PQLNode& node) {}

	ResultNode(Field* field) {
		this->type = "result";
		this->nodeType = "result";
		this->field = field;
	}

public:
    Field* getField() {
		return field;
	}

	void setField(Field* field) {
		this->field = field;
	}

private:
	Field* field;
};

class SuchMainNode: public PQLNode {
public:
	SuchMainNode() {
		this->type = "mainSuch";
		this->nodeType = "mainSuch";
	}
};

class SuchNode: public SuchMainNode {
public:
	SuchNode() {
		this->type = "such";
		this->nodeType = "such";
		this->star = false;
	}

	SuchNode(string suchtype, Field* atr1, Field* atr2, bool star) {
		this->type = "such";
		this->nodeType = suchtype;
		this->suchtype = suchtype;
		this->attribute1 = atr1;
		this->attribute2 = atr2;
		this->star = star;
	}

	Field* getAttribute1() {
		return attribute1;
	}

	void setAttribute1(Field* attribute1) {
		this->attribute1 = attribute1;
	}

	Field* getAttribute2() {
		return attribute2;
	}

	void setAttribute2(Field* attribute2) {
		this->attribute2 = attribute2;
	}

	bool isStar() {
		return star;
	}

	void setStar(bool star) {
		this->star = star;
	}

	string& getSuchType() {
		return suchtype;
	}

	void setSuchType(string& suchtype) {
		this->suchtype = suchtype;
	}

private:
	string suchtype; // parent, follows, modifies, uses, affects, next, calls
	Field* attribute1; // 1st attribute in function
	Field* attribute2; // 2nd attribute in function
	bool star; // function with star or not - DEFAULT NOT
};

#endif /* SRC_PQL_PQLNODE_H_ */
