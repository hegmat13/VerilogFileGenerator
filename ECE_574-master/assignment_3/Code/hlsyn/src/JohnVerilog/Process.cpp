#include "dpgen.h"
#include "Process.h"

#include <iostream>
#include <sstream>
#include <iterator>
#include <cmath>

using namespace std;

Process::Process()
{
}

Process::~Process()
{
}

int Process::writeDeclarations(vector<CircuitSignal> &signals, vector<CircuitComp> &components, ofstream &verilogFile)
{
    int status = -1;
    int temp;
    list<string> declarations;
    stringstream lineStream;
    string lineOut;
    string moduleName;
    string signalWidth;
    string regType;

    // Verilog module name is the same as the filename without the extension
    string::size_type pos = verilogFilename.find(".v");
    if (pos == string::npos)
    {
        return FAIL_FILE_TYPE;
    }
    moduleName = verilogFilename.erase(pos, verilogFilename.length());

    // First line of Verilog file declares the time scale
    lineStream << "`timescale 1ns / 1ns";
    lineOut = lineStream.str();
    declarations.push_back(lineOut);
    lineStream.str("");

    // Next line is module declaration with ports
    lineStream << "module " << moduleName << "(Clk, Rst, Start, Done";
    for (vector<CircuitSignal>::iterator it = signals.begin(); it != signals.end(); ++it)
    {
        if ((it->getCircuitSignalType() == "input") || (it->getCircuitSignalType() == "output"))
        {
            lineStream << ", " << it->getCircuitSignalName();
        }
    }
    lineStream << ");";
    lineOut = lineStream.str();
    declarations.push_back(lineOut);
    lineStream.str("");

    // Inputs and outputs follow the declaration
    lineStream << "\t" << "input Clk, Rst, Start;";
    lineOut = lineStream.str();
    declarations.push_back(lineOut);
    lineStream.str("");

    lineStream << "\t" << "output reg Done;";
    lineOut = lineStream.str();
    declarations.push_back(lineOut);
    lineStream.str("");

    for (vector<CircuitSignal>::iterator it = signals.begin(); it != signals.end(); ++it)
    {
        if (it->getCircuitSignalType() == "input")
        {
            lineStream << "\tinput ";
        }
        else if (it->getCircuitSignalType() == "output")
        {
            lineStream << "\toutput reg ";
        }
        else
        {
            lineStream << "\treg ";
        }

        if (it->getCircuitSignalDataType() == "Int")
        {
            lineStream << "signed ";
        }

        temp = stoi(it->getCircuitSignalWidth(), nullptr, 10);
        if (temp != 1)
        {
            signalWidth = to_string(--temp);
            lineStream << "[" << signalWidth << " : 0] ";
        }

        lineStream << it->getCircuitSignalName() << ";";
        lineOut = lineStream.str();
        declarations.push_back(lineOut);
        lineStream.str("");
    }

    // build cycle counter declaration
    declareCycleCounter(declarations);

    // build state machine declaration
    declareStateMachine(declarations, components);

    // place all declarations in the Verilog file
    writeListToFile(declarations, verilogFile);

    // add empty line for readability
    verilogFile << '\n';

    status = SUCCESS;
    return status;
}

void Process::writeListToFile(list<string> &listToWrite, ofstream &outputFile)
{
    for (list<string>::iterator it = listToWrite.begin(); it != listToWrite.end(); ++it)
    {
        outputFile << *it << '\n';
    }
}

void Process::declareCycleCounter(list<string> &declarations)
{
    stringstream lineStream;
    string lineOut;
    int vecSize = static_cast<int>(ceil(log2(static_cast<double>(latency))));
    int msb = vecSize - 1;

    lineStream << "\treg [" << msb << ":0] CycleCnt;";
    lineOut = lineStream.str();
    declarations.push_back(lineOut);
    lineStream.str("");
}

void Process::declareStateMachine(list<string> &declarations, vector<CircuitComp> &components)
{
    int vecSize;
    int msb;
    stringstream lineStream;
    string lineOut;

    // find out how many states are needed
    numStates = findNumStates(components);
    vecSize = static_cast<int>(ceil(log2(static_cast<double>(numStates))));
    msb = vecSize - 1;
    
    // define a parameter to enumerate the states
    declareStates(declarations);

    // declare the state machine
    lineStream << "\treg [" << msb << ":0] State;";
    lineOut = lineStream.str();
    declarations.push_back(lineOut);
    lineStream.str("");
}

