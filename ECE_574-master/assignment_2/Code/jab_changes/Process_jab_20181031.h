#include <fstream>
#include <string>
#include <vector>

#include "CircuitSignal.h"

// jab added forward declaration of CircuitComp 10/31/2018
struct CircuitComp;

class Process
{
	public:
	
	int parseNetlist(std::ifstream &netList, std::ofstream &verilogFile);
	bool parseVariable(std::string variable, std::vector<std::string> &variables);
	std::string parseDataType(std::string &dataType);
	void parseSignals(std::ifstream &netList, std::vector<CircuitSignal> &circuitVector, std::string &line);
    // jab added write function declarations 10/31/2018
    int writeDeclarations(std::vector<CircuitSignal> &signals, ofstream &verilogFile);
    int writeInstantiations(std::vector<CircuitComp> &components, ofstream &verilogFile);
	Process();
	~Process();
	
};