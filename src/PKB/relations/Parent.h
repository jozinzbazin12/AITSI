/*
 * Parent.h
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_PKB_RELATIONS_PARENT_H_
#define SRC_PKB_RELATIONS_PARENT_H_

#include "../tables/LinesTable.h"
#include <map>
using namespace std;

class Parent {
public:
	Parent();
	void addParent(int lineNumber, int parentLine);
	void addKid(int lineNumber, int kidLine);
	bool parent(int parentLine, int kidLine);
	void writeAll();
	virtual ~Parent();

private:
	map <int, int> lineParent;
	map <int, vector<int> > lineKids;
};

#endif /* SRC_PKB_RELATIONS_PARENT_H_ */
