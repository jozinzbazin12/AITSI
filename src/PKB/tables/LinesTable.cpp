/*
 * LinesTable.cpp
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#include "LinesTable.h"

LinesTable::LinesTable() {
	// TODO Auto-generated constructor stub

}

LinesTable::~LinesTable() {
	// TODO Auto-generated destructor stub
}

void LinesTable::addWhileLine(int loopLine, int stmtLine) {
	whileLines[loopLine].push_back(stmtLine);
}

void LinesTable::addAssignLine(int assignLine) {
	assignLines.push_back(assignLine);
}

void LinesTable::addLine(int lineNumber, string text){
	lines[lineNumber] = text;
}

vector<int> LinesTable::getAssignLines(){
	return assignLines;
}

map<int,vector<int>> LinesTable::getWhileLines(){
	return whileLines;
}

string LinesTable::getLineString(int lineNumber){
	if(lines.count(lineNumber) > 0)
		return lines[lineNumber];
	return "";
}
