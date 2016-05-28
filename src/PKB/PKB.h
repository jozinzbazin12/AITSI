/*
 * PKB.h
 *
 *  Created on: 31 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_PKB_PKB_H_
#define SRC_PKB_PKB_H_

#include <iostream>
#include "ast/tree_util.hh"

#include "ast/ASTNode.h"
#include "ast/ASTTree.h"
#include "tables/VarTable.h"
#include "tables/LinesTable.h"
#include "tables/ProcTable.h"

#include "relations/Parent.h"
#include "relations/Follows.h"

#include "relations/Uses.h"
#include "relations/Modifies.h"

#include "relations/Calls.h"
#include "relations/Next.h"

class PKB {
public:

	PKB(){
		modifies = new Modifies();
		uses = new Uses();
		parent = new Parent();
		follows = new Follows();
		calls = new Calls();
		next = new Next();
		varTable = new VarTable();
		lineTable = new LinesTable();
		procTable = new ProcTable();
	}

	virtual ~PKB();

	Follows* getFollows() {
		return follows;
	}

	Modifies* getModifies() {
		return modifies;
	}

	Parent* getParent() {
		return parent;
	}

	Uses* getUses() {
		return uses;
	}

	Calls* getCalls() {
		return calls;
	}

	Next* getNext() {
		return next;
	}

	ASTTree* getASTTree(){
		return tree;
	}

	VarTable* getVarTable(){
		return varTable;
	}

	LinesTable* getLineTable(){
		return lineTable;
	}

	ProcTable* getProcTable(){
		return procTable;
	}

	void setASTTree(ASTTree * tree2){
		tree = tree2;
	}
private:
	Modifies* modifies = NULL;
	Uses* uses = NULL;
	Parent* parent = NULL;
	Follows* follows = NULL;
	Calls* calls = NULL;
	Next* next = NULL;
	ASTTree * tree = NULL;
	VarTable * varTable = NULL;
	LinesTable * lineTable = NULL;
	ProcTable * procTable = NULL;
};

#endif /* SRC_PKB_PKB_H_ */
