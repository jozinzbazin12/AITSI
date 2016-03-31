/*
 * Modifies.cpp
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#include "Modifies.h"

Modifies::Modifies() {
	// TODO Auto-generated constructor stub

}

Modifies::~Modifies() {
	// TODO Auto-generated destructor stub
}

void Modifies::add(int varId, int lineNumber){
	varModificationsLines[varId].push_back(lineNumber);
}

bool Modifies::modifies(int lineNumber, string varName){
	// do uzupe³nenia
	return false;
}

void Modifies::writeAll(){
	for( map<int, vector<int> >::iterator iter = varModificationsLines.begin(); iter != varModificationsLines.end(); ++iter ) {
	   vector<int> tempVec = (*iter).second;
	   int key = (*iter).first;
	   cout << key << " :";
	   for (unsigned i = 0; i < tempVec.size(); i++) {
	      cout << " " << tempVec[i];
	   }
	   cout << endl;
	}
}

