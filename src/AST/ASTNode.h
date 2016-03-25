#ifndef ASTNode_H_
#define ASTNode_H_
#include <string>
class ASTNode
{
public:
	std::string value="";
	std::string type="";
	int lineNumber;
	ASTNode() {

	}
	ASTNode(std::string type,int lineNumber) {    //for nodes like stmtLst or assign
		this->type = type;
		this->lineNumber = lineNumber;
	}
	ASTNode(std::string value, std::string type, int lineNumber) {  //for nodes like varable or call
		this->value = value;
		this->type = type;
		this->lineNumber = lineNumber;
	}
};
#endif
