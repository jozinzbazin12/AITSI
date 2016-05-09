/*
 * Modifies.cpp
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#include "Modifies.h"
#include "../../globalVars.h"
Modifies::Modifies() {
	// TODO Auto-generated constructor stub

}

Modifies::~Modifies() {
	// TODO Auto-generated destructor stub
}

void Modifies::add(int varId, int lineNumber) {
	varModificationsLines[varId].push_back(lineNumber);
}

bool Modifies::modifies(int lineNumber, string varName) {
	if (varName == "") {
		for (map<int, vector<int> >::iterator iter = varModificationsLines.begin(); iter != varModificationsLines.end(); ++iter) {
			vector<int> tempVec = (*iter).second;
			for (unsigned i = 0; i < tempVec.size(); i++) {
				if(tempVec[i] == lineNumber){
					return true;
				}
			}
		}
		return false;
	}

	int varId = pkb -> getVarTable()->getVarId(varName);
	if (varId == -1)
		return false;
	if (varModificationsLines.count(varId) > 0) {
		for (vector<int>::iterator iter = varModificationsLines.at(varId).begin(); iter != varModificationsLines.at(varId).end(); ++iter) {
			if ((*iter) == lineNumber)
				return true;
		}
	}
	return false;
}

void Modifies::writeAll() {
	for (map<int, vector<int> >::iterator iter = varModificationsLines.begin();
			iter != varModificationsLines.end(); ++iter) {
		vector<int> tempVec = (*iter).second;
		int key = (*iter).first;
		cout << pkb->getVarTable()->getVarName(key) << " :";
		for (unsigned i = 0; i < tempVec.size(); i++) {
			cout << " " << tempVec[i];
		}
		cout << endl;
	}
}

