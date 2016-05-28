/*
 * Next.cpp
 *
 *  Created on: 28 maj 2016
 *      Author: Pawel
 */

#include "Next.h"

Next::Next() {
	// TODO Auto-generated constructor stub

}

Next::~Next() {
	// TODO Auto-generated destructor stub
}

void Next::addNext(int lineNumber, int nextLineNumber) {
	if(nextLines.count(lineNumber) > 0) { //zapobieganie powtórkom
		vector<int> temp = nextLines[lineNumber];
		for (vector<int>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
			if((*iter) == nextLineNumber)
				return;
		}
	}
	nextLines[lineNumber].push_back(nextLineNumber);
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

