/*
 * ProcTable.h
 *
 *  Created on: 27 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_PKB_TABLES_PROCTABLE_H_
#define SRC_PKB_TABLES_PROCTABLE_H_

#include<string.h>
#include<map>

using namespace std;

class ProcTable {
public:
	ProcTable();
	virtual ~ProcTable();
	virtual int addProc(string name);
	virtual int getProcId(string name); // raczej metoda nie powinna byc uzywana
	virtual string getProcName(int id);

private:
	vector<string> procedures;
	int procId;
};

#endif /* SRC_TABLES_PROCTABLE_H_ */