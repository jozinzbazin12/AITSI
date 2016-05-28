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

bool PKBApi::parentStar(int parentLine, int kidLine) {
	return pkb -> getParent() -> parentStar(parentLine, kidLine);
}

bool PKBApi::follows(int lineNum, int followerLineNum){
	return pkb -> getFollows() -> follows(lineNum, followerLineNum);
}

bool PKBApi::followsStar(int lineNum, int followerLineNum) {
	return pkb -> getFollows() -> followsStar(lineNum, followerLineNum);
}

bool PKBApi::calls(int callerId, int calleeId) {
	return pkb -> getCalls() -> calls(callerId, calleeId);
}

bool PKBApi::callsStar(int callerId, int calleeId) {
	return pkb -> getCalls() -> callsStar(callerId, calleeId);
}

bool PKBApi::next(int lineNumber, int nextLineNumber) {
	return pkb -> getNext() -> next(lineNumber, nextLineNumber);
}

bool PKBApi::nextStar(int lineNumber, int nextLineNumber) {
	return pkb -> getNext() -> nextStar(lineNumber, nextLineNumber);
}

/////////////////////////////////////////////////////////////////////////////

int PKBApi::getLinesCount(){
	return pkb -> getLineTable() -> getLinesCount();
}

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

vector<int> PKBApi::getProceduresLines(){
	return pkb -> getProcTable() -> getProceduresLines();
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

////////////////////////////////////////////////////////////

int PKBApi::getProcId(string name) {
	return pkb -> getProcTable() -> getProcId(name);
}

int PKBApi::getProcId(int startLine) {
	return pkb -> getProcTable() -> getProcId(startLine);
}

int PKBApi::getMaxProcId() {
	return pkb -> getProcTable() -> getMaxProcId();
}

int PKBApi::getProcStartLine(int id) {
	return pkb -> getProcTable() -> getProcStartLine(id);
}

int PKBApi::getProcStartLine(string name) {
	return pkb -> getProcTable() -> getProcStartLine(name);
}

string PKBApi::getProcName(int id) {
	return pkb -> getProcTable() -> getProcName(id);
}

vector<int> PKBApi::getProcedureBodyLines(int procId) {
	return pkb -> getProcTable() -> getProcedureBodyLines(procId);
}

set<int> PKBApi::getProcedureBodyLinesStar(int procId) {
	return pkb -> getProcTable() -> getProcedureBodyLinesStar(procId, pkb -> getCalls());
}

