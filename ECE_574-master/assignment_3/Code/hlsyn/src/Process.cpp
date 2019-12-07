#include "Process.h"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include <cmath>
#include <algorithm>

#include "getValue.h"
#include "dpgen.h"
#include "FDS.h"

using namespace std;


GetValue delays;

Process::Process()
{
}

Process::~Process()
{
}

int Process::parseNetlist(ifstream &netList, ofstream &verilogFile)
{
    int status = UNKNOWN_ERROR;
    string comment =  "//";
	vector<CircuitSignal> circuitVector;
	vector<CircuitComp> circuitCompVector;
	bool startComps = false;
	
	string currentIfOrElse = "none";
	
	ifStatement = false;
	ifLevel = 0;
	elseLevel = 0;
	
	for (string line; getline(netList, line); )
	{
		// first we will remove all comments from the line
		string::size_type commentIndex = line.find(comment);

		if (commentIndex != string::npos)
			line.erase(commentIndex, line.length());
		
		if(((line.find("input") != string::npos) || (line.find("output") != string::npos) || ((line.find("variable") != string::npos) && (line.find("=") == string::npos))
            || (line.find("register") != string::npos)) && !startComps)
		{
			//cout << line << endl;
			parseSignals(netList, circuitVector, line);
		}
		else
		{
			if(!line.empty())
			{	
				startComps = true;
				
				stringstream lineStream(line);
				
				if(line.find("+ 1") != string::npos)
				{	
					status = parseOneOperand(circuitVector, circuitCompVector, lineStream, "INC");
				}
				else if(line.find("- 1") != string::npos)
				{
					status = parseOneOperand(circuitVector, circuitCompVector, lineStream, "DEC");
				}
				else if(line.find("+") != string::npos)
				{	
					status = parseTwoOperands(circuitVector, circuitCompVector, lineStream, "ADD");
					
					//cout << circuitCompVector.at(0).type << " " << circuitCompVector.at(0).result << " " << circuitCompVector.at(0).operands.at(0) << " " << circuitCompVector.at(0).operands.at(1) << endl;
				
				}
				else if(line.find("-") != string::npos)
				{
					status = parseTwoOperands(circuitVector, circuitCompVector, lineStream, "SUB");	
				}			
				else if(line.find("*") != string::npos)
				{
					status = parseTwoOperands(circuitVector, circuitCompVector, lineStream, "MUL");
				}
				else if(line.find("/") != string::npos)
				{				
					status = parseTwoOperands(circuitVector, circuitCompVector, lineStream, "DIV");
				}
				else if(line.find("<<") != string::npos)
				{
					status = parseTwoOperands(circuitVector, circuitCompVector, lineStream, "SHL");
				}		
				else if(line.find(">>") != string::npos)
				{
					status = parseTwoOperands(circuitVector, circuitCompVector, lineStream, "SHR");
				}	
				else if(line.find("?") != string::npos)
				{
					status = parseMux(circuitVector, circuitCompVector, lineStream, "MUX");	
				}			
				else if(line.find("%") != string::npos)
				{	
					status = parseTwoOperands(circuitVector, circuitCompVector, lineStream, "MOD");
				}
				else if((line.find("<") != string::npos) || (line.find(">") != string::npos) || (line.find("==") != string::npos))
				{
					//NOTE: COMP will be stored as 5 operands in the following order
					// op1 will be 1st input, op2 will be 2nd input, op3 will be gt, op4 will be lt, op5 will be eq
					// if we are gt, op3 will have value, op4 and op5 will be 0 
					// ie if we have c = a == b, a will be op1, b will be op2, 0 will be op3, 0 will be op4, c will be op5
					// for extra help I will also put c in the result for the example above
					
					status = parseComp(circuitVector, circuitCompVector, lineStream, "COMP");
					//circuitComp.type = "COMP";
				}
				else if((line.find("&") != string::npos) || (line.find("^") != string::npos) || (line.find("#") != string::npos) 
					|| (line.find("$") != string::npos) || (line.find("@") != string::npos))
				{
					status = FAIL_OPERATOR;
					return status;
				}
				else if((line.find("if") != string::npos))
				{	
					//cout << line << endl;
					string ignoreIf, ignoreParen1, condition;
					lineStream >> ignoreIf >> ignoreParen1 >> condition;
					//cout << condition << endl;
					ifStatement = true;
					ifLevel++;
					
					for (std::vector<int>::size_type i = 0; i != circuitVector.size(); i++)
					{
						if(circuitVector.at(i).getCircuitSignalName() == condition)
						{
							//cout << circuitVector.at(i).getCircuitSignalName() << endl;
							circuitVector.at(i).setCircuitSignalIfCondition(true);
							circuitVector.at(i).setCircuitSignalIfConditionLevel(ifLevel);
							if(currentIfOrElse == "if")
							{
								circuitVector.at(i).setIfNestedWith(currentIfOrElse+to_string(ifLevel-1));
							}
							else if(currentIfOrElse == "else")
							{
								circuitVector.at(i).setIfNestedWith(currentIfOrElse+to_string(elseLevel));
							}
							
							//cout << circuitVector.at(i).getIfNestedWith() << endl;
						}
					}
					
					currentIfOrElse = "if";
				}
				else if((line.find("}") != string::npos) && elseLevel == 0)
				{
					ifStatement = false;
					ifLevel--;
				}
				else if(line.find("else") != string::npos)
				{
					currentIfOrElse = "else";
					
					elseLevel = ifLevel+1;
				}
				else if((line.find("}") != string::npos) && elseLevel > 0)
				{
					elseLevel--;
				}
				//REG?
				else
				{	
					//cout << line << endl;
					//status = parseOneOperand(circuitVector, circuitCompVector, lineStream, "REG");
				}
				
				
				if (status != SUCCESS)
				{
					return status;
				}
			}
		}
	}
	FDS fds(latency);
	fds.scheduleFDS(circuitCompVector);
	
	status = writeDeclarations(circuitVector, circuitCompVector, verilogFile);
	if(status == 1)
	{
		writeStateMachineProc(circuitVector, circuitCompVector, verilogFile);
	}
	else
	{
		return status;
	}

	//cout << "Critical Path: " << CalculateCriticalPath(circuitCompVector) << "ns" <<endl;
	
	return status;
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
	
	while(multipleVariables) 
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
	
	if(pos == string::npos)
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
	
	width = "invalid";
	
	string::size_type pos = dataType.find("Int");
	
    if(pos != string::npos)
	{
		width = dataType.substr(pos+3, dataType.length()); //create string which represents the data width after "Int"
	
		dataType = dataType.substr(0,pos+3); // change widthString to be just "Int" or "UInt" need to test
		
		//cout << width << " " << dataType << endl;
	}

	return width;

}

