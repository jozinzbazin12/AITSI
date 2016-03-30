/*
 * nodeUtil.h
 *
 *  Created on: 29 mar 2016
 *      Author: Grzegurz
 */

#ifndef SRC_PARSER_NODEUTIL_H_
#define SRC_PARSER_NODEUTIL_H_

class NodeUtil {
private:
	NodeUtil() {
	}

	static tree_node_<ASTNode*>* createTreeNode(ASTNode* node) {
		return new tree_node_<ASTNode*>(node);
	}

	static ASTTree* createTree(ASTNode* node) {
		ASTTree* tree = new ASTTree();
		tree_node_<ASTNode*>* outer = createTreeNode(node);
		tree->appendRoot(outer);
		return tree;
	}

public:
	static ASTTree* createAssignmentNode(int lineNumber) {
		ASTNode* node = new ASTNode(0, StatementType::ASSIGN, lineNumber);
		return createTree(node);
	}

	static ASTTree* createProcedureNode(string name, int lineNumber) {
		ASTNode* node = new NewLevelNode(0, StatementType::PROCEDURE, lineNumber, name);
		return createTree(node);
	}

	static tree_node_<ASTNode*>* createProgramNode() {
		ASTNode* node = new NewLevelNode(0, StatementType::PROGRAM, 0);
		return createTreeNode(node);
	}

	static ASTTree* createOperandNode(string name, int lineNumber) {
		ASTNode* node = new ASTNode(0, "OPERAND", lineNumber, name);
		return createTree(node);
	}

	static ASTTree* createExpressionNode(string name, int lineNumber) {
		ASTNode* node = new ASTNode(0, "EXPRESSION", lineNumber, name);
		return createTree(node);
	}

	static tree<tree_node_<ASTNode*>*>::iterator appendChild(ASTTree* t, tree<tree_node_<ASTNode*>*>::iterator parent,
			tree_node_<ASTNode*>* child) {
		return t->appendChild(parent, child);
	}

	static tree<tree_node_<ASTNode*>*>::iterator appendChild(ASTTree* t, tree<tree_node_<ASTNode*>*>::iterator parent, ASTTree* child) {
		return t->appendSubTree(parent, child);
	}

	static void appendChild(ASTTree* t, ASTTree* child) {
		t->appendSubTree(t->getRoot(), child);
	}
};

#endif /* SRC_PARSER_NODEUTIL_H_ */