int Process::findNumStates(vector<CircuitComp> &components)
{
    bool foundEntry = false;

    // The number of states needed is equal to the number of start times
    // in top level and if statements (outside of else statements)
    // plus the number of start times in else statements and their internal if
    // statements
    for (vector<CircuitComp>::iterator it = components.begin(); it != components.end(); ++it)
    {
        for (set<int>::iterator it2 = startTimeSet.begin(); (it2 != startTimeSet.end() && !foundEntry); ++it2)
        {
            if (it->timeframe[0] == *it2)
            {
                foundEntry = true;
            }
        }

        if (!foundEntry)
            startTimeSet.insert(it->timeframe[0]);

        foundEntry = false;
    }

    //cout << "start time set size: " << startTimeSet.size() << endl;
    return static_cast<int>(startTimeSet.size());
}

void Process::declareStates(list<string> &declarations)
{
    int vecSize = static_cast<int>(ceil(log2(static_cast<double>(startTimeSet.size() + 2))));
    int msb = vecSize - 1;
    int stateNum = 0;
    stringstream lineStream;
    string lineOut;

    // Declare parameter as a vector
    lineStream << "\tparameter [" << msb << ":0]";
    lineOut = lineStream.str();
    declarations.push_back(lineOut);
    lineStream.str("");

    // The first state will always be WAIT
    lineStream << "\t\tWAIT = " << vecSize << "'d" << stateNum << ",";
    lineOut = lineStream.str();
    declarations.push_back(lineOut);
    lineStream.str("");

    // Create the intermediate states (S0, S1, etc.)
    for (int i = 0; i < numStates; ++i)
    {
        lineStream << "\t\tS" << i << " = " << vecSize << "'d" << ++stateNum;
        lineOut = lineStream.str();
        declarations.push_back(lineOut);
        lineStream.str("");
    }

    // The last state will always be FINAL
    lineStream << "\t\tFINAL = " << vecSize << "'d" << ++stateNum << ",";
    lineOut = lineStream.str();
    declarations.push_back(lineOut);
    lineStream.str("");
}

void Process::writeStateMachineProc(vector<CircuitSignal> &signals, vector<CircuitComp> &components, ofstream &verilogFile)
{
    stringstream lineStream;
    string lineOut;
    list<string> stateMachine;

    // State machine is a synchronous process
    lineStream << "always @(posedge Clk) begin";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    // Reset all outputs and registers
    resetStateMachine(signals, stateMachine);

    // Create state machine Verilog code
    createStateMachine(components, signals, stateMachine);

    // Write state machine to Verilog file
    writeListToFile(stateMachine, verilogFile);
}

void Process::resetStateMachine(vector<CircuitSignal> &signals, list<string> &stateMachine)
{
    int vecSize = static_cast<int>(ceil(log2(static_cast<double>(latency))));
    stringstream lineStream;
    string lineOut;

    // Check for Rst assertion
    lineStream << "\tif (Rst) begin";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    // Done and State are in every Verilog file
    lineStream << "\t\tDone <= 1'b0;";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    lineStream << "\t\tState <= WAIT;";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    //lineStream << "\treg [" << msb << ":0] CycleCnt;";
    lineStream << "\t\tCycleCnt <= " << vecSize << "'b0;";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");
    
    // Reset all other registers and outputs
    for (vector<CircuitSignal>::iterator it = signals.begin(); it != signals.end(); ++it)
    {
        string signalType = it->getCircuitSignalType();
        if ((signalType == "output") || (signalType == "variable"))
        {
            lineStream << "\t\t" << it->getCircuitSignalName() << " <= " << it->getCircuitSignalWidth() << "'b0;";
            lineOut = lineStream.str();
            stateMachine.push_back(lineOut);
            lineStream.str("");
        }
    }
}

