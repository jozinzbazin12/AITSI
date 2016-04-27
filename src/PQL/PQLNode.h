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
	// for set field by yourself
	PQLNode() {
	}
	// for QueryNode, ResultMainNode, SuchMainNode
	PQLNode(string type) {
		this->type = type; // type = "queryNode" // type = "resultMainNode" // type = "suchMainNode"
	}
	// for ResultNode
	PQLNode(string type, Field* field1) {
		this->type = type; // type = "resultNode"
		this->field1 = field1;
	}
	// for SuchNode
	PQLNode(string type, string nodeType, Field* field1, Field* field2, bool star) {
		this->type = type; // type = "suchNode"
		this->nodeType = nodeType; // nodeType = "modifies" // nodeType = "parent" // nodeType = "follows" // nodeType = "uses" //
		this->field1 = field1;
		this->field2 = field2;
		this->star = star;
	}

	Field*& getField1() {
		return field1;
	}

	void setField1(Field*& field1) {
		this->field1 = field1;
	}

	Field*& getField2() {
		return field2;
	}

	void setField2(Field*& field2) {
		this->field2 = field2;
	}

	string& getNodeType() {
		return nodeType;
	}

	void setNodeType(string& nodeType) {
		this->nodeType = nodeType;
	}

	bool isStar() {
		return star;
	}

	void setStar(bool star) {
		this->star = star;
	}

	string& getType() {
		return type;
	}

	void setType(string& type) {
		this->type = type;
	}

private:
	// node type - queryNode, resultMainNode, resultNode, suchMainNode, suchNode, ...
	string type;
	// for Such That Node - parent, follows, modifies, uses, ...
	// for Other Node - ""
	string nodeType;
	// for Result Node - field
	// for Such Node - field (attributte 1 in relations)
	Field* field1;
	// for Result Node - empty
	// only for Such Node - field (attributte 2 in relations)
	Field* field2;
	// only for Such Field - relations with star or not
	bool star;
};

#endif /* SRC_PQL_PQLNODE_H_ */