int Process::parseOneOperand(vector<CircuitSignal> &circuitVector, vector<CircuitComp> &circuitCompVector, stringstream &lineStream, string type)
{
	CircuitComp circuitComp;
	string result, equal, op, op1;
				
	lineStream >> result >> equal >> op1;
	
	circuitComp.type = type;

	circuitComp.enumType = stringToEnumType(type);
	
	circuitComp.comparatorOp = NOT_COMP;

	circuitComp.cycleTime = getCycleTime(type);
	
	// loop through entire circuit signal vector looking for any input/output/wire/reg within the 
	// component instantiation 
	// if you never found one of the values error out
	int numOperandsFound = 0;
	for (std::vector<int>::size_type i = 0; i != circuitVector.size(); i++)
	{					
		if((circuitVector.at(i).getCircuitSignalType() == "input") || (circuitVector.at(i).getCircuitSignalType() == "variable") 
			|| (circuitVector.at(i).getCircuitSignalType() == "output") || (circuitVector.at(i).getCircuitSignalType() == "register"))
		{
			if(circuitVector.at(i).getCircuitSignalName() == op1)
			{
				//found input/wire
				numOperandsFound++;
				
				circuitVector.at(i).setCircuitSignalPosition("op1");
				
				circuitComp.operands.push_back(circuitVector.at(i));
			}
			else if(circuitVector.at(i).getCircuitSignalName() == result)
			{
				numOperandsFound++;
				
				circuitVector.at(i).setCircuitSignalPosition("result");

                if (circuitVector.at(i).getCircuitSignalDataType().find('U') == string::npos)
                {
                    circuitComp.type = 'S' + type;
                }

				//add in getting delay
				
				circuitComp.width = circuitVector.at(i).getCircuitSignalWidth();
				circuitComp.delay = delays.stringToDelay(type, circuitComp.width);
				//circuitComp.delay += addDependentDelaysOneOP(circuitCompVector, op1);
				//cout << "delay: " << circuitComp.delay << endl;
				circuitComp.result = circuitVector.at(i);
				circuitComp.operands.push_back(circuitVector.at(i));
				//found output/wire
			}
		}
	}
	
	if(numOperandsFound != 2)
	{
		return FAIL_OPERAND;
	}
	else
	{
		// if(ifLevel > 0)
		// {
			// circuitComp.ifStatement = true;
		// }
		// else
		// {
			// circuitComp.ifStatement = false;
		// }
		
		circuitComp.elseStatementLevel = elseLevel;
		circuitComp.ifStatementLevel = ifLevel;
		circuitCompVector.push_back(circuitComp);
		
		return SUCCESS;
	}
}

