#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <istream>
#include <algorithm>
#include <map>
#include <iterator>
#include <stack>

using namespace std;

string toLower(string str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}
#include "exceptions.h"
#include "types.h"
#include "tree/tree_nodes.h"
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
		Parser parser(args[2], true);
		if (!parser.fileExists) {
			cout << "File not found\n";
		} else {
			try {
				parser.parse();
				cout << "OK";
			} catch (RuntimeException &ex) {
				ex.print();
				cout << "FAIL";
			} catch (Exception &ex) {
				ex.print();
				cout << "FAIL";
			}
		}
	}
	return 0;
}
