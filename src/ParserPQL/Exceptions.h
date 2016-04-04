/*
 * Exceptions.h
 *
 *  Created on: 3 kwi 2016
 *      Author: Popek
 */

#ifndef SRC_PARSERPQL_EXCEPTIONS_H_
#define SRC_PARSERPQL_EXCEPTIONS_H_

#include <iostream>

using namespace std;

class Exceptions
{
public:
	void throwException() {
		cout << "Query is invalid.";
		exit(1);
	}

	void throwSplitException() {
		cout << "Query was split incorrect. Some parts of query is invalid.";
		exit(1);
	}

	void throwUnexpectedTypeOfClause() {
		cout << "Query has unexpected clause in such that part.";
		exit(1);
	}

	void throwToMuchArguments() {
		cout << "One of clause has more then two arguments.";
		exit(1);
	}

	void throwInvalidNumberOfArguments()
	{
		cout << "Clause has invalid number of arguments.";
		exit(1);
	}

	void throwInvalidTypeOfArguments()
	{
		cout << "Clause has invalid type of arguments.";
		exit(1);
	}
};

#endif /* SRC_PARSERPQL_EXCEPTIONS_H_ */
