/*
 * Follows.cpp
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#include "Follows.h"

Follows::Follows() {
	// TODO Auto-generated constructor stub

}

Follows::~Follows() {
	// TODO Auto-generated destructor stub
}

void Follows::addNext(int lineNum,int followLineNum){
	varNextFollowers[lineNum] = followLineNum;
}
void Follows::addPrev(int lineNum,int prevLineNum){
	varPrevFollowers[lineNum] = prevLineNum;
}
