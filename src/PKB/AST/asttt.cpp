// asttt.cpp : main project file.

#include <string>
#include <map>
using namespace std;

#include"tree_util.hh"
#include "ASTNode.h"
#include "ASTTree.h"
#include "NodeFactory.h"

void fillNextFollows(ASTTree* filledTree) {

	map<int, int> nextFollow;    //1 arg - line num   , 2 - its follower
	tree<tree_node_<ASTNode*>*>::iterator begin = filledTree->getRoot();
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	tree<tree_node_<ASTNode*>*>::iterator end = filledTree->getEnd();

	while (begin != end) {
		if (filledTree->isValid(begin)) {
			if (!(*begin)->data) {
				cout << "error <fillNextFollows>" << endl;
			} else {
				tmp = begin.node->next_sibling;
				if (filledTree->isValid(tmp)) {
					if ((*begin)->data->lineNumber != (*tmp)->data->lineNumber)
						nextFollow.insert(
								std::make_pair((*tmp)->data->id,
										(*begin)->data->id));
				}
			}
		}
		++begin;

	}

	std::map<int, int>::iterator it = nextFollow.begin();
	for (it = nextFollow.begin(); it != nextFollow.end(); ++it)
		std::cout << it->first << " => " << it->second << '\n';

}

void fillParents(ASTTree* filledTree) {
	map<int, int> lineParent;
	map<int, vector<int> > lineKids;

	tree<tree_node_<ASTNode*>*>::iterator begin = filledTree->getRoot();
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	tree<tree_node_<ASTNode*>*>::iterator end = filledTree->getEnd();

	while (begin != end) {
		if (filledTree->isValid(begin)) {
			if (!(*begin)->data) {
				cout << "error <fillParents>" << endl;
			} else {
				tmp = begin.node->parent;
				if (filledTree->isValid(tmp)
						&& (*tmp)->data->type != "procedure") {
					if ((*begin)->data->lineNumber
							!= (*tmp)->data->lineNumber) {
						lineParent[(*begin)->data->lineNumber] =
								(*tmp)->data->lineNumber;
						lineKids[(*tmp)->data->lineNumber].push_back(
								(*begin)->data->lineNumber);
					}

				}
			}
		}
		++begin;

	}

	for (map<int, vector<int> >::iterator iter = lineKids.begin();
			iter != lineKids.end(); ++iter) {
		vector<int> tempVec = (*iter).second;
		int key = (*iter).first;
		cout << key << " :";
		for (unsigned i = 0; i < tempVec.size(); i++) {
			cout << " " << tempVec[i];
		}
		cout << endl;
	}

}

void rek(tree<tree_node_<ASTNode*>*>::iterator it,ASTTree* filledTree){

	cout<<it.node->data->data->lineNumber<<endl;
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	tree<tree_node_<ASTNode*>*>::iterator endLoop;

	tree<tree_node_<ASTNode*>*>::iterator parent;
	tmp=filledTree->getFirstChild(it);
	parent=tmp.node->parent;
	endLoop = filledTree->getLastChild(tmp);
	if(filledTree->isValid(tmp) && filledTree->isValid(endLoop) ){
		while (tmp != endLoop){
			if(parent==it){
				cout<<(*tmp)->data->lineNumber<<" << Beginlinenum  :  "<<(*tmp)->data->type<<" << Beginlinenum  :  "<<(*endLoop)->data->lineNumber<<" << Endlinenum"<<(*endLoop)->data->type<<" << Beginlinenum  :  "<<endl;
							rek(tmp,filledTree);
			}

					++tmp;
		}
	}else
		return;

}

void fillLoops(ASTTree* filledTree) {
	tree<tree_node_<ASTNode*>*>::iterator begin = filledTree->getRoot();
	tree<tree_node_<ASTNode*>*>::iterator end = filledTree->getEnd();
	tree<tree_node_<ASTNode*>*>::iterator stmtlst;
	tree<tree_node_<ASTNode*>*>::iterator sib;

	while (begin != end) {
		if (filledTree->isValid(begin) && (*begin)->data->type == "LOOP") {
			if (!(*begin)->data) {
				cout << "error <fillLoops>" << endl;
			} else {
				stmtlst = begin.node->first_child;  //stmtlst    //
				sib = filledTree->getFirstChild(stmtlst); //first child of stmtlist   //7

				cout << (*sib)->data->lineNumber <<"  :  ++"<<endl;
				for(int i=0;i<filledTree->getNumberOfChildren(stmtlst);i++){
					sib = sib.node->next_sibling;
					if(filledTree->isValid(sib))
						cout << (*sib)->data->lineNumber <<"  :  ++"<<endl;
				}

			}
		}

		++begin;
	}

}