int Process::parseTwoOperands(vector<CircuitSignal> &circuitVector, vector<CircuitComp> &circuitCompVector, stringstream &lineStream, string type)
{
	CircuitComp circuitComp;
	string result, equal, op, op1, op2;
				
	lineStream >> result >> equal >> op1 >> op >> op2;
	
	circuitComp.type = type;
	
	circuitComp.enumType = stringToEnumType(type);
	
	circuitComp.comparatorOp = NOT_COMP;

	circuitComp.cycleTime = getCycleTime(type);
	
	// loop through entire circuit signal vector looking for any input/output/wire/reg within the 
	// component instantiation 
	// if you never found one of the values error out
	int numOperandsFound = 0;
	for (std::vector<int>::size_type i = 0; i != circuitVector.size(); i++)
	{					
		if((circuitVector.at(i).getCircuitSignalType() == "input") || (circuitVector.at(i).getCircuitSignalType() == "variable") 
			|| (circuitVector.at(i).getCircuitSignalType() == "output") || (circuitVector.at(i).getCircuitSignalType() == "register"))
		{
			if(circuitVector.at(i).getCircuitSignalName() == op1)
			{
				//found input/wire
				numOperandsFound++;
				
				circuitVector.at(i).setCircuitSignalPosition("op1");
				
				circuitComp.operands.push_back(circuitVector.at(i));
			}
			else if(circuitVector.at(i).getCircuitSignalName() == op2)
			{
				//found input/wire
				numOperandsFound++;
				
				circuitVector.at(i).setCircuitSignalPosition("op2");
				
				circuitComp.operands.push_back(circuitVector.at(i));
			}
			else if(circuitVector.at(i).getCircuitSignalName() == result)
			{
				numOperandsFound++;
				
				circuitVector.at(i).setCircuitSignalPosition("result");
				
                if (circuitVector.at(i).getCircuitSignalDataType().find('U') == string::npos)
                {
                    circuitComp.type = 'S' + type;
                }

                //add in getting delay
				
				circuitComp.width = circuitVector.at(i).getCircuitSignalWidth();
				circuitComp.delay = delays.stringToDelay(type, circuitComp.width);
				//circuitComp.delay += addDependentDelaysTwoOP(circuitCompVector, op1, op2);
				//cout << "delay: " << circuitComp.delay << endl;
				circuitComp.result = circuitVector.at(i);
				circuitComp.operands.push_back(circuitVector.at(i));
				//found output/wire
			}
		}
	}
	
	if(numOperandsFound != 3)
	{
		return FAIL_OPERAND;
	}
	else
	{
		// if(inIfLevel > 0)
		// {
			// circuitComp.ifStatement = true;
		// }
		// else
		// {
			// circuitComp.ifStatement = false;
		// }
		
		circuitComp.elseStatementLevel = elseLevel;
		circuitComp.ifStatementLevel = ifLevel;
		circuitCompVector.push_back(circuitComp);
		
		return SUCCESS;
	}
}

