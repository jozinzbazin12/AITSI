/*
 * Field.cpp
 *
 *  Created on: 29 mar 2016
 *      Author: Popek
 */

#include <iostream>
#include <sstream>
#include "Field.h"

Field::Field() {
	this->procName = false;
	this->varName = false;
	this->val = false;
	this->stmt = false;
}

Field::Field(string type, string value) {
	this->type = type;
	this->value = value;
	this->procName = false;
	this->varName = false;
	this->val = false;
	this->stmt = false;
}

Field::Field(string type, string value, bool procName, bool varName, bool val,
		bool stmt) {
	this->type = type;
	this->value = value;
	this->procName = procName;
	this->varName = varName;
	this->val = val;
	this->stmt = stmt;
}

Field::~Field() {
	// TODO Auto-generated destructor stub
}

bool Field::isStmt() {
	return stmt;
}

void Field::setStmt(bool stmt) {
	this->stmt = stmt;
}

string& Field::getType() {
	return type;
}

void Field::setType(string& type) {
	this->type = type;
}

bool Field::isVal() {
	return val;
}

void Field::setVal(bool val) {
	this->val = val;
}

string& Field::getValue() {
	return value;
}

void Field::setValue(string& value) {
	this->value = value;
}

bool Field::isProcName() {
	return procName;
}

void Field::setProcName(bool procName) {
	this->procName = procName;
}

bool Field::isVarName() {
	return varName;
}

void Field::setVarName(bool varName) {
	this->varName = varName;
}

string Field::printField()
{
	stringstream fieldText;
	fieldText << "Field: [" << this->type << " " << this->value << "] {procName: " << this->procName << ", varName: " << this->varName << ", stmt#: " << this->stmt << ", value: " << this->val << "}" << endl;
	return fieldText.str();
}
