// asttt.cpp : main project file.


#include <string>
using namespace std;

#include"tree_util.hh"
#include "ASTNode.h"
#include "ASTTree.h"
int main2()
{

	tree<tree_node_<ASTNode*>*>::iterator iter;
	tree<tree_node_<ASTNode*>*>::iterator tmp;
	ASTTree* tree=new ASTTree();
	ASTNode* node;
	tree_node_<ASTNode*>* treeNode;



	node = new ASTNode(1,"PROCEDURE",1);
	treeNode = new tree_node_<ASTNode*>(node);
	
	iter = tree->appendRoot(treeNode);

	node = new ASTNode(2,"STM_LIST",2);
	treeNode = new tree_node_<ASTNode*>(node);
		
	iter = tree->appendChild(iter, treeNode);
	tmp = iter;


	node = new ASTNode(3, "ASSIGN", 3);
	treeNode = new tree_node_<ASTNode*>(node);

	iter = tree->appendChild(iter, treeNode);

	tmp = tree->appendChild(tmp, treeNode);

	node = new ASTNode(4, "Variable", 5);
	treeNode = new tree_node_<ASTNode*>(node);

	iter = tree->appendChild(iter, treeNode);
	tmp = tree->appendChild(tmp, treeNode);


	node = new ASTNode(4, "Expression", 7);
	treeNode = new tree_node_<ASTNode*>(node);

	iter = tree->appendSibling(iter, treeNode);
	tmp = tree->appendSibling(tmp, treeNode);

	node = new ASTNode(4, "Variable", 7);
	treeNode = new tree_node_<ASTNode*>(node);

	iter = tree->appendChild(iter, treeNode);
	tmp = tree->appendChild(tmp, treeNode);

	node = new ASTNode(4, "Variable", 7);
	treeNode = new tree_node_<ASTNode*>(node);

	iter = tree->appendSibling(iter, treeNode);
	tmp = tree->appendSibling(tmp, treeNode);

	tree->printTree();

    return 0;
}
