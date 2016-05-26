/*
 * VarTable.h
 *
 *  Created on: 27 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_PKB_TABLES_VARTABLE_H_
#define SRC_PKB_TABLES_VARTABLE_H_

#include<string>
#include<vector>
#include <map>
#include <iostream>

using namespace std;

class VarTable {
public:
	VarTable();
	virtual ~VarTable();
	virtual int addVar(string name);
	virtual int getVarId(string name);
	virtual string getVarName(int id);
	virtual void writeAll();
	virtual vector<string> getAllVar();

private:
	vector<string> variables;
	int varId;
};

#endif /* SRC_TABLES_VARTABLE_H_ */
