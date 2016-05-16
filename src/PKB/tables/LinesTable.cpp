/*
 * LinesTable.cpp
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#include "LinesTable.h"

LinesTable::LinesTable() {
	// TODO Auto-generated constructor stub
	linesCount = 0;
}

LinesTable::~LinesTable() {
	// TODO Auto-generated destructor stub
}

int LinesTable::getLinesCount(){
	return linesCount;
}

map<int, string> LinesTable::getAllLines(){
	return lines;
}

vector<int> LinesTable::getCallLines(){
	return callLines;
}

vector<int> LinesTable::getWhileLines(){
	vector<int> v;
	for(map<int, vector<int>>::iterator it = whileLines.begin(); it != whileLines.end(); ++it) {
		v.push_back(it->first);
	}
	return v;
}

vector<int> LinesTable::getIfLines(){
	vector<int> v;
	for(map<int, vector<int>>::iterator it = ifLines.begin(); it != ifLines.end(); ++it) {
		v.push_back(it->first);
	}
	return v;
}

map<int, vector<int>> LinesTable::getIfBodyLines(){
	return ifLines;
}

vector<int> LinesTable::getAssignLines(){
	return assignLines;
}

map<int,vector<int>> LinesTable::getWhileBodyLines(){
	return whileLines;
}

string LinesTable::getLineString(int lineNumber){
	if(lines.count(lineNumber) > 0)
		return lines[lineNumber];
	return "";
}

void LinesTable::addWhileLine(int loopLine, int stmtLine) {
	whileLines[loopLine].push_back(stmtLine);
}

void LinesTable::addIfLine(int ifLine, int stmtLine){
	ifLines[ifLine].push_back(stmtLine);
}

void LinesTable::addCallLine(int callLine){
	callLines.push_back(callLine);
}

void LinesTable::addAssignLine(int assignLine) {
	assignLines.push_back(assignLine);
}

void LinesTable::addLine(string text){
	linesCount ++;
	lines[linesCount] = text;
}

void LinesTable::writeAll() {
	for (map<int, string >::iterator iter = lines.begin();
			iter != lines.end(); ++iter) {
		cout << (*iter).first << " : " << (*iter).second << endl;
	}
}

void LinesTable::writeCallLines(){
	for (vector<int>::iterator iter = callLines.begin();
				iter != callLines.end(); ++iter) {
			cout << (*iter)<< endl;
		}
}


void LinesTable::writeIfLines() {
	for (map<int, vector<int> >::iterator iter = ifLines.begin();
				iter != ifLines.end(); ++iter) {
			vector<int> tempVec = (*iter).second;
			int key = (*iter).first;
			cout << key << " :";
			for (unsigned i = 0; i < tempVec.size(); i++) {
				cout << " " << tempVec[i];
			}
			cout << endl;
		}
}
void LinesTable::writeWhileLines() {
	for (map<int, vector<int> >::iterator iter = whileLines.begin();
				iter != whileLines.end(); ++iter) {
			vector<int> tempVec = (*iter).second;
			int key = (*iter).first;
			cout << key << " :";
			for (unsigned i = 0; i < tempVec.size(); i++) {
				cout << " " << tempVec[i];
			}
			cout << endl;
		}
}
