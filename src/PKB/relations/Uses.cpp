/*
 * Uses.cpp
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#include "Uses.h"
#include "../../globalVars.h"

Uses::Uses() {

}

Uses::~Uses() {
	// TODO Auto-generated destructor stub
}

void Uses::add(int varId, int lineNumber){
	varUsesLines[varId].push_back(lineNumber);
}

bool Uses::uses(int lineNumber, string varName){
	if (varName == "") {
		for (map<int, vector<int> >::iterator iter = varUsesLines.begin(); iter != varUsesLines.end(); ++iter) {
			vector<int> tempVec = (*iter).second;
			for (unsigned i = 0; i < tempVec.size(); i++) {
				if(tempVec[i] == lineNumber){
					return true;
				}
			}
		}
		return false;
	}

	int varId = pkb -> getVarTable() -> getVarId(varName);
	if (varId == -1)
		return false;
	if (varUsesLines.count(varId) > 0) {
		for (vector<int>::iterator iter = varUsesLines.at(varId).begin(); iter != varUsesLines.at(varId).end(); ++iter) {
			if ((*iter) == lineNumber)
				return true;
		}
	}
	return false;
}

void Uses::writeAll(){
	for( map<int, vector<int> >::iterator iter = varUsesLines.begin(); iter != varUsesLines.end(); ++iter ) {
	   vector<int> tempVec = (*iter).second;
	   int key = (*iter).first;
	   cout << pkb->varTable->getVarName(key) << " :";
	   for (unsigned i = 0; i < tempVec.size(); i++) {
	      cout << " " << tempVec[i];
	   }
	   cout << endl;
	}
}

