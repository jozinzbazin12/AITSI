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

	void print() {
		stringstream ss;
		ss << "Wystapil blad w linii " << line << ", " << parser << ", " << msg<<endl;
		cout << ss.str();
	}

};

class RuntimeException: public Exception {
};

#endif /* SRC_EXCEPTIONS_H_ */
