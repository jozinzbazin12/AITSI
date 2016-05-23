/*
* Parent.cpp
*
*  Created on: 29 mar 2016
*      Author: Pawel
*/

#include "Parent.h"

Parent::Parent() {
	// TODO Auto-generated constructor stub

}

Parent::~Parent() {
	// TODO Auto-generated destructor stub
}

void Parent::addParent(int lineNumber, int parentLine) {
	lineParent[lineNumber] = parentLine;
}

void Parent::addKid(int lineNumber, int kidLine) {
	lineKids[lineNumber].push_back(kidLine);
}

bool Parent::parent(int parentLine, int kidLine) {
	if (lineParent.count(kidLine) > 0) {
		return lineParent.at(kidLine) == parentLine;
	}
	return false;
}

bool Parent::parentStar(int parentLine, int kidLine) {
	if (parent(parentLine, kidLine)) {
		return true;
	}

	if (lineParent.count(kidLine) > 0) {
		return parentStar(parentLine, lineParent.at(kidLine));
	}

	return false;
}

void Parent::writeAll() {
	for (map<int, vector<int> >::iterator iter = lineKids.begin();
		iter != lineKids.end(); ++iter) {
		vector<int> tempVec = (*iter).second;
		int key = (*iter).first;
		cout << key << " :";
		for (unsigned i = 0; i < tempVec.size(); i++) {
			cout << " " << tempVec[i];
		}
		cout << endl;
	}

	/*	cout << "\nparents\n";

	for (map<int, int>::iterator iter = lineParent.begin();
	iter != lineParent.end(); ++iter) {
	int value = (*iter).second;
	int key = (*iter).first;
	cout << key << " : " << value << endl;
	}*/
}