int Process::parseComp(vector<CircuitSignal> &circuitVector, vector<CircuitComp> &circuitCompVector, stringstream &lineStream, string type)
{
	CircuitComp circuitComp;
	string result, equal, op1, op, op2;
	string width = "0";
				
	lineStream >> result >> equal >> op1 >> op >> op2;
	
	circuitComp.type = type;
	
	circuitComp.enumType = stringToEnumType(type);
	
	circuitComp.comparatorOp = NOT_COMP;

	circuitComp.cycleTime = getCycleTime(type);
	
	// loop through entire circuit signal vector looking for any input/output/wire/reg within the 
	// component instantiation 
	// if you never found one of the values error out
	int numOperandsFound = 0;
	for (std::vector<int>::size_type i = 0; i != circuitVector.size(); i++)
	{					
		if((circuitVector.at(i).getCircuitSignalType() == "input") || (circuitVector.at(i).getCircuitSignalType() == "variable") 
			|| (circuitVector.at(i).getCircuitSignalType() == "output") || (circuitVector.at(i).getCircuitSignalType() == "register"))
		{
			if(circuitVector.at(i).getCircuitSignalName() == op1)
			{
				//found input/wire
				numOperandsFound++;
				
				if(circuitVector.at(i).getCircuitSignalWidth() > width)
					width = circuitVector.at(i).getCircuitSignalWidth();
				
				circuitVector.at(i).setCircuitSignalPosition("op1");
				
				circuitComp.operands.push_back(circuitVector.at(i));
			}
			else if(circuitVector.at(i).getCircuitSignalName() == op2)
			{
				//found input/wire
				numOperandsFound++;
				
				if(circuitVector.at(i).getCircuitSignalWidth() > width)
					width = circuitVector.at(i).getCircuitSignalWidth();
				
				circuitVector.at(i).setCircuitSignalPosition("op2");
				
				circuitComp.operands.push_back(circuitVector.at(i));
			}
			else if(circuitVector.at(i).getCircuitSignalName() == result)
			{
				numOperandsFound++;
				
				circuitVector.at(i).setCircuitSignalPosition("result");
				
                if (circuitVector.at(i).getCircuitSignalDataType().find('U') == string::npos)
                {
                    circuitComp.type = 'S' + type;
                }

                //add in getting delay
				
				circuitComp.width = width;
				circuitComp.delay = delays.stringToDelay(type, circuitComp.width);
				//circuitComp.delay += addDependentDelaysTwoOP(circuitCompVector, op1, op2);
				//cout << "delay: " << circuitComp.delay << endl;
				circuitComp.result = circuitVector.at(i);
				circuitComp.operands.push_back(circuitVector.at(i));
				//found output/wire
			
			
				if( op == ">")
				{
					circuitComp.comparatorOp = GT;
					circuitVector.at(i).setCircuitSignalPosition("op3");
					circuitComp.operands.push_back(circuitVector.at(i));
				}
				else
				{
					CircuitSignal circuitSignal;
					circuitSignal.setCircuitSignalName("0");
					
					circuitComp.operands.push_back(circuitSignal);
				}
				
				if (op == "<")
				{
					circuitComp.comparatorOp = LT;
					circuitVector.at(i).setCircuitSignalPosition("op4");
					circuitComp.operands.push_back(circuitVector.at(i));
				}
				else
				{
					CircuitSignal circuitSignal;
					circuitSignal.setCircuitSignalName("0");
					
					circuitComp.operands.push_back(circuitSignal);
				}
				if (op == "==")
				{
					circuitComp.comparatorOp = EQ;
					circuitVector.at(i).setCircuitSignalPosition("op5");
					circuitComp.operands.push_back(circuitVector.at(i));
				}
				else
				{
					CircuitSignal circuitSignal;
					circuitSignal.setCircuitSignalName("0");
					
					circuitComp.operands.push_back(circuitSignal);
				}
			}
		}
	}
	
	if(numOperandsFound != 3)
	{
		return FAIL_OPERAND;
	}
	else
	{
		// if(inIfLevel > 0)
		// {
			// circuitComp.ifStatement = true;
		// }
		// else
		// {
			// circuitComp.ifStatement = false;
		// }
		
		circuitComp.elseStatementLevel = elseLevel;
		circuitComp.ifStatementLevel = ifLevel;
		circuitCompVector.push_back(circuitComp);
		
		return SUCCESS;
	}
}

