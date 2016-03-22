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
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

void ltrim(string &s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
}

void rtrim(string &s) {
	s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
}

void trim(string &s) {
	ltrim(s);
	rtrim(s);
}

void removeWhitespaces(string &str) {
	str.erase(remove_if(str.begin(), str.end(), ptr_fun<int, int>(isspace)), str.end());
}

#include "exceptions.h"
#include "types.h"
#include "tree/tree_nodes.h"
#include "parser/syntax.h"
#include "parser/parser.h"

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
	} else {
		cout << "Invalid arguments";
	}
	return 0;
}
