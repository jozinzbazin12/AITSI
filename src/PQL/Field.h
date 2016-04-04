/*
 * Field.h
 *
 *  Created on: 29 mar 2016
 *      Author: Popek
 */

#ifndef SRC_TREE_FIELD_H_
#define SRC_TREE_FIELD_H_

#include <string>

using namespace std;

class Field {
public:
	Field();
	Field(string type, string value);
	Field(string type, string value, bool procName, bool varName, bool val, bool stmt);
	virtual ~Field();
	bool isStmt();
	void setStmt(bool stmt);
	string& getType();
	void setType(string& type);
	bool isVal();
	void setVal(bool val);
	string& getValue();
	void setValue(string& value);
	bool isProcName();
	void setProcName(bool procName);
	bool isVarName();
	void setVarName(bool varName);
	string printField();


private:
	string type;
	string value;
	bool procName; // DEFAULT FALSE
	bool varName; // DEFAULT FALSE
	bool val; // DEFAULT FALSE
	bool stmt; // DEFAULT FALSE
};

#endif /* SRC_TREE_FIELD_H_ */
