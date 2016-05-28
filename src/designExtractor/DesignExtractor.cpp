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
	setProcTable();
	setCallsRelations();
	setNextRelations();
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
	tree<tree_node_<ASTNode*>*>::iterator prevSib; //if node
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getEnd();

	while (begin != end) {
		if (ASTtree->isValid(begin)) {
			if (!(*begin)->data) {
				cout << "error <fillParents>" << endl;
			} else {
				tmp = begin.node->parent;        //tmp = parent    begin - child
				if (ASTtree->isValid(tmp) && (*tmp)->data->type != "PROCEDURE"
						&& (*tmp)->data->type != "PROGRAM") {
					if ((*tmp)->data->type == "ELSE") {
						prevSib = tmp.node->prev_sibling;  //getting if node
						if (ASTtree->isValid(prevSib)) {
							parent->addParent((*begin)->data->lineNumber,
									(*prevSib)->data->lineNumber);
							parent->addKid((*prevSib)->data->lineNumber,
									(*begin)->data->lineNumber);
						}
					} else if ((*begin)->data->lineNumber
							!= (*tmp)->data->lineNumber && (*begin)->data->type != "ELSE") {
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
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	while (begin != end) {
		if (ASTtree->isValid(begin) && (*begin)->data->type == "WHILE") {
			if (!(*begin)->data) {
				cout << "error <fillLoops>" << endl;
			} else {
				for (int i = 0; i < ASTtree->getNumberOfChildren(begin); i++) {
					if (i == 0) {
						tmp = begin.node->first_child;
					} else {
						tmp = tmp.node->next_sibling;
					}
					if (ASTtree->isValid(tmp)) {
						if ((*tmp)->data->type == "IF"
								|| (*tmp)->data->type == "ELSE"
								|| (*tmp)->data->type == "WHILE") {
							whileRecur(tmp, begin);
						} else
							linesTable->addWhileLine((*begin)->data->lineNumber,
									(*tmp)->data->lineNumber);
					}
				}

			}
		}

		++begin;
	}

}
void DesignExtractor::whileRecur(tree<tree_node_<ASTNode*>*>::iterator current,
		tree<tree_node_<ASTNode*>*>::iterator whileNode) {

	tree<tree_node_<ASTNode*>*>::iterator tmp;
	ASTTree * ASTtree = pkb->getASTTree();
	LinesTable * linesTable = pkb->getLineTable();

	linesTable->addWhileLine((*whileNode)->data->lineNumber,
			(*current)->data->lineNumber);

	for (int i = 0; i < ASTtree->getNumberOfChildren(current); i++) {

		tmp = ASTtree->getChild(current, i);
		if (ASTtree->isValid(tmp)) {

			if ((*tmp)->data->type == "IF" || (*tmp)->data->type == "ELSE"
					|| (*tmp)->data->type == "WHILE") {
				whileRecur(tmp, whileNode);

			} else {
				linesTable->addWhileLine((*whileNode)->data->lineNumber,
						(*tmp)->data->lineNumber);
			}

		}
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

		/*else if (ASTtree->isValid(begin) && (*begin)->data->type == "IF") {
		 tmp = ASTtree->getNextSibling(begin);
		 recur(begin, begin, modifies, varTable);
		 recur(tmp, begin, modifies, varTable);
		 //get all childs

		 /*for (int i = 0; i < ASTtree->getNumberOfChildren(begin); i++) {
		 tmp = ASTtree->getChild(begin, i);
		 if ((*tmp)->data->type == "ASSIGN") {

		 int varId = varTable->getVarId(
		 tmp.node->first_child->data->data->value);

		 cout << "Node (child of IF) under if assign value : "
		 << tmp.node->first_child->data->data->value
		 << " And the id node line : "
		 << (*begin)->data->lineNumber << " varId : "
		 << varId << endl;
		 if (varId != -1)
		 modifies->add(varId, (*begin)->data->lineNumber);
		 else {
		 modifies->add(
		 varTable->addVar(
		 tmp.node->first_child->data->data->value),
		 (*begin)->data->lineNumber);
		 }
		 } else if ((*tmp)->data->type == "IF"
		 || (*tmp)->data->type == "ELSE") {

		 cout << " ------- IF LINE :  " << (*tmp)->data->lineNumber << endl;
		 recur(tmp, begin, modifies, varTable);
		 }

		 //get else
		 //}
		 //first sib of if
		 //else
		 } else if (ASTtree->isValid(begin) && (*begin)->data->type == "WHILE") {
		 recur(begin, begin, modifies, varTable);
		 }*/

		++begin;

	}

}
/*void DesignExtractor::recur(tree<tree_node_<ASTNode*>*>::iterator current,
 tree<tree_node_<ASTNode*>*>::iterator ifNode, Modifies * modifies,
 VarTable * varTable) {

 tree<tree_node_<ASTNode*>*>::iterator tmp;
 ASTTree * ASTtree = pkb->getASTTree();
 cout << "Enter recur funct" << endl;

 for (int i = 0; i < ASTtree->getNumberOfChildren(current); i++) {

 tmp = ASTtree->getChild(current, i);
 cout << "Children of node : " << (*tmp)->data->type << " Line num :  "
 << (*tmp)->data->lineNumber << endl;

 if ((*tmp)->data->type == "ASSIGN") {
 int varId = varTable->getVarId(
 tmp.node->first_child->data->data->value);
 if (varId != -1)
 modifies->add(varId, (*ifNode)->data->lineNumber);
 else {
 modifies->add(
 varTable->addVar(
 tmp.node->first_child->data->data->value),
 (*ifNode)->data->lineNumber);
 }
 /*cout << "Node in recur ASSIGN under if assign value : "
 << tmp.node->first_child->data->data->value
 << " And the id node line : "
 << tmp.node->first_child->data->data->lineNumber
 << " varId : " << varTable->getVarId((*tmp)->data->value) << " IfNode line :  "
 << ifNode.node->data->data->lineNumber << endl;
 }

 else if (ASTtree->isValid(tmp)
 && ((*tmp)->data->type == "IF" || (*tmp)->data->type == "ELSE"
 || (*tmp)->data->type == "WHILE")) {
 cout << " ------- IF LINE OR WHILE in rec:  "
 << (*tmp)->data->lineNumber << endl;
 recur(tmp, ifNode, modifies, varTable);
 } else
 return;
 }

 }*/

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
					else
					 uses->add(varTable->addVar((*secondChildOfAssign)->data->value),
					 (*begin)->data->lineNumber);

				}
			} else if ((*begin)->data->type == "WHILE") {              //in loop
				int varId = varTable->getVarId((*begin)->data->value);
				if (varId != -1)
					uses->add(varId, (*begin)->data->lineNumber);
				else
				 uses->add(varTable->addVar((*begin)->data->value),
				 (*begin)->data->lineNumber);
			}else if ((*begin)->data->type == "IF") {              //in loop
				int varId = varTable->getVarId((*begin)->data->value);
				if (varId != -1)
					uses->add(varId, (*begin)->data->lineNumber);
				else
				 uses->add(varTable->addVar((*begin)->data->value),
				 (*begin)->data->lineNumber);
			} else if ((*begin)->data->type == "OPERAND") {   //under Expression
				tmp = begin.node->parent;
				if ((*tmp)->data->type == "EXPR") {
					int varId = varTable->getVarId((*begin)->data->value);
					if (varId != -1)
						uses->add(varId, (*begin)->data->lineNumber);
					else
					 uses->add(varTable->addVar((*begin)->data->value),
					 (*begin)->data->lineNumber);
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

				if (ASTtree->isValid(ifstmt))
					for (int i = 0; i < ASTtree->getNumberOfChildren(ifstmt); //stmts under if
							i++) {
						if (i == 0) {
							tmp = ifstmt.node->first_child;
						} else {
							tmp = tmp.node->next_sibling;
						}
						if (ASTtree->isValid(tmp)) {
							if ((*tmp)->data->type == "IF"
									|| (*tmp)->data->type == "ELSE"
									|| (*tmp)->data->type == "WHILE") {
								ifRecur(tmp, begin);
							} else
								linesTable->addIfLine(
										(*begin)->data->lineNumber,
										(*tmp)->data->lineNumber);
						}

					}
				if (ASTtree->isValid(elsetmt))
					for (int i = 0; i < ASTtree->getNumberOfChildren(elsetmt); //stmts under else
							i++) {
						if (i == 0) {
							tmp = elsetmt.node->first_child;
						} else {
							tmp = tmp.node->next_sibling;
						}
						if (ASTtree->isValid(tmp)) {
							if ((*tmp)->data->type == "IF"
									|| (*tmp)->data->type == "ELSE"
									|| (*tmp)->data->type == "WHILE") {
								ifRecur(tmp, begin);
							} else
								linesTable->addIfLine(
										(*begin)->data->lineNumber,
										(*tmp)->data->lineNumber);
						}

					}

			}
		}

		++begin;
	}
}
void DesignExtractor::ifRecur(tree<tree_node_<ASTNode*>*>::iterator current,
		tree<tree_node_<ASTNode*>*>::iterator ifNode) {

	tree<tree_node_<ASTNode*>*>::iterator tmp;
	ASTTree * ASTtree = pkb->getASTTree();
	LinesTable * linesTable = pkb->getLineTable();

	linesTable->addIfLine((*ifNode)->data->lineNumber,
			(*current)->data->lineNumber);

	for (int i = 0; i < ASTtree->getNumberOfChildren(current); i++) {

		tmp = ASTtree->getChild(current, i);
		if (ASTtree->isValid(tmp)) {

			if ((*tmp)->data->type == "IF" || (*tmp)->data->type == "ELSE"
					|| (*tmp)->data->type == "WHILE") {
				ifRecur(tmp, ifNode);

			} else {
				linesTable->addIfLine((*ifNode)->data->lineNumber,
						(*tmp)->data->lineNumber);
			}

		}
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
				linesTable->addCallLine((*begin)->data->lineNumber,
						(*begin)->data->value);
			}
		}
		++begin;
	}

}

