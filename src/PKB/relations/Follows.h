/*
 * Follows.h
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_PKB_RELATIONS_FOLLOWS_H_
#define SRC_PKB_RELATIONS_FOLLOWS_H_

#include <map>

using namespace std;

class Follows {
public:
	Follows();
	virtual ~Follows();
	void addNext(int lineNum, int FollowerLineNum);
	void addPrev(int lineNum, int PrevLineNum);
private:
	map <int, int > varNextFollowers;
	map <int, int > varPrevFollowers;
};


#endif /* SRC_PKB_RELATIONS_FOLLOWS_H_ */
