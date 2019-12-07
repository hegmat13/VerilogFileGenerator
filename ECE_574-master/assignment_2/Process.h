#include <fstream>
#include <string>
#include <vector>

#include "CircuitSignal.h"

// enumeration for Verilog comparator component outputs
enum {GT, LT, EQ};

struct CircuitComp
{
	string type;
	CircuitSignal result;
	vector<CircuitSignal> operands;
	double delay;
	string width;
};

class Process
{
    std::string verilogFilename;

    public:
	
	int parseNetlist(std::ifstream &netList, std::ofstream &verilogFile);
	bool parseVariable(std::string variable, std::vector<std::string> &variables);
	std::string parseDataType(std::string &dataType);
	void parseSignals(std::ifstream &netList, std::vector<CircuitSignal> &circuitVector, std::string &line);
	int parseOneOperand(std::vector<CircuitSignal> &circuitVector, std::vector<CircuitComp> &circuitCompVector, std::stringstream &lineStream, std::string type);
	int parseTwoOperands(std::vector<CircuitSignal> &circuitVector, std::vector<CircuitComp> &circuitCompVector, std::stringstream &lineStream, std::string type);
    int parseMux(std::vector<CircuitSignal> &circuitVector, std::vector<CircuitComp> &circuitCompVector, std::stringstream &lineStream, std::string type);
    int parseComp(std::vector<CircuitSignal> &circuitVector, std::vector<CircuitComp> &circuitCompVector, std::stringstream &lineStream, std::string type);
    int writeDeclarations(std::vector<CircuitSignal> &signals, std::ofstream &verilogFile);
    int writeInstantiations(std::vector<CircuitComp> &components, std::ofstream &verilogFile);
    void setVerilogFilename(const char *vFilename);
    void getOperands(std::vector<CircuitComp> &components, std::vector<int>::size_type &i, std::string &result, std::string &op1);
    void getOperands(std::vector<CircuitComp> &components, std::vector<int>::size_type &i, std::string &result, std::string &op1, std::string &op2);
    void getOperands(std::vector<CircuitComp> &components, std::vector<int>::size_type &i, std::string &result, std::string &op1, std::string &op2, std::string &sel);
    void getOperands(std::vector<CircuitComp> &components, std::vector<int>::size_type &i, std::string &result, std::string &op1, std::string &op2, int &resultType);

	//cirical path functions
	double addDependentDelaysOneOP(std::vector<CircuitComp> &circuitCompVector, std::string op1);
	double addDependentDelaysTwoOP(std::vector<CircuitComp> &circuitCompVector, std::string op1, std::string op2);
	double CalculateCiritcalPath(vector<CircuitComp> &circuitCompVector);
	
	Process();
	~Process();
	
};