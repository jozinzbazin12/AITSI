/*
 * exceptions.h
 *
 *  Created on: 20 mar 2016
 *      Author: Grzegurz
 */

#ifndef SRC_EXCEPTIONS_H_
#define SRC_EXCEPTIONS_H_

class Exception {
public:
	int line;
	string parser;
	string msg;
	bool runtime = false;

	void print() {
		stringstream ss;
		ss << "Error occured in line " << line << ", " << parser << ", " << msg << endl;
		cout << ss.str();
	}

};

class RuntimeException: public Exception {
public:
	RuntimeException() {
		runtime = true;
	}

	RuntimeException(int line, string parser, string msg) :
			RuntimeException() {
		this->line = line;
		this->parser = parser;
		this->msg = msg;
	}
};

#endif /* SRC_EXCEPTIONS_H_ */
