#include "Process.h"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

struct CircuitComp
{
    string type;
    string result;
    vector<string> operands;
    double delay;
};

Process::Process()
{
}

Process::~Process()
{
}

int Process::parseNetlist(ifstream &netList, ofstream &verilogFile)
{
    // jab added int status = -1; 10/31/2018
    int status = -1;
    string comment = "//";
    CircuitComp circuitComp;
    vector<CircuitSignal> circuitVector;
    vector<CircuitComp> circuitCompVector;

    for (string line; getline(netList, line); )
    {
        // first we will remove all comments from the line
        //cout << line << endl;
        string::size_type commentIndex = line.find(comment);

        if (commentIndex != string::npos)
            line.erase(commentIndex, line.length());

        //cout << line << endl;
        // jab added || (line.find("register") ... 10/31/2018
        if ((line.find("input") != string::npos) || (line.find("output") != string::npos) || (line.find("wire") != string::npos)
            || (line.find("register") != string::npos))
        {
            cout << "here" << endl;
            parseSignals(netList, circuitVector, line);
        }
        else
        {
            stringstream lineStream(line);

            if (line.find("+ 1") != string::npos)
            {
                cout << "INC" << endl;
                string equal, result, op1;

                lineStream >> circuitComp.result >> equal >> op1;

                circuitComp.type = "INC";

                circuitComp.operands.push_back(op1);

                circuitCompVector.push_back(circuitComp);

                // cout << circuitCompVector.at(0).type << " " << circuitCompVector.at(0).result << " " << circuitCompVector.at(0).operands.at(0) << " " << circuitCompVector.at(0).operands.at(0) << endl;
            }
            else if (line.find("- 1") != string::npos)
            {
                cout << "DEC" << endl;

                string equal, result, op1;

                lineStream >> circuitComp.result >> equal >> op1;

                circuitComp.type = "DEC";

                circuitComp.operands.push_back(op1);

                circuitCompVector.push_back(circuitComp);
            }
            else if (line.find("+") != string::npos)
            {
                cout << "ADD" << endl;
                string equal, op, op1, op2;

                lineStream >> circuitComp.result >> equal >> op1 >> op >> op2;

                circuitComp.type = "ADD";

                circuitComp.operands.push_back(op1);
                circuitComp.operands.push_back(op2);

                circuitCompVector.push_back(circuitComp);

                cout << circuitCompVector.at(0).type << " " << circuitCompVector.at(0).result << " " << circuitCompVector.at(0).operands.at(0) << " " << circuitCompVector.at(0).operands.at(1) << endl;

            }
            else if (line.find("-") != string::npos)
            {
                cout << "SUB" << endl;
                string equal, op, op1, op2;

                lineStream >> circuitComp.result >> equal >> op1 >> op >> op2;

                circuitComp.type = "SUB";

                circuitComp.operands.push_back(op1);
                circuitComp.operands.push_back(op2);

                circuitCompVector.push_back(circuitComp);
            }
            else if (line.find("*") != string::npos)
            {
                string equal, op, op1, op2;

                lineStream >> circuitComp.result >> equal >> op1 >> op >> op2;

                circuitComp.type = "MUL";

                circuitComp.operands.push_back(op1);
                circuitComp.operands.push_back(op2);

                circuitCompVector.push_back(circuitComp);
            }
            else if (line.find("/") != string::npos)
            {
                string equal, op, op1, op2;

                lineStream >> circuitComp.result >> equal >> op1 >> op >> op2;

                circuitComp.type = "DIV";

                circuitComp.operands.push_back(op1);
                circuitComp.operands.push_back(op2);

                circuitCompVector.push_back(circuitComp);
            }
            else if (line.find("<<") != string::npos)
            {
                circuitComp.type = "SHL";
            }
            else if (line.find(">>") != string::npos)
            {
                circuitComp.type = "SHR";
            }
            else if (line.find("?") != string::npos)
            {
                string equal, op, colon, sel, op1, op2;

                //TODO: check if operands exist and record delay time (new function?)
                //int muxTime[6] = {4.083, 4.115, 4.815, 5.623, 8.079, 8.766};

                lineStream >> circuitComp.result >> equal >> sel >> op >> op1 >> colon >> op2;

                circuitComp.type = "MUX";

                cout << "MUX found:" << circuitComp.result << equal << sel << op << op1 << colon << op2;

                circuitComp.operands.push_back(op1);
                circuitComp.operands.push_back(op2);
                circuitComp.operands.push_back(sel);

                circuitCompVector.push_back(circuitComp);
            }
            else if (line.find("%") != string::npos)
            {
                string equal, op, op1, op2;

                lineStream >> circuitComp.result >> equal >> op1 >> op >> op2;

                circuitComp.type = "MOD";

                circuitComp.operands.push_back(op1);
                circuitComp.operands.push_back(op2);

                circuitCompVector.push_back(circuitComp);
            }
            else if ((line.find("<") != string::npos) || (line.find(">") != string::npos) || (line.find("==") != string::npos))
            {
                circuitComp.type = "COMP";
            }
            //REG?
            else
            {
                circuitComp.type = "REG";
            }

        }
    }

    // jab added write calls 10/31/2018
    status = writeDeclarations(circuitVector, verilogFile);
    status = writeInstantiations(circuitCompVector, verilogFile);

    cout << "Signal is: " << circuitVector.at(0).getCircuitSignalType() << " DataType is: " << circuitVector.at(0).getCircuitSignalDataType() << " Variable is: " << circuitVector.at(0).getCircuitSignalName() << " Width is: " << circuitVector.at(0).getCircuitSignalWidth() << endl;
    cout << "Signal is: " << circuitVector.at(1).getCircuitSignalType() << " DataType is: " << circuitVector.at(1).getCircuitSignalDataType() << " Variable is: " << circuitVector.at(1).getCircuitSignalName() << " Width is: " << circuitVector.at(1).getCircuitSignalWidth() << endl;
    cout << "Signal is: " << circuitVector.at(2).getCircuitSignalType() << " DataType is: " << circuitVector.at(2).getCircuitSignalDataType() << " Variable is: " << circuitVector.at(2).getCircuitSignalName() << " Width is: " << circuitVector.at(2).getCircuitSignalWidth() << endl;
    cout << "Signal is: " << circuitVector.at(3).getCircuitSignalType() << " DataType is: " << circuitVector.at(3).getCircuitSignalDataType() << " Variable is: " << circuitVector.at(3).getCircuitSignalName() << " Width is: " << circuitVector.at(3).getCircuitSignalWidth() << endl;


    return 0;
}

