/*
* Exceptions.h
*
*  Created on: 3 kwi 2016
*      Author: Popek
*/

#ifndef SRC_PARSERPQL_EXCEPTIONS_H_
#define SRC_PARSERPQL_EXCEPTIONS_H_

#include <iostream>
#include <exception>

using namespace std;

class Exceptions
{
public:
	void throwException() {
		throw "Query is invalid.";
	}

	void throwSplitException() {
		throw "Query was split incorrect. Some parts of query is invalid.";
	}

	void throwUnexpectedTypeOfClause() {
		throw "Query has unexpected clause in such that part.";
	}

	void throwToMuchArguments() {
		throw "One of clause has more then two arguments.";
	}

	void throwInvalidNumberOfArguments()
	{
		throw "Clause has invalid number of arguments.";
	}

	void throwInvalidTypeOfArguments()
	{
		throw "Clause has invalid type of arguments.";
	}

	void throwInvalidWithStatement()
	{
		throw "Clause has invalid with part.";
	}

	void throwInvalidWithStatementAttributes()
	{
		throw "Clause with has invalid equals attributes (e.g. string = integer, etc.).";
	}

	void throwUnexpectedSelectPart()
	{
		throw "Clause has undeclared select part.";
	}
};

#endif /* SRC_PARSERPQL_EXCEPTIONS_H_ */
