/*
 * LinesTable.h
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_PKB_TABLES_LINESTABLE_H_
#define SRC_PKB_TABLES_LINESTABLE_H_

#include <vector>
#include <map>

using namespace std;

class LinesTable {
public:
	LinesTable();
	virtual ~LinesTable();
	vector<int> getAssignLines();
	map<int, vector<int> > getWhileLines(); // w pierwszym parametrze nr tej linijki z napisem while, a w vectorze pozosta³e liniie while
	// jak bêd¹ potrzebowali stmt to chyba mog¹ sobie zrobic assign i while zamiast pobierac stmt
	string getLineString(int lineNumber); // zwraca test który byl w linii o tym numerze
	void addWhileLine(int loopLine,int stmtLine);
	void addAssignLine(int assignLine);
	void addLine(int lineNumber, string text);

private:

	map<int, string> lines; // tutaj po prostu dla kolejnych numerów linii ich tekst (mo¿na na vector zamienic)
	vector<int> assignLines;
	map<int, vector<int>> whileLines;

};

#endif /* SRC_PKB_TABLES_LINESTABLE_H_ */