void DesignExtractor::setCallsRelations() {
	ASTTree * ASTtree = pkb->getASTTree();
	ProcTable * procTable = pkb->getProcTable();
	Calls* calls = pkb->getCalls();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getRoot();
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getEnd();
	int procId = -1;

	while (begin != end) {
		if (ASTtree->isValid(begin) && (*begin)->data) {
			if ((*begin)->data->type == "PROCEDURE") {
				procId = procTable->getProcId((*begin)->data->value);
			} else if (procId != -1 && (*begin)->data->type == "CALL") {
				calls->addCall(procId,
						procTable->getProcId((*begin)->data->value));
			}
		}
		++begin;
	}
}

void DesignExtractor::setProcTable() {

	ASTTree * ASTtree = pkb->getASTTree();
	ProcTable * procTable = pkb->getProcTable();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getRoot();
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getEnd();
	tree<tree_node_<ASTNode*>*>::iterator tmp;

	while (begin != end) {
		if (ASTtree->isValid(begin) && (*begin)->data->type == "PROCEDURE") {
			if (!(*begin)->data) {
				cout << "error <set procedures's>" << endl;
			} else {
				procTable->addProc(((*begin)->data->value),
						((*begin)->data->lineNumber));

				//get childs
				int procId = procTable->getProcId((*begin)->data->lineNumber);
				for (int i = 0; i < ASTtree->getNumberOfChildren(begin); i++) {

					tmp = ASTtree->getChild(begin, i);
					if (ASTtree->isValid(tmp)) {

						if ((*tmp)->data->type == "IF"
								|| (*tmp)->data->type == "ELSE"
								|| (*tmp)->data->type == "WHILE") {
							procRecur(tmp, procId);

						} else {
							procTable->addProcBodyLine(
									procId,
									(*tmp)->data->lineNumber);
						}

					}
				}

			}
		}
		++begin;
	}
}

