#include "TestWrapper.h"
#include "AITSI.cpp"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

PKB* pkb;
DesignExtractor* designExtractor;
Validator* v;
//QueryPreProcessor* que;
PQL* pql;
QueryEvaluator* queEva;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
	initSyntax();
	pkb = new PKB();
	designExtractor = new DesignExtractor();
	v = new Validator();
	//que = new QueryPreProcessor();
	pql = new PQL();
	queEva = new QueryEvaluator();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// call your parser to do the parsing
  // ...rest of your code...
	Parser parser(filename);
	if (!parser.fileExists) {
		cout << "File not found\n";
	}
	else {
		try {
			parser.parse();
			pkb->setASTTree(parser.root);
			designExtractor->start();
		}
		catch (RuntimeException &ex) {
			ex.print();
		}
		catch (Exception &ex) {
			ex.print();
		}
	}
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...
  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
	QueryPreProcessor* que = new QueryPreProcessor();
	que->parseQuery(query);
	PQLTree* tree = que->getTree();
	vector<string> resultsTmp = queEva->getResult(tree);
	copy(resultsTmp.begin(), resultsTmp.end(), back_inserter(results));
	delete que;
	delete tree;
}
