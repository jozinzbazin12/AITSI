/*
 * Uses.cpp
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#include "Uses.h"

Uses::Uses() {
	// TODO Auto-generated constructor stub

}

Uses::~Uses() {
	// TODO Auto-generated destructor stub
}

void Uses::add(int varId, int lineNumber){
	varUsesLines[varId].push_back(lineNumber);
}

bool Uses::uses(int lineNumber, string varName){
	// do uzupe³nenia
	return false;
}

void Uses::writeAll(){
	for( map<int, vector<int> >::iterator iter = varUsesLines.begin(); iter != varUsesLines.end(); ++iter ) {
	   vector<int> tempVec = (*iter).second;
	   int key = (*iter).first;
	   cout << key << " :";
	   for (unsigned i = 0; i < tempVec.size(); i++) {
	      cout << " " << tempVec[i];
	   }
	   cout << endl;
	}
}

