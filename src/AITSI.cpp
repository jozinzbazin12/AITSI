#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
#include "types.h"
#include "syntax.h"
#include "parser.h"

int main(int argc, char** args) {
	if (argc <= 1) {
		cout << "No arguments, to see usage help, use \"help\" parameter";
		return 0;
	}
	//W args[0] jest sciezka do exe.
	string action = string(args[1]);
	if (action == "help") {
		cout << "Usage...\n";
	} else if (action == "file") {
		Parser parser(args[2]);
		if (!parser.fileExists) {
			cout << "File not found\n";
		} else {
			bool result = parser.parse();
			if (result) {
				cout << "OK";
			} else {
				cout << "b³ont\n";
				cout << "b³ont ortograficzny\n";
			}
		}
	}
	return 0;
}
