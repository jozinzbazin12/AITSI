/*
 * VarTable.h
 *
 *  Created on: 27 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_PKB_TABLES_VARTABLE_H_
#define SRC_PKB_TABLES_VARTABLE_H_

#include<string.h>
#include<map>

using namespace std;

class VarTable {
public:
	VarTable();
	virtual ~VarTable();
	virtual int addVar(string name);
	virtual int getVarId(string name); // raczej metoda nie powinna byc uzywana
	virtual string getVarName(int id);

private:
	vector<string> variables;
	int varId;
};

#endif /* SRC_TABLES_VARTABLE_H_ */
