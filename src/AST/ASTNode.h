#ifndef ASTNode_H_
#define ASTNode_H_

using namespace std;

class ASTNode
{
public:
	string value;
	StatementType type;
	int lineNumber;
	ASTNode(StatementType type,int lineNumber) {    //for nodes like stmtLst or assign
		this->type = type;
		this->lineNumber = lineNumber;
	}
	ASTNode(string value, StatementType type, int lineNumber) {  //for nodes like varable or call
		this->value = value;
		this->type = type;
		this->lineNumber = lineNumber;
	}
};
#endif
