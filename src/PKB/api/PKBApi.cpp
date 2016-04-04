/*
 * PKBApi.cpp
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#include "PKBApi.h"

PKBApi::PKBApi() {
	//pkb = PKB::getInstance();
}

PKBApi::~PKBApi() {
	// TODO Auto-generated destructor stub
}

bool PKBApi::modifies(int lineNumber, string varName){
	return pkb.getModifies() -> modifies(lineNumber, varName);
}

bool PKBApi::uses(int lineNumber, string varName){
	return pkb.getUses() -> uses(lineNumber, varName);
}

bool PKBApi::parent(int parentLine, int kidLine){
	return pkb.getParent() -> parent(parentLine, kidLine);
}

bool PKBApi::follows(int lineNum, int followerLineNum){
	return pkb.getFollows() -> follows(lineNum, followerLineNum);
}

vector<int> PKBApi::getAssignLines(){
	return pkb.getLineTable() -> getAssignLines();
}

map<int, vector<int> > PKBApi::getWhileLines(){
	return pkb.getLineTable() -> getWhileLines();
}

string PKBApi::getLineString(int lineNumber){
	return pkb.getLineTable() -> getLineString(lineNumber);
}

