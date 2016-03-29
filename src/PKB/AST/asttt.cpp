// asttt.cpp : main project file.


#include <string>
using namespace std;

#include"tree_util.hh"
#include "ASTNode.h"
#include "ASTTree.h"
int main()
{

	tree<tree_node_<ASTNode>>::iterator iter;
	tree<tree_node_<ASTNode>>::iterator tmp;
	ASTTree* tree;
	tree = tree->getInstance();
	ASTNode* node;
	tree_node_<ASTNode>* treeNode;

	node = new ASTNode("Procedure", "PROC", 1);
	treeNode = new tree_node_<ASTNode>(*node);
	
	iter = tree->appendRoot(*treeNode);

	node = new ASTNode("STMTLST", "STMTLST", 2);
	treeNode = new tree_node_<ASTNode>(*node);
		
	iter = tree->appendChild(iter, *treeNode);
	tmp = iter;


	node = new ASTNode("Assing", "Assing", 3);
	treeNode = new tree_node_<ASTNode>(*node);

	iter = tree->appendChild(iter, *treeNode);

	tmp = tree->appendChild(tmp, *treeNode);

	node = new ASTNode("X", "Variable", 5);
	treeNode = new tree_node_<ASTNode>(*node);

	iter = tree->appendChild(iter, *treeNode);
	tmp = tree->appendChild(tmp, *treeNode);


	node = new ASTNode("+", "Expression", 7);
	treeNode = new tree_node_<ASTNode>(*node);

	iter = tree->appendSibling(iter, *treeNode);
	tmp = tree->appendSibling(tmp, *treeNode);

	node = new ASTNode("6", "Variable", 7);
	treeNode = new tree_node_<ASTNode>(*node);

	iter = tree->appendChild(iter, *treeNode);
	tmp = tree->appendChild(tmp, *treeNode);

	node = new ASTNode("3", "Variable", 7);
	treeNode = new tree_node_<ASTNode>(*node);

	iter = tree->appendSibling(iter, *treeNode);
	tmp = tree->appendSibling(tmp, *treeNode);

	tree->printTree();

	int a;
	cin >>a;
    return 0;
}