int Process::parseMux(vector<CircuitSignal> &circuitVector, vector<CircuitComp> &circuitCompVector, stringstream &lineStream, string type)
{
	CircuitComp circuitComp;
	string result, equal, sel, op, op1, colon, op2;
				
	lineStream >> result >> equal >> sel >> op >> op1 >> colon >> op2;
	
	circuitComp.type = type;
	
	circuitComp.enumType = stringToEnumType(type);
	
	circuitComp.comparatorOp = NOT_COMP;

	circuitComp.cycleTime = getCycleTime(type);
	
	// loop through entire circuit signal vector looking for any input/output/wire/reg within the 
	// component instantiation 
	// if you never found one of the values error out
	int numOperandsFound = 0;
	for (std::vector<int>::size_type i = 0; i != circuitVector.size(); i++)
	{					
		if((circuitVector.at(i).getCircuitSignalType() == "input") || (circuitVector.at(i).getCircuitSignalType() == "variable") 
			|| (circuitVector.at(i).getCircuitSignalType() == "output") || (circuitVector.at(i).getCircuitSignalType() == "register"))
		{
			if(circuitVector.at(i).getCircuitSignalName() == sel)
			{
				//found input/wire
				numOperandsFound++;
				
				circuitVector.at(i).setCircuitSignalPosition("sel");
				
				circuitComp.operands.push_back(circuitVector.at(i));
			}
			else if(circuitVector.at(i).getCircuitSignalName() == op1)
			{
				//found input/wire
				numOperandsFound++;
				
				circuitVector.at(i).setCircuitSignalPosition("op1");
				
				circuitComp.operands.push_back(circuitVector.at(i));
			}
			else if(circuitVector.at(i).getCircuitSignalName() == op2)
			{
				//found input/wire
				numOperandsFound++;
				
				circuitVector.at(i).setCircuitSignalPosition("op2");
				
				circuitComp.operands.push_back(circuitVector.at(i));
			}
			else if(circuitVector.at(i).getCircuitSignalName() == result)
			{
				numOperandsFound++;
				
				circuitVector.at(i).setCircuitSignalPosition("result");
				
                if (circuitVector.at(i).getCircuitSignalDataType().find('U') == string::npos)
                {
                    circuitComp.type = 'S' + type;
                }

                //add in getting delay
				
				circuitComp.width = circuitVector.at(i).getCircuitSignalWidth();
				circuitComp.delay = delays.stringToDelay(type, circuitComp.width);
				//circuitComp.delay += addDependentDelaysMux(circuitCompVector, sel, op1, op2);
				//cout << "delay: " << circuitComp.delay << endl;
				circuitComp.result = circuitVector.at(i);
				circuitComp.operands.push_back(circuitVector.at(i));
				//found output/wire
			}
		}
	}
	
	if(numOperandsFound != 4)
	{
		return FAIL_OPERAND;
	}
	else
	{
		// if(inIfLevel > 0)
		// {
			// circuitComp.ifStatement = true;
		// }
		// else
		// {
			// circuitComp.ifStatement = false;
		// }
		
		circuitComp.elseStatementLevel = elseLevel;
		circuitComp.ifStatementLevel = ifLevel;
		circuitCompVector.push_back(circuitComp);
		
		return SUCCESS;
	}
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
    int vecSize = static_cast<int>(ceil(log2(static_cast<double>(latency+1))));
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
    declareStates(declarations, components);

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

void Process::declareStates(list<string> &declarations, vector<CircuitComp> circuitCompVector)
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
    for (int i = 0; i <= numStates; ++i)
    {
        lineStream << "\t\tS" << i << " = " << vecSize << "'d" << ++stateNum << ",";
        lineOut = lineStream.str();
        declarations.push_back(lineOut);
        lineStream.str("");
    }

	vector<int> ifLevels;
	vector<int> elseLevels;
	int ifNum = 2;
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++)
	{

		if (circuitCompVector.at(i).ifStatementLevel > 0)
		{
			if (find(ifLevels.begin(), ifLevels.end(), circuitCompVector.at(i).ifStatementLevel) != ifLevels.end())
			{ //do nothing 
			}
			else 
			{
				ifLevels.push_back(circuitCompVector.at(i).ifStatementLevel);
				lineStream << "\t\tIF" << (circuitCompVector.at(i).ifStatementLevel) << " = " << vecSize << "'d" << (stateNum + ifNum) << ",";
				ifNum++;
				lineOut = lineStream.str();
				declarations.push_back(lineOut);
				lineStream.str("");
				lineStream << "\t\tFORK" << (circuitCompVector.at(i).ifStatementLevel) << " = " << vecSize << "'d" << (stateNum + ifNum) << ",";
				ifNum++;
				lineOut = lineStream.str();
				declarations.push_back(lineOut);
				lineStream.str("");
			}
		}
		if (circuitCompVector.at(i).elseStatementLevel > 0)
		{
			if (find(elseLevels.begin(), elseLevels.end(), circuitCompVector.at(i).elseStatementLevel) != elseLevels.end())
			{ //do nothing 
			}
			else
			{
				elseLevels.push_back(circuitCompVector.at(i).elseStatementLevel);
				lineStream << "\t\tELSE" << circuitCompVector.at(i).elseStatementLevel << " = " << vecSize << "'d" << (stateNum + ifNum) << ",";
				ifNum++;
				lineOut = lineStream.str();
				declarations.push_back(lineOut);
				lineStream.str("");
			}
		}
	}

    // The last state will always be FINAL
    lineStream << "\t\tFINAL = " << vecSize << "'d" << ++stateNum << ";";
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
    int vecSize = static_cast<int>(ceil(log2(static_cast<double>(latency+1))));
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
	int stateNum1 = 1;
	int stateNum2 = 0;
    int ifStateNum = 0;
    int elseStateNum = 0;
    int forkStateNum = 0;
    int vecSize = static_cast<int>(ceil(log2(static_cast<double>(latency+1))));
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
    
    for (int i = 0; i <= latency; ++i)
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
    for (int i = 0; i <= latency; ++i)
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
                                lineStream << lineTabs << "\tState <= S" << stateNum1++ << ";";
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
                            
                            lineStream << "\t\tS" << stateNum2++ << " :";
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
                                    lineStream << lineTabs << "\tState <= FORK" << mapIt->second.at(j).compToLocate.ifStatementLevel << ";";
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
                                
                                lineStream << "\t\tFORK" << mapIt->second.at(j).compToLocate.ifStatementLevel << " :";
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
                                
                                lineStream << lineTabs << "\tState <= IF" << mapIt->second.at(j).compToLocate.ifStatementLevel << ";";
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
                                
                                lineStream << "\t\tIF" << mapIt->second.at(j).compToLocate.ifStatementLevel << " :";
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
                                
                                lineStream << "\t\tELSE" << mapIt->second.at(j).compToLocate.elseStatementLevel << " :";
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

			result = component.result.getCircuitSignalName();
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

			result = component.result.getCircuitSignalName();
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

			result = component.result.getCircuitSignalName();
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

			result = component.result.getCircuitSignalName();
        }
    }
}

