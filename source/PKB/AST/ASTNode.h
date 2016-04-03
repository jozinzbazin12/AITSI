#ifndef SRC_PKB_AST_ASTNode_H_
#define SRC_PKB_AST_ASTNode_H_
#include <string>
class ASTNode {
public:
	std::string value = "";
	std::string type = "";
	int id;
	int lineNumber;

	//parser
	bool last = false;
	bool newLevel = false;
	ASTNode() {

	}

	ASTNode(int id, std::string type, int lineNumber, std::string value = "") {  //for nodes like varable or call
		this->value = value;
		this->type = type;
		this->lineNumber = lineNumber;
		this->id = id;
	}
};

class NewLevelNode: public ASTNode {
public:
	NewLevelNode(int id, std::string type, int lineNumber, std::string value = "") :
			ASTNode(id, type, lineNumber, value) {
		newLevel = true;
	}
};

#endif
