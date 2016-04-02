/*
 * Uses.h
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_PKB_RELATIONS_USES_H_
#define SRC_PKB_RELATIONS_USES_H_


#include <map>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class Uses {
public:
	Uses();
	void add(int varId, int lineNumber);
	bool uses(int lineNumber, string varName); // jeœli varName puste to znaczy ¿e jakakolwiek zmienna
	void writeAll();
	virtual ~Uses();

private:
	map <int, vector<int> > varUsesLines;
};

#endif /* SRC_PKB_RELATIONS_USES_H_ */
