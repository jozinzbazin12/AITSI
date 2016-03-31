/*
 * Modifies.h
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_PKB_RELATIONS_MODIFIES_H_
#define SRC_PKB_RELATIONS_MODIFIES_H_

#include <iostream>
#include <vector>
#include <map>

#include "../tables/LinesTable.h"

using namespace std;

class Modifies {
public:
	Modifies();
	virtual ~Modifies();
	void add(int varId, int lineNumber);

	//tutaj trzeba dodac metody do odpowiadania na pytania
	bool modifies(int lineNumber, string varName); // jeœli varName puste to znaczy ¿e jakakolwiek zmienna
	bool modifies(string procName, string varName);
	bool modifies(EnumStmt enumStmt, string varName);

	void writeAll();

private:
	map <int, vector<int> > varModificationsLines;
};

#endif /* SRC_PKB_RELATIONS_MODIFIES_H_ */
