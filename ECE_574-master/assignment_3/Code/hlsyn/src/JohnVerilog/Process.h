#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

#include "CircuitSignal.h"

// enumeration for Verilog comparator component outputs
enum comparatorType { GT, LT, EQ, NOT_COMP };

enum ifOrElseType { NEITHER_TYPE = 0, IF_TYPE = 1, ELSE_TYPE = 2 };

struct CircuitComp
{
    string type;
    int enumType;
    comparatorType comparatorOp;
    CircuitSignal result;
    vector<CircuitSignal> operands;
    string width;
    bool ifStatement;
    int ifStatementLevel;
    int elseStatementLevel;
    int timeframe[2];
};

struct CompLocation
{
    int ifElseLevel;
    ifOrElseType ifOrElse;
    CircuitComp compToLocate;
};

class Process
{
    std::string verilogFilename;
    int numStates;
    set<int> startTimeSet;
    set<int> endTimeSet;

public:

    int latency;

    int writeDeclarations(std::vector<CircuitSignal> &signals, std::vector<CircuitComp> &components, std::ofstream &verilogFile);
    void declareCycleCounter(std::list<string> &declarations);
    void declareStateMachine(std::list<string> &declarations, std::vector<CircuitComp> &components);
    int findNumStates(std::vector<CircuitComp> &components);
    void declareStates(std::list<string> &declarations);
    void writeStateMachineProc(std::vector<CircuitSignal> &signals, std::vector<CircuitComp> &components, std::ofstream &verilogFile);
    void resetStateMachine(std::vector<CircuitSignal> &signals, std::list<string> &stateMachine);
    void createStateMachine(std::vector<CircuitComp> &components, std::vector<CircuitSignal> &signals, std::list<string> &stateMachine);
    void createWaitState(std::list<string> &stateMachine);
    void convertComp(CircuitComp &component, std::string &opSymbol, std::string &result, std::string &op1, std::string &op2, std::string &sel);
    void createFinalState(std::vector<CircuitComp> &components, std::list<string> &stateMachine);
    void writeListToFile(std::list<string> &listToWrite, std::ofstream &outputFile);
    void setVerilogFilename(const char *vFilename);
    Process();
    ~Process();
};