void Process::createFinalState(vector<CircuitComp> &components, list<string> &stateMachine)
{
    int vecSize = static_cast<int>(ceil(log2(static_cast<double>(latency+1))));
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

	int *endTime = new int(0);
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
		{
			endTimeSet.insert(it->timeframe[1]);
			//endTime = it->timeframe[1];
		}

        foundEntry = false;
    }

	for (vector<int>::size_type i = 0; i != components.size(); i++)
	{
		if (components.at(i).timeframe[1] > *endTime)
		{;
			*endTime = components.at(i).timeframe[1];
		}
	}
    //set<int>::reverse_iterator etit = endTimeSet.rbegin();
    lineStream << "\t\t\t\tif (CycleCnt == " << vecSize << "'d" << *endTime << ") begin";
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
	if( pos != string::npos)
	{
		verilogFilename = inVerilogFilename.substr(pos+1, inVerilogFilename.length());
	}
	else
	{
		verilogFilename = inVerilogFilename;
	}
}

double Process::addDependentDelaysOneOP(vector<CircuitComp> &circuitCompVector, string op1) //add passing in vector
{
	double addedDelay = 0;
	
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++)
	{
		if (op1 == circuitCompVector.at(i).result.getCircuitSignalName()) //if the current operand was ever a result then this component needs to "wait" for that instantiation
		{
			//set the added delay based on the current component instantiations' data width and type
						//addedDelay = delays.stringToDelay(circuitCompVector.at(i).type, circuitCompVector.at(i).width);
            if(circuitCompVector.at(i).delay > addedDelay)
                addedDelay = circuitCompVector.at(i).delay;
			//cout<<"Found dependency, adding " << addedDelay << "ns" << " Type: " << circuitCompVector.at(i).type << " OP: " << op1 << endl; //TODO Remove DEBUG print
		}
	}
	
	return addedDelay;
}

