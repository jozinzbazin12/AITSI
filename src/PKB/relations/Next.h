/*
 * Next.h
 *
 *  Created on: 28 maj 2016
 *      Author: Pawel
 */

#ifndef SRC_PKB_RELATIONS_NEXT_H_
#define SRC_PKB_RELATIONS_NEXT_H_

#include <map>
#include <vector>
#include <iostream>

using namespace std;

class Next {
public:
	Next();
	virtual ~Next();
	void addNext(int lineNumber, int nextLineNumber);
	void writeAll();

private:
	map <int, vector<int> > nextLines;
};

#endif /* SRC_PKB_RELATIONS_NEXT_H_ */