void Process::createStateMachine(vector<CircuitComp> &components, vector<CircuitSignal> &signals, list<string> &stateMachine)
{
    int stateNum = 0;
    int ifStateNum = 0;
    int elseStateNum = 0;
    int forkStateNum = 0;
    int vecSize = static_cast<int>(ceil(log2(static_cast<double>(latency))));
    stringstream lineStream;
    string lineOut;
    string lineTabs = "\t\t\t\t";
    string assignToState;
    CompLocation compAndIf;
    vector<CircuitSignal> ifCondSigs;
    vector<CompLocation> compsAndIfs;
    map<int, vector<CompLocation>> compTimeMap;
    bool stateDeclared = false;
    bool ifStateDeclared = false;
    bool elseStateDeclared = false;
    bool first = true;
    string opSymbol;
    string result;
    string op1;
    string op2;
    string sel;

    lineStream << "\tend else begin";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    createWaitState(stateMachine);
    
    for (vector<CircuitSignal>::iterator ifCondIt = signals.begin(); ifCondIt != signals.end(); ++ifCondIt)
    {
        if (ifCondIt->getCircuitSignalIfCondition())
        {
            ifCondSigs.push_back(*ifCondIt);
        }
    }
    
    for (int i = 0; i < latency; ++i)
    {
        for (vector<CircuitComp>::iterator compIt = components.begin(); compIt != components.end(); ++compIt)
        {
            if (compIt->timeframe[0] == i)
            {
                if (compIt->ifStatementLevel > 0)
                {
                    if (compIt->ifStatementLevel > compIt->elseStatementLevel)
                    {
                        compAndIf.ifElseLevel =  compIt->ifStatementLevel;
                        compAndIf.ifOrElse = IF_TYPE;
                        compAndIf.compToLocate = *compIt;
                    }
                    else
                    {
                        compAndIf.ifElseLevel = compIt->elseStatementLevel;
                        compAndIf.ifOrElse = ELSE_TYPE;
                        compAndIf.compToLocate = *compIt;
                    }
                }
                else
                {
                    compAndIf.ifElseLevel = 0;
                    compAndIf.ifOrElse = NEITHER_TYPE;
                    compAndIf.compToLocate = *compIt;
                }
                compsAndIfs.push_back(compAndIf);
            }
        }
        compTimeMap.insert(pair<int, vector<CompLocation>>(i, compsAndIfs));
        compsAndIfs.clear();
    }
    
    // Determine what happens in each state
    for (int i = 0; i < latency; ++i)
    {
        for (map<int, vector<CompLocation>>::iterator mapIt = compTimeMap.begin(); mapIt != compTimeMap.end(); ++mapIt)
        {
            if (mapIt->first == i)
            {
                for (int j = 0; j < static_cast<int>(mapIt->second.size()); ++j)
                {
                    if (mapIt->second.at(j).ifElseLevel == 0)
                    {
                        if (!stateDeclared)
                        {
                            if (!first)
                            {
                                lineStream << lineTabs << "\tState <= S" << stateNum << ";";
                                lineOut = lineStream.str();
                                stateMachine.push_back(lineOut);
                                lineStream.str("");
                                    
                                lineStream << lineTabs << "end";
                                lineOut = lineStream.str();
                                stateMachine.push_back(lineOut);
                                lineStream.str("");
                                
                                lineStream << lineTabs << "CycleCnt <= CycleCnt + 1;";
                                lineOut = lineStream.str();
                                stateMachine.push_back(lineOut);
                                lineStream.str("");
                                
                                lineStream << "\t\t\tend";
                                lineOut = lineStream.str();
                                stateMachine.push_back(lineOut);
                                lineStream.str("");
                            }
                            
                            lineStream << "\t\tS" << stateNum << " :";
                            lineOut = lineStream.str();
                            stateMachine.push_back(lineOut);
                            lineStream.str("");
    
                            lineStream << "\t\t\tbegin";
                            lineOut = lineStream.str();
                            stateMachine.push_back(lineOut);
                            lineStream.str("");
                            
                            lineStream << lineTabs << "if (CycleCnt == " << vecSize << "'d" << i << ") begin";
                            lineOut = lineStream.str();
                            stateMachine.push_back(lineOut);
                            lineStream.str("");
                                
                            stateDeclared = true;
                        }
                        
                        stateNum++;
                    }
                    else
                    {
                        if (mapIt->second.at(j).ifOrElse == IF_TYPE)
                        {
                            if (!ifStateDeclared)
                            {
                                if (!first)
                                {
                                    lineStream << lineTabs << "\tState <= FORK" << forkStateNum << ";";
                                    lineOut = lineStream.str();
                                    stateMachine.push_back(lineOut);
                                    lineStream.str("");
                                    
                                    lineStream << lineTabs << "end";
                                    lineOut = lineStream.str();
                                    stateMachine.push_back(lineOut);
                                    lineStream.str("");
                                    
                                    lineStream << lineTabs << "CycleCnt <= CycleCnt + 1;";
                                    lineOut = lineStream.str();
                                    stateMachine.push_back(lineOut);
                                    lineStream.str("");
                                    
                                    lineStream << "\t\t\tend";
                                    lineOut = lineStream.str();
                                    stateMachine.push_back(lineOut);
                                    lineStream.str("");
                                }
                                
                                lineStream << "\t\tFORK" << forkStateNum << " :";
                                lineOut = lineStream.str();
                                stateMachine.push_back(lineOut);
                                lineStream.str("");
                                
                                lineStream << "\t\t\tbegin";
                                lineOut = lineStream.str();
                                stateMachine.push_back(lineOut);
                                lineStream.str("");
                                
                                for (vector<CircuitSignal>::iterator sigIt = ifCondSigs.begin(); sigIt != ifCondSigs.end(); ++sigIt)
                                {
                                    if ((sigIt->getCircuitSignalIfCondition()) && (sigIt->getCircuitSignalIfConditionLevel() == mapIt->second.at(j).ifElseLevel))
                                    {
                                        lineStream << lineTabs << "if (" << sigIt->getCircuitSignalName() << ") begin";
                                        lineOut = lineStream.str();
                                        stateMachine.push_back(lineOut);
                                        lineStream.str("");
                                    }
                                }
                                
                                lineStream << lineTabs << "\tState <= IF" << ifStateNum << ";";
                                lineOut = lineStream.str();
                                stateMachine.push_back(lineOut);
                                lineStream.str("");
                                
                                lineStream << lineTabs << "end else begin";
                                lineOut = lineStream.str();
                                stateMachine.push_back(lineOut);
                                lineStream.str("");
                                
                                lineStream << lineTabs << "end";
                                lineOut = lineStream.str();
                                stateMachine.push_back(lineOut);
                                lineStream.str("");
                                
                                lineStream << "\t\t\tend";
                                lineOut = lineStream.str();
                                stateMachine.push_back(lineOut);
                                lineStream.str("");
                                
                                lineStream << "\t\tIF" << ifStateNum << " :";
                                lineOut = lineStream.str();
                                stateMachine.push_back(lineOut);
                                lineStream.str("");
                                
                                lineStream << "\t\t\tbegin";
                                lineOut = lineStream.str();
                                stateMachine.push_back(lineOut);
                                lineStream.str("");
                                
                                lineStream << lineTabs << "if (CycleCnt == " << vecSize << "'d" << i << ") begin";
                                lineOut = lineStream.str();
                                stateMachine.push_back(lineOut);
                                lineStream.str("");
                                
                                ifStateDeclared = true;
                                forkStateNum++;
                                ifStateNum++;
                            }
                        }
                        else if (mapIt->second.at(j).ifOrElse == ELSE_TYPE)
                        {
                            if (!elseStateDeclared)
                            {
                                if (!first)
                                {
                                    lineStream << lineTabs << "\tState <= S" << stateNum << ";";
                                    lineOut = lineStream.str();
                                    stateMachine.push_back(lineOut);
                                    lineStream.str("");
                                    
                                    lineStream << lineTabs << "end";
                                    lineOut = lineStream.str();
                                    stateMachine.push_back(lineOut);
                                    lineStream.str("");
                                    
                                    lineStream << lineTabs << "CycleCnt <= CycleCnt + 1;";
                                    lineOut = lineStream.str();
                                    stateMachine.push_back(lineOut);
                                    lineStream.str("");
                                    
                                    lineStream << "\t\t\tend";
                                    lineOut = lineStream.str();
                                    stateMachine.push_back(lineOut);
                                    lineStream.str("");
                                }
                                
                                lineStream << "\t\tELSE" << elseStateNum << " :";
                                lineOut = lineStream.str();
                                stateMachine.push_back(lineOut);
                                lineStream.str("");
                                
                                lineStream << "\t\t\tbegin";
                                lineOut = lineStream.str();
                                stateMachine.push_back(lineOut);
                                lineStream.str("");
                                
                                lineStream << lineTabs << "if (CycleCnt == " << vecSize << "'d" << i << ") begin";
                                lineOut = lineStream.str();
                                stateMachine.push_back(lineOut);
                                lineStream.str("");
                                
                                elseStateDeclared = true;
                                elseStateNum++;
                            }
                        }
                    }
                    
                    first = false;
                
                    convertComp(mapIt->second.at(j).compToLocate, opSymbol, result, op1, op2, sel);

                    // Write Verilog equivalent of component
                    if (mapIt->second.at(j).compToLocate.type.find("MUX") != string::npos)
                    {
                        lineStream << lineTabs << "\tif (" << sel << ") " << result << " <= " << op1 << ";";
                        lineOut = lineStream.str();
                        stateMachine.push_back(lineOut);
                        lineStream.str("");

                        lineStream << lineTabs << "\telse " << result << " <= " << op2 << ";";
                        lineOut = lineStream.str();
                        stateMachine.push_back(lineOut);
                        lineStream.str("");
                    }
                    else if (mapIt->second.at(j).compToLocate.type.find("COMP") != string::npos)
                    {
                        lineStream << lineTabs << "\tif (" << op1 << " " << opSymbol << " " << op2 << ") " << result << " <= 1'b1;";
                        lineOut = lineStream.str();
                        stateMachine.push_back(lineOut);
                        lineStream.str("");

                        lineStream << lineTabs << "\telse " << result << " <= 1'b0;";
                        lineOut = lineStream.str();
                        stateMachine.push_back(lineOut);
                        lineStream.str("");
                    }
                    else
                    {
                        lineStream << lineTabs << "\t" << result << " <= " << op1 << " " << opSymbol;
                        if (!op2.empty()) lineStream << " " << op2;
                        lineStream << ";";
                        lineOut = lineStream.str();
                        stateMachine.push_back(lineOut);
                        lineStream.str("");
                    }
                }
            }
        }
        
        stateDeclared = false;
        ifStateDeclared = false;
        elseStateDeclared = false;
    }

    createFinalState(components, stateMachine);
}

