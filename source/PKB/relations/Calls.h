/*
* Calls.h
*
*  Created on: 12 maj 2016
*      Author: Pawel
*/

#ifndef SRC_PKB_RELATIONS_CALLS_H_
#define SRC_PKB_RELATIONS_CALLS_H_

#include <map>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class Calls {
public:
	Calls();
	virtual ~Calls();
	void addCall(int callerId, int calleeId);
	bool calls(int callerId, int calleeId);
	bool callsStar(int callerId, int calleeId);
	vector<int> getCallees(int callerId);
	void writeAll();

private:
	int maxId;
	map <int, vector<int> > calleeIds;

	bool callsStarRecur(int callerId, int calleeId, bool visited[]);

};

#endif /* SRC_PKB_RELATIONS_CALLS_H_ */