//NOTE: Also use for COMP since we only need to look at two operands
double Process::addDependentDelaysTwoOP(vector<CircuitComp> &circuitCompVector, string op1, string op2)
{
	double addedDelay = 0;
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++) //check Comp Vector if there's a dependency on op1
	{
		if (op1 == circuitCompVector.at(i).result.getCircuitSignalName()) //if the current operand was ever a result then this component needs to "wait" for that instantiation
		{
			//set the added delay based on the current component instantiations' data width and type
						//addedDelay = delays.stringToDelay(circuitCompVector.at(i).type, circuitCompVector.at(i).width);
            if(circuitCompVector.at(i).delay > addedDelay)
                addedDelay = circuitCompVector.at(i).delay;
			//cout<<"Found dependency, adding " << addedDelay << "ns" << " Type: " << circuitCompVector.at(i).type << " OP: " << op1 << endl; //TODO Remove DEBUG print
		}
	}
	
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++) //check Comp Vector if there's a dependency on op2
	{
		if (op2 == circuitCompVector.at(i).result.getCircuitSignalName()) //if the current operand was ever a result then this component needs to "wait" for that instantiation
		{
			//set the added delay based on the current component instantiations' data width and type
						//addedDelay = delays.stringToDelay(circuitCompVector.at(i).type, circuitCompVector.at(i).width);
           if(circuitCompVector.at(i).delay > addedDelay)
                addedDelay = circuitCompVector.at(i).delay;
			//cout<<"Found dependency, adding " << addedDelay << "ns" <<  " Type: " << circuitCompVector.at(i).type << " OP: " << op2 << endl; //TODO Remove DEBUG print
		}
	}
	
	return addedDelay;
}

double Process::addDependentDelaysMux(vector<CircuitComp> &circuitCompVector, string op1, string op2, string op3)
{
	double addedDelay = 0;
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++) //check Comp Vector if there's a dependency on op1
	{
		if (op1 == circuitCompVector.at(i).result.getCircuitSignalName()) //if the current operand was ever a result then this component needs to "wait" for that instantiation
		{
			//set the added delay based on the current component instantiations' data width and type
						//addedDelay = delays.stringToDelay(circuitCompVector.at(i).type, circuitCompVector.at(i).width);
            if(circuitCompVector.at(i).delay > addedDelay)
                addedDelay = circuitCompVector.at(i).delay;
			//cout<<"Found dependency, adding " << addedDelay << "ns" << " Type: " << circuitCompVector.at(i).type << " OP1: " << op1 << endl; //TODO Remove DEBUG print
		}
	}
	
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++) //check Comp Vector if there's a dependency on op2
	{
		if (op2 == circuitCompVector.at(i).result.getCircuitSignalName()) //if the current operand was ever a result then this component needs to "wait" for that instantiation
		{
			//set the added delay based on the current component instantiations' data width and type
						//addedDelay = delays.stringToDelay(circuitCompVector.at(i).type, circuitCompVector.at(i).width);
            if(circuitCompVector.at(i).delay > addedDelay)
                addedDelay = circuitCompVector.at(i).delay;
			//cout<<"Found dependency, adding " << addedDelay << "ns" << " Type: " << circuitCompVector.at(i).type << " OP2: " << op2 << endl; //TODO Remove DEBUG print
		}
	}
	
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++) //check Comp Vector if there's a dependency on op2
	{
		if (op3 == circuitCompVector.at(i).result.getCircuitSignalName()) //if the current operand was ever a result then this component needs to "wait" for that instantiation
		{
			//set the added delay based on the current component instantiations' data width and type
			//addedDelay = delays.stringToDelay(circuitCompVector.at(i).type, circuitCompVector.at(i).width);
            if(circuitCompVector.at(i).delay > addedDelay)
                addedDelay = circuitCompVector.at(i).delay;
			//cout<<"Found dependency, adding " << addedDelay << "ns" << " Type: " << circuitCompVector.at(i).type << " OP3: " << op3 <<endl; //TODO Remove DEBUG print
		}
	}
	
	return addedDelay;
}