void fillAssigns(ASTTree* filledTree) {
	tree<tree_node_<ASTNode*>*>::iterator begin = filledTree->getRoot();
		tree<tree_node_<ASTNode*>*>::iterator tmp;
		tree<tree_node_<ASTNode*>*>::iterator end = filledTree->getEnd();

		while (begin != end) {
			if (filledTree->isValid(begin) && (*begin)->data->type == "ASSIGN") {
				cout<<(*begin)->data->lineNumber <<endl;
			}
			++begin;

		}

}


map<int, int> fillPrevFollows() {

}

int main4() {

	tree<tree_node_<ASTNode*>*>::iterator iter;
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	tree<tree_node_<ASTNode*>*>::iterator test;
	tree<tree_node_<ASTNode*>*>::iterator loop;
	tree<tree_node_<ASTNode*>*>::iterator loopAssign2;

	ASTTree* tree = new ASTTree();
	ASTNode* node;
	tree_node_<ASTNode*>* treeNode;

	node = NodeFactory::CreateNode("procedure", 1, "22");
	treeNode = new tree_node_<ASTNode*>(node);

	iter = tree->appendRoot(treeNode);

	node = NodeFactory::CreateNode("STM_LIST", 2, "");
	treeNode = new tree_node_<ASTNode*>(node);

	iter = tree->appendChild(iter, treeNode);
	tmp = iter;

	loop = iter;

	node = NodeFactory::CreateNode("LOOP", 5, "");
	treeNode = new tree_node_<ASTNode*>(node);

	loop = tree->appendChild(loop, treeNode);

	node = NodeFactory::CreateNode("STM_LIST", 5, "");
	treeNode = new tree_node_<ASTNode*>(node);

	loop = tree->appendChild(loop, treeNode);

	node = NodeFactory::CreateNode("ASSIGN", 7, "");
	treeNode = new tree_node_<ASTNode*>(node);
	loopAssign2 = tree->appendChild(loop, treeNode);

	node = NodeFactory::CreateNode("ASSIGN", 6, "");
	treeNode = new tree_node_<ASTNode*>(node);
	loop = tree->appendChild(loop, treeNode);




	node = NodeFactory::CreateNode("Var", 99, "");
	treeNode = new tree_node_<ASTNode*>(node);
	loop = tree->appendChild(loop, treeNode);







	node = NodeFactory::CreateNode("ASSIGN", 3, "");
	treeNode = new tree_node_<ASTNode*>(node);

	iter = tree->appendChild(iter, treeNode);

	node = NodeFactory::CreateNode("ASSIGN", 4, "");
	treeNode = new tree_node_<ASTNode*>(node);

	tmp = tree->appendChild(tmp, treeNode);

	node = NodeFactory::CreateNode("Variable", 3, "");
	treeNode = new tree_node_<ASTNode*>(node);

	iter = tree->appendChild(iter, treeNode);

	node = NodeFactory::CreateNode("Variable", 4, "");
	treeNode = new tree_node_<ASTNode*>(node);

	tmp = tree->appendChild(tmp, treeNode);

	node = NodeFactory::CreateNode("Expr", 3, "");
	treeNode = new tree_node_<ASTNode*>(node);

	iter = tree->appendSibling(iter, treeNode);

	node = NodeFactory::CreateNode("Expr", 4, "");
	treeNode = new tree_node_<ASTNode*>(node);

	tmp = tree->appendSibling(tmp, treeNode);

	node = NodeFactory::CreateNode("Var", 3, "");
	treeNode = new tree_node_<ASTNode*>(node);

	iter = tree->appendChild(iter, treeNode);

	node = NodeFactory::CreateNode("Var", 4, "");
	treeNode = new tree_node_<ASTNode*>(node);
	tmp = tree->appendChild(tmp, treeNode);

	node = NodeFactory::CreateNode("Var", 3, "");
	treeNode = new tree_node_<ASTNode*>(node);

	iter = tree->appendSibling(iter, treeNode);

	node = NodeFactory::CreateNode("Var", 4, "");
	treeNode = new tree_node_<ASTNode*>(node);
	tmp = tree->appendSibling(tmp, treeNode);

	tree->printTree();
	//fillNextFollows(tree);
	//fillParents(tree);
	//fillLoops(tree);
	fillAssigns(tree);
	return 0;
}
