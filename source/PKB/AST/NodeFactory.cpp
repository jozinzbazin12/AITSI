/*
 * NodeFactory.cpp
 *
 *  Created on: 30 mar 2016
 *      Author: Mateusz
 */

#include "NodeFactory.h"

NodeFactory::NodeFactory() {
	// TODO Auto-generated constructor stub

}

int NodeFactory::counter = 0;

NodeFactory::~NodeFactory() {
	// TODO Auto-generated destructor stub
}
ASTNode* NodeFactory::CreateNode(std::string type, int lineNumber, std::string value){
	return new ASTNode(++counter,type,lineNumber,value);
}

NewLevelNode* NodeFactory:: CreateNewLeveLNode(std::string type, int lineNumber, std::string value ){
	return new NewLevelNode(++counter,type,lineNumber,value);
}