void Process::parseSignals(ifstream &netList, vector<CircuitSignal> &circuitVector, std::string &line)
{
    CircuitSignal circuitSignal;
    stringstream lineStream(line);
    string signal, dataType, variable, dataWidth;
    vector<string> variables;
    bool multipleVariables = false;

    lineStream >> signal >> dataType;
    lineStream >> variable;

    dataWidth = parseDataType(dataType);

    multipleVariables = parseVariable(variable, variables);

    //cout << variable << endl;

    while (multipleVariables)
    {
        lineStream >> variable;
        multipleVariables = parseVariable(variable, variables);
        //cout << variable << endl;
    }

    for (std::vector<int>::size_type i = 0; i != variables.size(); i++)
    {
        circuitSignal.setCircuitSignalType(signal);
        circuitSignal.setCircuitSignalDataType(dataType);
        circuitSignal.setCircuitSignalWidth(dataWidth);
        circuitSignal.setCircuitSignalName(variables[i]);

        circuitVector.push_back(circuitSignal);
    }
    //cout << "Signal is: " << circuitVector.at(2).getCircuitSignalType() << " DataType is: " << circuitVector.at(2).getCircuitSignalDataType() << " Variable is: " << circuitVector.at(2).getCircuitSignalName() <<  " Width is: " << circuitVector.at(2).getCircuitSignalWidth() << endl;
    //cout << "Signal is: " << circuitVector.at(2).getCircuitSignalType() << " DataType is: " << circuitVector.at(2).getCircuitSignalDataType() << " Variable is: " << circuitVector.at(2).getCircuitSignalName() << endl;

}

bool Process::parseVariable(string variable, vector<string> &variables)
{
    string::size_type pos = variable.find(",");

    if (pos == string::npos)
    {
        variables.push_back(variable);

        return false;
    }
    else
    {
        variable = variable.erase(pos, variable.length());
        variables.push_back(variable);

        return true;
    }
}

string Process::parseDataType(string &dataType)
{
    string width;

    string::size_type pos = dataType.find("Int");

    width = dataType.substr(pos + 3, dataType.length()); //create string which represents the data width after "Int"

    dataType = dataType.substr(0, pos + 3); // change widthString to be just "Int" or "UInt" need to test

    return width;

}

int Process::writeDeclarations(vector<CircuitSignal> &signals, ofstream &verilogFile)
{
    // TODO: Write Code
    int status = -1;
    return status;
}

int Process::writeInstantiations(vector<CircuitComp> &components, ofstream &verilogFile)
{
    // TODO: Write Code
    int status = -1;
    return status;
}