void DesignExtractor::procRecur(tree<tree_node_<ASTNode*>*>::iterator current,int procId) {

	tree<tree_node_<ASTNode*>*>::iterator tmp;
	ASTTree * ASTtree = pkb->getASTTree();
	ProcTable * procTable = pkb->getProcTable();

	procTable->addProcBodyLine(procId,
			(*current)->data->lineNumber);

	for (int i = 0; i < ASTtree->getNumberOfChildren(current); i++) {

		tmp = ASTtree->getChild(current, i);
		if (ASTtree->isValid(tmp)) {

			if ((*tmp)->data->type == "IF" || (*tmp)->data->type == "ELSE"
					|| (*tmp)->data->type == "WHILE") {
				procRecur(tmp, procId);

			} else {
				procTable->addProcBodyLine(procId,
						(*tmp)->data->lineNumber);
			}

		}
	}
}

void DesignExtractor::setNextRelations() {
	ASTTree * ASTtree = pkb->getASTTree();
	Next * next = pkb->getNext();

	tree<tree_node_<ASTNode*>*>::iterator begin = ASTtree->getRoot();
	tree<tree_node_<ASTNode*>*>::iterator end = ASTtree->getEnd();
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	tree<tree_node_<ASTNode*>*>::iterator tmp2;

	while (begin != end) {
		if (ASTtree->isValid(begin) && (*begin)->data) {
			if ((*begin)->data->type == "ASSIGN" || (*begin)->data->type == "CALL") {
				tmp = begin.node -> next_sibling;
				if(ASTtree->isValid(tmp) && (*tmp)->data) {
					next->addNext((*begin)->data->lineNumber, (*tmp)->data->lineNumber);
				}
				else {
					tmp = begin.node -> parent;
					if(ASTtree->isValid(tmp) && (*tmp)->data) {
						if((*tmp)->data->type == "WHILE") {
							next->addNext((*begin)->data->lineNumber, (*tmp)->data->lineNumber);
						}
						else if((*tmp)->data->type == "IF" || (*tmp)->data->type == "ELSE") {
							while(true) {
								if((*tmp)->data->type == "ELSE") {
									tmp = tmp.node -> prev_sibling;
								}
								if(!(ASTtree->isValid(tmp)) || !((*tmp)->data) || (*tmp)->data->type != "IF") {
									if((*tmp)->data->type == "WHILE") {
										next->addNext((*begin)->data->lineNumber, (*tmp)->data->lineNumber);
									}
									break;
								}
								else {
									tmp2 = tmp.node -> next_sibling;
									if(ASTtree->isValid(tmp2) && (*tmp2)->data) {
										if((*tmp2)->data->type != "ELSE") {
											next->addNext((*begin)->data->lineNumber, (*tmp2)->data->lineNumber);
											break;
										}
										else {
											tmp2 = tmp.node -> next_sibling -> next_sibling;
											if(ASTtree->isValid(tmp2) && (*tmp2)->data) {
												next->addNext((*begin)->data->lineNumber, (*tmp2)->data->lineNumber);
												break;
											}
										}
									}
								}
								tmp = tmp.node -> parent;
							}
						}
					}
				}
			}
			else if ((*begin)->data->type == "WHILE") {
				tmp = begin.node -> first_child;
				if(ASTtree->isValid(tmp) && (*tmp)->data) {
					next->addNext((*begin)->data->lineNumber, (*tmp)->data->lineNumber);
				}
				tmp = begin.node -> next_sibling;
				if(ASTtree->isValid(tmp) && (*tmp)->data) {
					next->addNext((*begin)->data->lineNumber, (*tmp)->data->lineNumber);
				}
			}
			else if ((*begin)->data->type == "IF") {
				tmp = begin.node -> first_child;
				if(ASTtree->isValid(tmp) && (*tmp)->data) {
					next->addNext((*begin)->data->lineNumber, (*tmp)->data->lineNumber);
				}
				tmp2 = begin.node -> next_sibling;

				if(ASTtree->isValid(tmp2) && (*tmp2)->data && (*tmp2)->data->type == "ELSE") {
					tmp = tmp2.node -> first_child;
					if(ASTtree->isValid(tmp) && (*tmp)->data) {
						next->addNext((*begin)->data->lineNumber, (*tmp)->data->lineNumber);
					}
				}
			}
		}
		++begin;
	}
}