//loops through the Comp vector to find the element with the largest delay time and returns that time
double Process::CalculateCriticalPath(vector<CircuitComp> &circuitCompVector)
{
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++)
	{
		if((circuitCompVector.at(i).type == "INC") || (circuitCompVector.at(i).type == "DEC") || (circuitCompVector.at(i).type == "REG") 
            || (circuitCompVector.at(i).type == "SINC") || (circuitCompVector.at(i).type == "SDEC") || (circuitCompVector.at(i).type == "SREG"))
		{
			string input1, input2;
            
            if(circuitCompVector.at(i).operands.at(0).getCircuitSignalName() != circuitCompVector.at(i).result.getCircuitSignalName())
            {
                input1 = circuitCompVector.at(i).operands.at(0).getCircuitSignalName();
            }
            else
            {
                input1 = circuitCompVector.at(i).operands.at(1).getCircuitSignalName();
            }
			
           // cout << circuitCompVector.at(i).type << " "<< input1 << endl;
			circuitCompVector.at(i).delay += addDependentDelaysOneOP(circuitCompVector, input1);
           // cout << "New delay: " << circuitCompVector.at(i).delay << endl;
		}
		else if((circuitCompVector.at(i).type == "MUX") || (circuitCompVector.at(i).type == "SMUX"))
		{
            string input1, input2, input3;
            
            if(circuitCompVector.at(i).operands.at(0).getCircuitSignalName() != circuitCompVector.at(i).result.getCircuitSignalName())
            {
                input1 = circuitCompVector.at(i).operands.at(0).getCircuitSignalName();
                if(circuitCompVector.at(i).operands.at(1).getCircuitSignalName() != circuitCompVector.at(i).result.getCircuitSignalName())
                {
                    input2 = circuitCompVector.at(i).operands.at(1).getCircuitSignalName();
                    if(circuitCompVector.at(i).operands.at(2).getCircuitSignalName() != circuitCompVector.at(i).result.getCircuitSignalName())
					{
						input3 = circuitCompVector.at(i).operands.at(2).getCircuitSignalName();
					}
					else
					{
						input3 = circuitCompVector.at(i).operands.at(3).getCircuitSignalName();
					}
                }
                else
                {
                    input2 = circuitCompVector.at(i).operands.at(2).getCircuitSignalName();
                    input3 = circuitCompVector.at(i).operands.at(3).getCircuitSignalName();
                }
            }
            else
            {
                input1 = circuitCompVector.at(i).operands.at(1).getCircuitSignalName();
                input2 = circuitCompVector.at(i).operands.at(2).getCircuitSignalName();
                input3 = circuitCompVector.at(i).operands.at(3).getCircuitSignalName();
            }
           // cout << circuitCompVector.at(i).type << " "<< input1 << " "<< input2 << " "<< input3 << endl;
			circuitCompVector.at(i).delay += addDependentDelaysMux(circuitCompVector, input1, 
												input2, input3) ;
           // cout << "New delay: " << circuitCompVector.at(i).delay << endl;                                            
		}
		else
		{
			string input1, input2;
            
            if(circuitCompVector.at(i).operands.at(0).getCircuitSignalName() != circuitCompVector.at(i).result.getCircuitSignalName())
            {
                input1 = circuitCompVector.at(i).operands.at(0).getCircuitSignalName();
                if(circuitCompVector.at(i).operands.at(1).getCircuitSignalName() != circuitCompVector.at(i).result.getCircuitSignalName())
                {
                    input2 = circuitCompVector.at(i).operands.at(1).getCircuitSignalName();
                }
                else
                {
                    input2 = circuitCompVector.at(i).operands.at(2).getCircuitSignalName();
                }
            }
            else
            {
                input1 = circuitCompVector.at(i).operands.at(1).getCircuitSignalName();
                input2 = circuitCompVector.at(i).operands.at(2).getCircuitSignalName();
            }
			
            // cout << circuitCompVector.at(i).type << " "<< input1 << " "<< input2 << endl;
			circuitCompVector.at(i).delay += addDependentDelaysTwoOP(circuitCompVector, input1, input2) ;
           // cout << "New delay: " << circuitCompVector.at(i).delay << endl;
        }
	}
	double maxTime = 0;
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++) 
	{
		if(circuitCompVector.at(i).delay > maxTime)
			maxTime = circuitCompVector.at(i).delay;
	}
	
	return maxTime;
	
}

int Process::stringToEnumType(string datatype)
{
	int enumType = -1;
	
	if ((datatype == "ADD") || (datatype == "SUB") || (datatype == "INC" ) || (datatype == "DEC"))
	{
		enumType = 2;
	}
	else if ((datatype == "MUL"))
	{
		enumType = 0;
	}
	else if ((datatype == "DIV") || (datatype == "MOD"))
	{
		enumType = 1;
	}
	else if ((datatype == "COMP") || (datatype == "MUX") || (datatype == "SHR") || (datatype == "SHL"))
	{
		enumType = 3;
	}
	
	return enumType;
}

int Process::getCycleTime(string datatype)
{
	int cycle = -1;
	
	if ((datatype == "ADD") || (datatype == "SUB") || (datatype == "INC" ) || (datatype == "DEC"))
	{
		cycle = 1;
	}
	else if ((datatype == "MUL"))
	{
		cycle = 2;
	}
	else if ((datatype == "DIV") || (datatype == "MOD"))
	{
		cycle = 3;
	}
	else if ((datatype == "COMP") || (datatype == "MUX") || (datatype == "SHR") || (datatype == "SHL"))
	{
		cycle = 1;
	}
	
	return cycle;
}
