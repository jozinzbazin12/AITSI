/*
 * Next.cpp
 *
 *  Created on: 28 maj 2016
 *      Author: Pawel
 */

#include "Next.h"

Next::Next() {
	maxId = -1;
}

Next::~Next() {
	// TODO Auto-generated destructor stub
}

void Next::addNext(int lineNumber, int nextLineNumber) {

	if(lineNumber > maxId)
		maxId = lineNumber;
	if(nextLineNumber > maxId)
		maxId = nextLineNumber;

	if(nextLines.count(lineNumber) > 0) { //zapobieganie powtórkom
		vector<int> temp = nextLines[lineNumber];
		for (vector<int>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
			if((*iter) == nextLineNumber)
				return;
		}
	}
	nextLines[lineNumber].push_back(nextLineNumber);
}

bool Next::next(int lineNumber, int nextLineNumber) {
	if(nextLines.count(lineNumber) == 0)
		return false;

	for (vector<int>::iterator iter = nextLines[lineNumber].begin();
				iter != nextLines[lineNumber].end(); ++iter) {
		if((*iter) == nextLineNumber)
			return true;
	}

	return false;
}

bool Next::nextStar(int lineNumber, int nextLineNumber) {
	int const sooBigNumberCuzVisual = 653000;
	bool visited[sooBigNumberCuzVisual];
	for(int i=0; i<maxId+1; i++)
		visited[i] = false;

	return nextStarRecur(lineNumber, nextLineNumber, visited);
}

bool Next::nextStarRecur(int lineNumber, int nextLineNumber, bool visited[]) {
	if(next(lineNumber, nextLineNumber))
		return true;

	if(nextLines.count(lineNumber) > 0) {
		for (vector<int>::iterator iter = nextLines[lineNumber].begin(); iter != nextLines[lineNumber].end(); ++iter) {
			if(!visited[(*iter)]) {
				visited[(*iter)] = true;
				return nextStarRecur((*iter), nextLineNumber, visited);
			}
		}
	}
	return false;
}

void Next::writeAll() {
	for (map<int, vector<int> >::iterator iter = nextLines.begin(); iter != nextLines.end(); ++iter) {
		vector<int> tempVec = (*iter).second;
		cout << (*iter).first << " :";
		for (unsigned i = 0; i < tempVec.size(); i++) {
			cout << " " << tempVec[i];
		}
		cout << endl;
	}
}

