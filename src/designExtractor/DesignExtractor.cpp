/*
 * DesignExtractor.cpp
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#include "DesignExtractor.h"

DesignExtractor::DesignExtractor() {
	PKB pkb = PKB::getInstance();
	Follows * follows = pkb.getFollows();
	ASTTree * tree = pkb.getASTTree();
}

DesignExtractor::~DesignExtractor() {
	// TODO Auto-generated destructor stub
}

void DesignExtractor::fillFollowRelations() {

	PKB pkb = PKB::getInstance();
	Follows * follows = pkb.getFollows();
	ASTTree * ASTtree = pkb.getASTTree();

	map<int, int> nextFollow;    //1 arg - line num   , 2 - its follower
	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getRoot();
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getEnd();

	while (begin != end) {
		if (ASTtree->isValid(begin)) {
			if (!(*begin)->data) {
				cout << "error <fillNextFollows>" << endl;
			} else {
				tmp = begin.node->next_sibling;
				if (ASTtree->isValid(tmp)) {
					if ((*begin)->data->lineNumber
							!= (*tmp)->data->lineNumber) {
						follows->addNext((*begin)->data->id, (*tmp)->data->id);
						follows->addPrev((*tmp)->data->id, (*begin)->data->id);
					}
				}
			}
		}
		++begin;
	}
}
void DesignExtractor::fillParentRelations() {
	PKB pkb = PKB::getInstance();
	Parent * parent = pkb.getParent();
	ASTTree * ASTtree = pkb.getASTTree();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getRoot();
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getEnd();

	while (begin != end) {
		if (ASTtree->isValid(begin)) {
			if (!(*begin)->data) {
				cout << "error <fillParents>" << endl;
			} else {
				tmp = begin.node->parent;
				if (ASTtree->isValid(tmp)
						&& (*tmp)->data->type != "procedure") {
					if ((*begin)->data->lineNumber
							!= (*tmp)->data->lineNumber) {
						parent->addParent((*begin)->data->lineNumber,
								(*tmp)->data->lineNumber);
						parent->addKid((*tmp)->data->lineNumber,
								(*begin)->data->lineNumber);
					}

				}
			}
		}
		++begin;

	}
}

