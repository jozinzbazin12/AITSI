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

void DesignExtractor::start() {
	setFollowRelations();
	setParentRelations();
	setLoopsTable();
	setAssignTable();
	setModifiesRelations();
	setUsesRelations();
	setIfLines();
	setCallLines();
}

void DesignExtractor::setFollowRelations() {

	Follows * follows = pkb->getFollows();
	ASTTree * ASTtree = pkb->getASTTree();

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
						follows->addNext((*begin)->data->lineNumber,
								(*tmp)->data->lineNumber);
						follows->addPrev((*tmp)->data->lineNumber,
								(*begin)->data->lineNumber);
					}
				}
			}
		}
		++begin;
	}
}
void DesignExtractor::setParentRelations() {
	Parent * parent = pkb->getParent();
	ASTTree * ASTtree = pkb->getASTTree();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getRoot();
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getEnd();

	while (begin != end) {
		if (ASTtree->isValid(begin)) {
			if (!(*begin)->data) {
				cout << "error <fillParents>" << endl;
			} else {
				tmp = begin.node->parent;
				if (ASTtree->isValid(tmp) && (*tmp)->data->type != "PROCEDURE"
						&& (*tmp)->data->type != "PROGRAM") {
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
	LinesTable * linesTable = pkb->getLineTable();
	ASTTree * ASTtree = pkb->getASTTree();

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
void DesignExtractor::setAssignTable() {
	LinesTable * linesTable = pkb->getLineTable();
	ASTTree * ASTtree = pkb->getASTTree();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getRoot();
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getEnd();

	while (begin != end) {
		if (ASTtree->isValid(begin) && (*begin)->data->type == "ASSIGN") {
			linesTable->addAssignLine((*begin)->data->lineNumber);
		}
		++begin;

	}
}
void DesignExtractor::setModifiesRelations() {

	Modifies * modifies = pkb->getModifies();
	ASTTree * ASTtree = pkb->getASTTree();
	VarTable * varTable = pkb->getVarTable();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getRoot();
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getEnd();

	while (begin != end) {
		if (ASTtree->isValid(begin) && (*begin)->data->type == "ASSIGN") {
			//linesTable->addAssignLine((*begin)->data->lineNumber);
			tmp = begin.node->first_child;
			int varId = varTable->getVarId((*tmp)->data->value);
			if (varId != -1)
				modifies->add(varId, (*begin)->data->lineNumber);
			else
				modifies->add(varTable->addVar((*tmp)->data->value),
						(*begin)->data->lineNumber);
		}
		++begin;

	}
}

void DesignExtractor::setUsesRelations() {
	Uses * uses = pkb->getUses();
	ASTTree * ASTtree = pkb->getASTTree();
	VarTable * varTable = pkb->getVarTable();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getRoot();
	tree<tree_node_<ASTNode*>*>::iterator firstChildOfAssign;
	tree<tree_node_<ASTNode*>*>::iterator secondChildOfAssign;
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getEnd();

	while (begin != end) {
		if (ASTtree->isValid(begin)) {
			if ((*begin)->data->type == "ASSIGN") {       //if assign only value
				firstChildOfAssign = begin.node->first_child;
				secondChildOfAssign = firstChildOfAssign.node->next_sibling;
				if (ASTtree->isValid(secondChildOfAssign)
						&& (*secondChildOfAssign)->data->type == "OPERAND") {
					int varId = varTable->getVarId(
							(*secondChildOfAssign)->data->value);
					if (varId != -1)
						uses->add(varId, (*begin)->data->lineNumber);
					/*else
					 uses->add(varTable->addVar((*secondChildOfAssign)->data->value),
					 (*begin)->data->lineNumber);*/

				}
			} else if ((*begin)->data->type == "WHILE") {              //in loop
				int varId = varTable->getVarId((*begin)->data->value);
				if (varId != -1)
					uses->add(varId, (*begin)->data->lineNumber);
				/*else
				 uses->add(varTable->addVar((*begin)->data->value),
				 (*begin)->data->lineNumber);*/
			} else if ((*begin)->data->type == "OPERAND") {   //under Expression
				tmp = begin.node->parent;
				if ((*tmp)->data->type == "EXPR") {
					int varId = varTable->getVarId((*begin)->data->value);
					if (varId != -1)
						uses->add(varId, (*begin)->data->lineNumber);
					/*else
					 uses->add(varTable->addVar((*begin)->data->value),
					 (*begin)->data->lineNumber);*/
				}

			}
		}
		++begin;

	}

}

void DesignExtractor::setIfLines() {

	ASTTree * ASTtree = pkb->getASTTree();
	LinesTable * linesTable = pkb->getLineTable();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getRoot();
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getEnd();

	tree<tree_node_<ASTNode*>*>::iterator ifstmt;
	tree<tree_node_<ASTNode*>*>::iterator elsetmt;

	while (begin != end) {
		if (ASTtree->isValid(begin) && (*begin)->data->type == "IF") {
			if (!(*begin)->data) {
				cout << "error <set If's>" << endl;
			} else {

				ifstmt = begin;
				elsetmt = begin.node->next_sibling;

				for (int i = 0; i < ASTtree->getNumberOfChildren(ifstmt); //stmts under if
						i++) {
					if (i == 0) {
						tmp = ifstmt.node->first_child;
					} else {
						tmp = tmp.node->next_sibling;
					}
					if (ASTtree->isValid(tmp))
						linesTable->addIfLine((*begin)->data->lineNumber,
								(*tmp)->data->lineNumber);

				}
				for (int i = 0; i < ASTtree->getNumberOfChildren(elsetmt); //stmts under else
						i++) {
					if (i == 0) {
						tmp = elsetmt.node->first_child;
					} else {
						tmp = tmp.node->next_sibling;
					}
					if (ASTtree->isValid(tmp))
						linesTable->addIfLine((*begin)->data->lineNumber,
								(*tmp)->data->lineNumber);

				}

			}
		}

		++begin;
	}
}
void DesignExtractor::setCallLines() {

	ASTTree * ASTtree = pkb->getASTTree();
	LinesTable * linesTable = pkb->getLineTable();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getRoot();
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getEnd();

	while (begin != end) {
		if (ASTtree->isValid(begin) && (*begin)->data->type == "CALL") {
			if (!(*begin)->data) {
				cout << "error <set call's>" << endl;
			} else {
				linesTable->addCallLine((*begin)->data->lineNumber);
			}
		}
		++begin;
	}

}