void Process::createWaitState(list<string> &stateMachine)
{
    stringstream lineStream;
    string lineOut;

    lineStream << "\t\tcase (State)";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    // Wait for the Start signal
    lineStream << "\t\tWAIT :";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    lineStream << "\t\t\tbegin";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    lineStream << "\t\t\t\tDone <= 1'b0;";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    lineStream << "\t\t\t\tif (Start) begin";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    lineStream << "\t\t\t\t\tState <= S0;";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    lineStream << "\t\t\t\tend";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    lineStream << "\t\t\tend";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");
}

void Process::convertComp(CircuitComp &component, string &opSymbol, string &result, string &op1, string &op2, string &sel)
{
    opSymbol.clear();
    result.clear();
    op1.clear();
    op2.clear();
    sel.clear();

    if (component.type.find("ADD") != string::npos)
    {
        //cout << "Convert: in find ADD" << endl;
        opSymbol = "+";
        result = component.result.getCircuitSignalName();
    }
    else if (component.type.find("SUB") != string::npos)
    {
        opSymbol = "-";
        result = component.result.getCircuitSignalName();
    }
    else if (component.type.find("MUL") != string::npos)
    {
        opSymbol = "*";
        result = component.result.getCircuitSignalName();
    }
    else if (component.type.find("COMP") != string::npos)
    {
        if (component.comparatorOp == GT)
        {
            opSymbol = ">";
        }
        else if (component.comparatorOp == LT)
        {
            opSymbol = "<";
        }
        else
        {
            opSymbol = "==";
        }
    }
    else if (component.type.find("SHR") != string::npos)
    {
        opSymbol = ">>";
        result = component.result.getCircuitSignalName();
    }
    else if (component.type.find("SHL") != string::npos)
    {
        opSymbol = "<<";
        result = component.result.getCircuitSignalName();
    }
    else if (component.type.find("DIV") != string::npos)
    {
        opSymbol = "/";
        result = component.result.getCircuitSignalName();
    }
    else if (component.type.find("MOD") != string::npos)
    {
        opSymbol = "%";
        result = component.result.getCircuitSignalName();
    }
    else if (component.type.find("INC") != string::npos)
    {
        opSymbol = "+ 1";
        result = component.result.getCircuitSignalName();
    }
    else if (component.type.find("DEC") != string::npos)
    {
        opSymbol = "- 1";
        result = component.result.getCircuitSignalName();
    }

    //cout << "Convert operands size: " << static_cast<int>(component.operands.size()) << endl;
    for (int i = 0; i < component.operands.size(); ++i)
    {
        //cout << "Convert: in for loop" << endl;
        if ((component.type.find("INC") != string::npos) ||
            (component.type.find("DEC") != string::npos))
        {
            if (component.operands.at(i).getCircuitSignalPosition() == "op1")
            {
                op1 = component.operands.at(i).getCircuitSignalName();
            }
        }
        else if (component.type.find("MUX") != string::npos)
        {
            if (component.operands.at(i).getCircuitSignalPosition() == "sel")
            {
                sel = component.operands.at(i).getCircuitSignalName();
            }
            else if (component.operands.at(i).getCircuitSignalPosition() == "op1")
            {
                op1 = component.operands.at(i).getCircuitSignalName();
            }
            else if (component.operands.at(i).getCircuitSignalPosition() == "op2")
            {
                op2 = component.operands.at(i).getCircuitSignalName();
            }
        }
        else if (component.type.find("COMP") != string::npos)
        {
            //cout << "Convert: In COMP ops" << endl;
            if (((component.comparatorOp == GT) && (component.operands.at(i).getCircuitSignalPosition() == "op3")) ||
                ((component.comparatorOp == LT) && (component.operands.at(i).getCircuitSignalPosition() == "op4")) ||
                ((component.comparatorOp == EQ) && (component.operands.at(i).getCircuitSignalPosition() == "op5")))
            {
                result = component.operands.at(i).getCircuitSignalName();
            }
            else if (component.operands.at(i).getCircuitSignalPosition() == "op1")
            {
                op1 = component.operands.at(i).getCircuitSignalName();
            }
            else if (component.operands.at(i).getCircuitSignalPosition() == "op2")
            {
                op2 = component.operands.at(i).getCircuitSignalName();
            }
        }
        else
        {
            //cout << "Convert: In other ops" << endl;
            if (component.operands.at(i).getCircuitSignalPosition() == "op1")
            {
                op1 = component.operands.at(i).getCircuitSignalName();
            }
            else if (component.operands.at(i).getCircuitSignalPosition() == "op2")
            {
                op2 = component.operands.at(i).getCircuitSignalName();
            }
        }
    }
}

