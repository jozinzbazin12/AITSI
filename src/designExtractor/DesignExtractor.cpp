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

void DesignExtractor::setFollowRelations() {

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
void DesignExtractor::setParentRelations() {
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
void DesignExtractor::setLoopsTable() {
	PKB pkb = PKB::getInstance();
	LinesTable * linesTable = pkb.getLineTable();
	ASTTree * ASTtree = pkb.getASTTree();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getRoot();
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getEnd();
	tree<tree_node_<ASTNode*>*>::iterator stmtlst;
	tree<tree_node_<ASTNode*>*>::iterator sib;

	while (begin != end) {
		if (ASTtree->isValid(begin) && (*begin)->data->type == "LOOP") {
			if (!(*begin)->data) {
				cout << "error <fillLoops>" << endl;
			} else {
				stmtlst = begin.node->first_child;  //stmtlst    //
				sib = ASTtree->getFirstChild(stmtlst); //first child of stmtlist   //7
				linesTable->addWhileLine((*begin)->data->lineNumber,
						(*sib)->data->lineNumber);
				for (int i = 0; i < ASTtree->getNumberOfChildren(stmtlst);
						i++) {
					sib = sib.node->next_sibling;
					if (ASTtree->isValid(sib))
						linesTable->addWhileLine((*begin)->data->lineNumber,
								(*sib)->data->lineNumber);
				}

			}
		}

		++begin;
	}

}
void DesignExtractor::setAssignTable(){
	PKB pkb = PKB::getInstance();
		LinesTable * linesTable = pkb.getLineTable();
		ASTTree * ASTtree = pkb.getASTTree();

		tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getRoot();
				tree<tree_node_<ASTNode*>*>::iterator tmp;
				tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getEnd();

				while (begin != end) {
					if (ASTtree->isValid(begin) && (*begin)->data->type == "ASSIGN") {
						linesTable->addAssignLine((*begin)->data->lineNumber);
					}
					++begin;

				}
}
