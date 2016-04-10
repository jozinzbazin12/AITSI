/*
 * types.h
 *
 *  Created on: 1 mar 2016
 *      Author: lulewiczg
 */

#ifndef TYPES_H_
#define TYPES_H_

class NodeName {
public:
	static string PROGRAM;
	static string PROCEDURE;
	static string ASSIGN;
	static string EXPR;
	static string STM_LIST;
	static string WHILE;
	static string CALL;
	static string OPERAND;
	//, STM_LIST, STMT, CALL, WHILE, IF, EXPR, TERM, FACTOR, VAR_NAME, PROC_NAME, CONST_NAME
};
string NodeName::OPERAND = "OPERAND";
string NodeName::PROGRAM = "PROGRAM";
string NodeName::PROCEDURE = "PROCEDURE";
string NodeName::ASSIGN = "ASSIGN";
string NodeName::EXPR = "EXPR";
string NodeName::STM_LIST = "STM_LIST";
string NodeName::WHILE = "WHILE";
string NodeName::CALL = "CALL";

class Keywords {
public:
	static string PROGRAM;
	static string PROCEDURE;
	static string ASSIGN;
	static string EXPR;
	static string WHILE;
	static string CALL;
	static string OPERAND;
	static string PARENTSISES;
};
string Keywords::PROGRAM = "program";
string Keywords::PARENTSISES = "PARENTISIES";
string Keywords::PROCEDURE = "procedure";
string Keywords::ASSIGN = "=";
string Keywords::OPERAND = "op";
string Keywords::EXPR = "expr";
string Keywords::WHILE = "while";
string Keywords::CALL = "call";
#endif /* TYPES_H_ */
