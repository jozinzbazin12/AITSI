/*
 * LinesTable.h
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_PKB_TABLES_LINESTABLE_H_
#define SRC_PKB_TABLES_LINESTABLE_H_

#include<vector>

using namespace std;

class LinesTable {
public:
	LinesTable();
	virtual ~LinesTable();
	vector<int> getIfLines();
	vector<int> getLoopLines();

private:

};

#endif /* SRC_PKB_TABLES_LINESTABLE_H_ */
