/*
 * PKB.h
 *
 *  Created on: 31 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_PKB_PKB_H_
#define SRC_PKB_PKB_H_

#include<iostream>

#include "relations/Modifies.h"
#include "relations/Uses.h"
#include "relations/Parent.h"
#include "relations/Follows.h"

class PKB {
public:
	PKB*& getInstance();
	virtual ~PKB();

	Follows*& getFollows() {
		return follows;
	}

	Modifies*& getModifies() {
		return modifies;
	}

	Parent*& getParent() {
		return parent;
	}

	Uses*& getUses() {
		return uses;
	}

private:
	PKB * pkb = NULL;
	Modifies * modifies = NULL;
	Uses * uses = NULL;
	Parent * parent = NULL;
	Follows * follows = NULL;

	PKB(){
		modifies = new Modifies();
		uses = new Uses();
		parent = new Parent();
		follows = new Follows();
	}
};

#endif /* SRC_PKB_PKB_H_ */
