/*
 * PKBApi.cpp
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#include "PKBApi.h"
#include "../../globalVars.h"

PKBApi::PKBApi() {

}

PKBApi::~PKBApi() {
	// TODO Auto-generated destructor stub
}

bool PKBApi::modifies(int lineNumber, string varName){
	return pkb -> getModifies() -> modifies(lineNumber, varName);
}

bool PKBApi::uses(int lineNumber, string varName){
	return pkb -> getUses() -> uses(lineNumber, varName);
}

bool PKBApi::parent(int parentLine, int kidLine){
	return pkb -> getParent() -> parent(parentLine, kidLine);
}

bool PKBApi::follows(int lineNum, int followerLineNum){
	return pkb -> getFollows() -> follows(lineNum, followerLineNum);
}

int getLinesCount();

vector<int> PKBApi::getAssignLines(){
	return pkb -> getLineTable() -> getAssignLines();
}

map<int, string> PKBApi::getAllLines(){
	return pkb -> getLineTable() -> getAllLines();
}

vector<int> PKBApi::getCallLines(){
	return pkb -> getLineTable() -> getCallLines();
}

vector<int> PKBApi::getWhileLines(){
	return pkb -> getLineTable() -> getWhileLines();
}

vector<int> PKBApi::getIfLines(){
	return pkb -> getLineTable() -> getIfLines();
}

map<int, vector<int>> PKBApi::getIfBodyLines(){
	return pkb -> getLineTable() -> getIfBodyLines();
}

map<int, vector<int> > PKBApi::getWhileBodyLines(){
	return pkb -> getLineTable() -> getWhileBodyLines();
}

string PKBApi::getLineString(int lineNumber){
	return pkb -> getLineTable() -> getLineString(lineNumber);
}

