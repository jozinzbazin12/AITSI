/*
 * types.h
 *
 *  Created on: 1 mar 2016
 *      Author: lulewiczg
 */

#ifndef TYPES_H_
#define TYPES_H_

class StatementType {
public:
	static string PROGRAM;
	static string PROCEDURE;
	static string ASSIGN;
	static string EXPR;
	static string STM_LIST;
	static string LOOP;
	//, STM_LIST, STMT, CALL, WHILE, IF, EXPR, TERM, FACTOR, VAR_NAME, PROC_NAME, CONST_NAME
};
string StatementType::PROGRAM = "PROGRAM";
string StatementType::PROCEDURE = "procedure";
string StatementType::ASSIGN = "=";
string StatementType::EXPR = "EXPR";
string StatementType::STM_LIST = "STM_LIST";
string StatementType::LOOP = "LOOP";
#endif /* TYPES_H_ */
