#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <set>

#include "CircuitSignal.h"

#ifndef PROCESS_H
#define PROCESS_H

// enumeration for Verilog comparator component outputs
enum comparatorType{ GT, LT, EQ, NOT_COMP };

enum ifOrElseType { NEITHER_TYPE = 0, IF_TYPE = 1, ELSE_TYPE = 2 };

struct CircuitComp
{
	string type;
	int enumType;
	comparatorType comparatorOp;
	CircuitSignal result;
	vector<CircuitSignal> operands;
	double delay;
	int cycleTime;
	string width;
	bool ifStatement;
	int ifStatementLevel;
	int elseStatementLevel;
	int timeframe[2];
	int range;
	vector<double> probability;
	vector<int> predecessor;
	vector<int> successor;
	bool visited;
	bool ignoreOp;

	CircuitComp()
	{
		visited = false;
		ignoreOp = false;
		ifStatement = false;
		ifStatementLevel = 0;
		elseStatementLevel = 0;
	}
};

struct CompLocation
{
    int ifElseLevel;
    ifOrElseType ifOrElse;
    CircuitComp compToLocate;
};

class Process
{
	int numStates;
    std::string verilogFilename;
	bool ifStatement;
	int ifLevel;
	int elseLevel;
	set<int> startTimeSet;
	set<int> endTimeSet;
	int m_endTime;

    public:
	
	int latency;
	
	int parseNetlist(std::ifstream &netList, std::ofstream &verilogFile);
	bool parseVariable(std::string variable, std::vector<std::string> &variables);
	std::string parseDataType(std::string &dataType);
	void parseSignals(std::ifstream &netList, std::vector<CircuitSignal> &circuitVector, std::string &line);
	int parseOneOperand(std::vector<CircuitSignal> &circuitVector, std::vector<CircuitComp> &circuitCompVector, std::stringstream &lineStream, std::string type);
	int parseTwoOperands(std::vector<CircuitSignal> &circuitVector, std::vector<CircuitComp> &circuitCompVector, std::stringstream &lineStream, std::string type);
	int parseMux(std::vector<CircuitSignal> &circuitVector, std::vector<CircuitComp> &circuitCompVector, std::stringstream &lineStream, std::string type);
	int parseComp(std::vector<CircuitSignal> &circuitVector, std::vector<CircuitComp> &circuitCompVector, std::stringstream &lineStream, std::string type);
	int writeDeclarations(std::vector<CircuitSignal> &signals, std::vector<CircuitComp> &components, std::ofstream &verilogFile);
	void declareCycleCounter(std::list<string> &declarations);
    void declareStateMachine(std::list<string> &declarations, std::vector<CircuitComp> &components);
    int findNumStates(std::vector<CircuitComp> &components);
	void declareStates(list<string> &declarations, vector<CircuitComp> circuitCompVector);
    void writeStateMachineProc(std::vector<CircuitSignal> &signals, std::vector<CircuitComp> &components, std::ofstream &verilogFile);
    void resetStateMachine(std::vector<CircuitSignal> &signals, std::list<string> &stateMachine);
    void createStateMachine(std::vector<CircuitComp> &components, std::vector<CircuitSignal> &signals, std::list<string> &stateMachine);
    void createWaitState(std::list<string> &stateMachine);
    void convertComp(CircuitComp &component, std::string &opSymbol, std::string &result, std::string &op1, std::string &op2, std::string &sel);
    void createFinalState(std::vector<CircuitComp> &components, std::list<string> &stateMachine);
    void writeListToFile(std::list<string> &listToWrite, std::ofstream &outputFile);
    void setVerilogFilename(const char *vFilename);
	int getCycleTime(std::string dataType);
	int stringToEnumType(std::string datatype);
	
	//critical path functions
	double addDependentDelaysOneOP(std::vector<CircuitComp> &circuitCompVector, std::string op1);
	double addDependentDelaysTwoOP(std::vector<CircuitComp> &circuitCompVector, std::string op1, std::string op2);
	double addDependentDelaysMux(std::vector<CircuitComp> &circuitCompVector, std::string op1, std::string op2, std::string op3);
	double CalculateCriticalPath(std::vector<CircuitComp> &circuitCompVector);

	Process();
	~Process();
	
};

#endif
