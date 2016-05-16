/*
 * DesignExtractor.h
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#ifndef SRC_DESIGNEXTRACTOR_DESIGNEXTRACTOR_H_
#define SRC_DESIGNEXTRACTOR_DESIGNEXTRACTOR_H_
#include "../PKB/AST/tree.hh"
#include "../PKB/AST/ASTNode.h"
#include "../PKB/relations/Modifies.h"
#include "../PKB/tables/VarTable.h"

class DesignExtractor {
public:
	DesignExtractor();
	virtual ~DesignExtractor();
	void start();

private:
	void setFollowRelations();
	void setParentRelations();
	void setLoopsTable();
	void setAssignTable();
	void setModifiesRelations();
	void setUsesRelations();
	void setProcTable();
	void setCallsRelations();
	/*void recur(tree<tree_node_<ASTNode*>*>::iterator current,tree<tree_node_<ASTNode*>*>::iterator ifNode, Modifies * modifies,
	 VarTable * varTable);*/
	void ifRecur(tree<tree_node_<ASTNode*>*>::iterator current,
			tree<tree_node_<ASTNode*>*>::iterator ifNode);
	void whileRecur(tree<tree_node_<ASTNode*>*>::iterator current,
			tree<tree_node_<ASTNode*>*>::iterator whileNode);
	void setIfLines();
	void setCallLines();
};

#endif /* SRC_DESIGNEXTRACTOR_DESIGNEXTRACTOR_H_ */
