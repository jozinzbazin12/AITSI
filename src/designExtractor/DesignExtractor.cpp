/*
 * DesignExtractor.cpp
 *
 *  Created on: 29 mar 2016
 *      Author: Pawel
 */

#include "DesignExtractor.h"
#include "../globalVars.h"

DesignExtractor::DesignExtractor() {
}

DesignExtractor::~DesignExtractor() {
	// TODO Auto-generated destructor stub
}

void DesignExtractor::start(){
	setFollowRelations();
	setParentRelations();
	setLoopsTable();
	setAssignTable();
}

void DesignExtractor::setFollowRelations() {

	Follows * follows = pkb -> getFollows();
	ASTTree * ASTtree = pkb -> getASTTree();

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
						follows->addNext((*begin)->data->lineNumber, (*tmp)->data->lineNumber);
						follows->addPrev((*tmp)->data->lineNumber, (*begin)->data->lineNumber);
					}
				}
			}
		}
		++begin;
	}
}
void DesignExtractor::setParentRelations() {
	Parent * parent = pkb -> getParent();
	ASTTree * ASTtree = pkb -> getASTTree();

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
						&& (*tmp)->data->type != "procedure" && (*tmp)->data->type != "PROGRAM") {
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
	LinesTable * linesTable = pkb -> getLineTable();
	ASTTree * ASTtree = pkb -> getASTTree();

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
				sib = stmtlst; //first child of stmtlist   //7  //THERE IS NO STMTLST
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
		LinesTable * linesTable = pkb -> getLineTable();
		ASTTree * ASTtree = pkb -> getASTTree();

		tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getRoot();
				tree<tree_node_<ASTNode*>*>::iterator tmp;
				tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getEnd();

				while (begin != end) {
					if (ASTtree->isValid(begin) && (*begin)->data->type == "=") {
						linesTable->addAssignLine((*begin)->data->lineNumber);
					}
					++begin;

				}
}