void Process::createFinalState(vector<CircuitComp> &components, list<string> &stateMachine)
{
    int vecSize = static_cast<int>(ceil(log2(static_cast<double>(latency))));
    stringstream lineStream;
    string lineOut;
    bool foundEntry = false;

    lineStream << "\t\t\t\t\tState <= FINAL;";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");
    
    lineStream << "\t\t\t\tend";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");
    
    lineStream << "\t\t\t\tCycleCnt <= CycleCnt + 1;";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");
    
    lineStream << "\t\t\tend";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");
    
    // Wait for the latency
    lineStream << "\t\tFINAL :";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    lineStream << "\t\t\tbegin";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    // Find latest end time
    for (vector<CircuitComp>::iterator it = components.begin(); it != components.end(); ++it)
    {
        for (set<int>::iterator it2 = endTimeSet.begin(); (it2 != endTimeSet.end() && !foundEntry); ++it2)
        {
            if (it->timeframe[1] == *it2)
            {
                foundEntry = true;
            }
        }

        if (!foundEntry)
            endTimeSet.insert(it->timeframe[1]);

        foundEntry = false;
    }

    set<int>::reverse_iterator etit = endTimeSet.rbegin();
    lineStream << "\t\t\t\tif (CycleCnt == " << vecSize << "'d" << *etit << ") begin";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    lineStream << "\t\t\t\t\tDone <= 1'b1;";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    lineStream << "\t\t\t\t\tState <= WAIT;";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    lineStream << "\t\t\t\tend";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    lineStream << "\t\t\t\tCycleCnt <= CycleCnt + 1;";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    lineStream << "\t\t\tend";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    lineStream << "\t\tendcase";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    lineStream << "\tend";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    lineStream << "end";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");

    lineStream << "endmodule";
    lineOut = lineStream.str();
    stateMachine.push_back(lineOut);
    lineStream.str("");
}

void Process::setVerilogFilename(const char *vFilename)
{
    string inVerilogFilename(vFilename);
    string::size_type pos = inVerilogFilename.find_last_of('/');
    if (pos != string::npos)
    {
        verilogFilename = inVerilogFilename.substr(pos + 1, inVerilogFilename.length());
    }
    else
    {
        verilogFilename = inVerilogFilename;
    }
}

