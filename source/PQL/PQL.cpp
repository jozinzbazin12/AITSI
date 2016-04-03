/*
 * PQL.cpp
 *
 *  Created on: 29 mar 2016
 *      Author: Popek
 */


#include "PQL.h"
#include "tree_util.hh"
#include "PQLNode.h"
#include "PQLTree.h"
#include "../QueryPreProcessor.h"

using namespace std;

PQL::PQL() {
	// TODO Auto-generated constructor stub
}

PQL::~PQL() {
	// TODO Auto-generated destructor stub
}

/*int main(int argc, char** args) {

	// DRZEWO PQL
	tree<tree_node_<PQLNode>>::iterator iter;
	PQLTree* tree;
	tree = tree->getInstance();
	PQLNode* node;
	tree_node_<PQLNode>* treeNode;

	// assign a; select a such that parent(a,2) and follows*(_,a);

	node = new QueryNode();
	treeNode = new tree_node_<PQLNode>(*node);
	iter = tree->appendRoot(*treeNode);

	node = new ResultMainNode();
	treeNode = new tree_node_<PQLNode>(*node);
	iter = tree->appendChild(iter, *treeNode);

	node = new ResultNode(new Field("assign", "a", false, false, false));
	treeNode = new tree_node_<PQLNode>(*node);
	iter = tree->appendChild(iter, *treeNode);

	iter = tree->getRoot();

	node = new SuchMainNode();
	treeNode = new tree_node_<PQLNode>(*node);
	iter = tree->appendChild(iter, *treeNode);

	node = new SuchNode("parent", new Field("assign", "a", false, false, false), new Field("constant", "2", false, false, false), false);
	treeNode = new tree_node_<PQLNode>(*node);
	iter = tree->appendChild(iter, *treeNode);

	node = new SuchNode("follows", new Field("all", "_", false, false, false), new Field("assign", "a", false, false, false), true);
	treeNode = new tree_node_<PQLNode>(*node);
	iter = tree->appendSibling(iter, *treeNode);

	tree->printTree();
	


	string a = "assign a, a2; prog_line p; select a, p.procname such that parent(a,2) and follows*(\"ala\",a2) such that uses(_,p) such that modifies*(2,p);";

	cout << a << endl;
	QueryPreProcessor* que = new QueryPreProcessor();
	que->parseQuery(a);

	que->getTree()->printTree();

	return 0;
}*/
