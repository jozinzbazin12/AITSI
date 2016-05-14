/*
 * Calls.cpp
 *
 *  Created on: 12 maj 2016
 *      Author: Pawel
 */

#include "Calls.h"

Calls::Calls() {
	// TODO Auto-generated constructor stub

}

Calls::~Calls() {
	// TODO Auto-generated destructor stub
}

void Calls::addCall(int callerId, int calleeId){
	if(calleeIds.count(callerId) > 0){
		vector<int> temp = calleeIds[callerId];
		for (vector<int>::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
			if((*iter) == calleeId)
				return;
		}

	}
	calleeIds[callerId].push_back(calleeId);
}

bool Calls::calls(int callerId, int calleeId){

	if(callerId == -1 && calleeId == -1){
		return !calleeIds.empty();
	}

	if(callerId == -1){
		for (map<int, vector<int>>::iterator iter = calleeIds.begin();
						iter != calleeIds.end(); ++iter) {
			vector<int> x = (*iter).second;
			for (vector<int>::iterator iter = x.begin();
						iter != x.end(); ++iter) {
				if((*iter) == calleeId)
					return true;
			}
		}
		return false;
	}

	if(calleeIds.count(callerId) == 0)
		return false;

	if(calleeId == -1){
		vector<int> x = calleeIds[callerId];
		return !x.empty();
	}

	for (vector<int>::iterator iter = calleeIds[callerId].begin();
				iter != calleeIds[callerId].end(); ++iter) {
		if((*iter) == calleeId)
			return true;
	}

	return false;
}

bool Calls::callsStar(int callerId, int calleeId){
	if(calls(callerId, calleeId))
		return true;

	if(calleeIds.count(callerId) > 0){
		for (vector<int>::iterator iter = calleeIds[callerId].begin(); iter != calleeIds[callerId].end(); ++iter) {
			return callsStar((*iter), calleeId);
		}
	}

	return false;
}

void Calls::writeAll(){
	for (map<int, vector<int> >::iterator iter = calleeIds.begin();
				iter != calleeIds.end(); ++iter) {
			vector<int> tempVec = (*iter).second;
			cout << (*iter).first << " :";
			for (unsigned i = 0; i < tempVec.size(); i++) {
				cout << " " << tempVec[i];
			}
			cout << endl;
		}
}
