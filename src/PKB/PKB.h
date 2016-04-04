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

#include "relations/Parent.h"
#include "relations/Follows.h"

#include "relations/Uses.h"
#include "relations/Modifies.h"

class PKB {
public:
	static PKB& getInstance();

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

	ASTTree* getASTTree(){
		if(tree == NULL)
			setASTTree();
		return tree;
	}

	VarTable* getVarTable(){
		return varTable;
	}

	LinesTable* getLineTable(){
			return lineTable;
	}

	void setASTTree(){

	}

private:
	Modifies* modifies = NULL;
	Uses* uses = NULL;
	Parent* parent = NULL;
	Follows* follows = NULL;
	ASTTree * tree = NULL;
	VarTable * varTable = NULL;
	LinesTable * lineTable = NULL;

	PKB(){
		modifies = new Modifies();
		uses = new Uses();
		parent = new Parent();
		follows = new Follows();
		varTable = new VarTable();
		lineTable = new LinesTable();
	}
};

#endif /* SRC_PKB_PKB_H_ */
