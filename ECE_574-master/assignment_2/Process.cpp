#include "Process.h"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "getValue.h"
#include "dpgen.h"

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
	
	for (string line; getline(netList, line); )
	{
		// first we will remove all comments from the line
		string::size_type commentIndex = line.find(comment);

		if (commentIndex != string::npos)
			line.erase(commentIndex, line.length());
		
		if(((line.find("input") != string::npos) || (line.find("output") != string::npos) || ((line.find("wire") != string::npos) && (line.find("=") == string::npos))
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
					cout << line << endl;
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
				//REG?
				else
				{	
					cout << line << endl;
					status = parseOneOperand(circuitVector, circuitCompVector, lineStream, "REG");
				}
				
				
				if (status != SUCCESS)
				{
					return status;
				}
				//writeToFile();
			}
		}
	}
	
    // TODO: if (!some error code) {
    status = writeDeclarations(circuitVector, verilogFile);
    status = writeInstantiations(circuitCompVector, verilogFile);
    // TODO: }

	cout << "Final: " << CalculateCiritcalPath(circuitCompVector) << endl;
	
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
	
	// loop through entire circuit signal vector looking for any input/output/wire/reg within the 
	// component instantiation 
	// if you never found one of the values error out
	int numOperandsFound = 0;
	for (std::vector<int>::size_type i = 0; i != circuitVector.size(); i++)
	{					
		if((circuitVector.at(i).getCircuitSignalType() == "input") || (circuitVector.at(i).getCircuitSignalType() == "wire") 
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
	
	// loop through entire circuit signal vector looking for any input/output/wire/reg within the 
	// component instantiation 
	// if you never found one of the values error out
	int numOperandsFound = 0;
	for (std::vector<int>::size_type i = 0; i != circuitVector.size(); i++)
	{					
		if((circuitVector.at(i).getCircuitSignalType() == "input") || (circuitVector.at(i).getCircuitSignalType() == "wire") 
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
				circuitComp.delay += addDependentDelaysTwoOP(circuitCompVector, op1, op2); //TEST of new function
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
		circuitCompVector.push_back(circuitComp);
		
		return SUCCESS;
	}
}

int Process::parseComp(vector<CircuitSignal> &circuitVector, vector<CircuitComp> &circuitCompVector, stringstream &lineStream, string type)
{
	CircuitComp circuitComp;
	string result, equal, op1, op, op2;
				
	lineStream >> result >> equal >> op1 >> op >> op2;
	
	circuitComp.type = type;
	
	// loop through entire circuit signal vector looking for any input/output/wire/reg within the 
	// component instantiation 
	// if you never found one of the values error out
	int numOperandsFound = 0;
	for (std::vector<int>::size_type i = 0; i != circuitVector.size(); i++)
	{					
		if((circuitVector.at(i).getCircuitSignalType() == "input") || (circuitVector.at(i).getCircuitSignalType() == "wire") 
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
				circuitComp.operands.push_back(circuitVector.at(i));
				//found output/wire
			
			
				if( op == ">")
				{
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
	
	// loop through entire circuit signal vector looking for any input/output/wire/reg within the 
	// component instantiation 
	// if you never found one of the values error out
	int numOperandsFound = 0;
	for (std::vector<int>::size_type i = 0; i != circuitVector.size(); i++)
	{					
		if((circuitVector.at(i).getCircuitSignalType() == "input") || (circuitVector.at(i).getCircuitSignalType() == "wire") 
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
		circuitCompVector.push_back(circuitComp);
		
		return SUCCESS;
	}
}

int Process::writeDeclarations(vector<CircuitSignal> &signals, ofstream &verilogFile)
{
    int status = -1;
    int temp;
    stringstream lineStream;
    string moduleName;
    string signalWidth;
    string regType;
    string::size_type pos = verilogFilename.find(".v");

	cout << verilogFilename << endl;
    lineStream << "`timescale 1ns / 1ns";
    verilogFile << lineStream.rdbuf() << "\n\n";
    // declare module: module verilogFilename(variable, variable...);
    moduleName = verilogFilename.erase(pos, verilogFilename.length());
    lineStream << "module " << moduleName << "(Clk, Rst";
    for (vector<CircuitSignal>::iterator it = signals.begin(); it != signals.end(); ++it)
    {
        if ((it->getCircuitSignalType() == "input") || (it->getCircuitSignalType() == "output"))
        {
            lineStream << ", " << it->getCircuitSignalName();
        }
    }
    lineStream << ");";
    verilogFile << lineStream.rdbuf() << '\n';
    lineStream.str("");

    // Now go through CircuitSignal vector and write signal declarations
    for (vector<CircuitSignal>::iterator it = signals.begin(); it != signals.end(); ++it)
    {
        if (it->getCircuitSignalType() != "register")
        {
            lineStream << "\t" << it->getCircuitSignalType() << " ";
        }
        else
        {
            lineStream << "\twire ";
        }

        if (it->getCircuitSignalDataType() == "Int")
        {
            lineStream << "signed ";
        }

        temp = stoi(it->getCircuitSignalWidth(), nullptr, 10);
        if (temp != 1)
        {
            --temp;
            signalWidth = to_string(temp);
            lineStream << "[" << signalWidth << " : 0] ";
        }

        lineStream << it->getCircuitSignalName() << ";";
        verilogFile << lineStream.rdbuf() << '\n';
        lineStream.str("");
    }

    status = 1;
    return status;
}

int Process::writeInstantiations(vector<CircuitComp> &components, ofstream &verilogFile)
{
    // TODO: Write Code
    int status = -1;
    stringstream lineStream;

    // Iterate through CircuitComp vector to write each component to verilogFile
    for (vector<int>::size_type i = 0; i != components.size(); i++)
    {
        // Construct current component
        lineStream << components.at(i).type << " #(.DATAWIDTH(" << components.at(i).width << ") inst" << i;
        if (components.at(i).type.find("ADD") != string::npos)
        {
            string op1, op2, result;

            getOperands(components, i, result, op1, op2);
            lineStream << "(.a(" << op1 << "), .b(" << op2 << "), .sum(" << result << "));";
        }
        if (components.at(i).type.find("SUB") != string::npos)
        {
            string op1, op2, result;

            getOperands(components, i, result, op1, op2);
            lineStream << "(.a(" << op1 << "), .b(" << op2 << "), .diff(" << result << "));";
        }
        if (components.at(i).type.find("MUL") != string::npos)
        {
            string op1, op2, result;

            getOperands(components, i, result, op1, op2);
            lineStream << "(.a(" << op1 << "), .b(" << op2 << "), .prod(" << result << "));";
        }
        if (components.at(i).type.find("SHR") != string::npos)
        {
            string op1, op2, result;

            getOperands(components, i, result, op1, op2);
            lineStream << "(.a(" << op1 << "), .sh_amt(" << op2 << "), .d(" << result << "));";
        }
        if (components.at(i).type.find("SHL") != string::npos)
        {
            string op1, op2, result;

            getOperands(components, i, result, op1, op2);
            lineStream << "(.a(" << op1 << "), .sh_amt(" << op2 << "), .d(" << result << "));";
        }
        if (components.at(i).type.find("DIV") != string::npos)
        {
            string op1, op2, result;

            getOperands(components, i, result, op1, op2);
            lineStream << "(.a(" << op1 << "), .b(" << op2 << "), .quot(" << result << "));";
        }
        if (components.at(i).type.find("MOD") != string::npos)
        {
            string op1, op2, result;

            getOperands(components, i, result, op1, op2);
            lineStream << "(.a(" << op1 << "), .b(" << op2 << "), .rem(" << result << "));";
        }
        if (components.at(i).type.find("REG") != string::npos)
        {
            string op1, result;

            getOperands(components, i, result, op1);
            lineStream << "(.d(" << op1 << "), .Clk(Clk), .Rst(Rst), .q(" << result << "));";
        }
        if (components.at(i).type.find("INC") != string::npos)
        {
            string op1, result;

            getOperands(components, i, result, op1);
            lineStream << "(.a(" << op1 << "), .d(" << result << "));";
        }
        if (components.at(i).type.find("DEC") != string::npos)
        {
            string op1, result;

            getOperands(components, i, result, op1);
            lineStream << "(.a(" << op1 << "), .d(" << result << "));";
        }
        if (components.at(i).type.find("MUX") != string::npos)
        {
            string sel, op1, op2, result;

            getOperands(components, i, result, op1, op2, sel);
            lineStream << "(.a(" << op1 << "), .b(" << op2 << "), .sel(" << sel << "), .d(" << result << "));";
        }
        if (components.at(i).type.find("COMP") != string::npos)
        {
            string op1, op2, result;
            int resultType;

            getOperands(components, i, result, op1, op2, resultType);
            lineStream << "(.a(" << op1 << "), .b(" << op2 << "), .gt(";
            if (resultType == GT)
                lineStream << result;
            lineStream << "), .lt(";
            if (resultType == LT)
                lineStream << result;
            lineStream << "), .eq(";
            if (resultType == EQ)
                lineStream << result;
            lineStream << "));";
        }

        verilogFile << lineStream.rdbuf() << '\n';
        lineStream.str("");
    }

    lineStream << "endmodule";
    verilogFile << lineStream.rdbuf() << '\n';
    lineStream.str("");

	status = 1;
    return status;
}

void Process::setVerilogFilename(const char *vFilename)
{
    string inVerilogFilename(vFilename);
	verilogFilename = inVerilogFilename;
}

void Process::getOperands(vector<CircuitComp> &components, vector<int>::size_type &i, string &result, string &op1)
{
    for (vector<int>::size_type j = 0; j != components.at(i).operands.size(); j++)
    {
        if (components.at(i).operands.at(j).getCircuitSignalPosition().find("op1") != string::npos)
            op1 = components.at(i).operands.at(j).getCircuitSignalName();
        if (components.at(i).operands.at(j).getCircuitSignalPosition().find("result") != string::npos)
            result = components.at(i).operands.at(j).getCircuitSignalName();
    }
}

void Process::getOperands(vector<CircuitComp> &components, vector<int>::size_type &i, string &result, string &op1, string &op2)
{
    vector<int>::size_type op1_pos, op2_pos, result_pos;
    int op1_width, op2_width, result_width, difference;
    bool op1_signed, op2_signed, result_signed;

    // Store operand positions, widths, and signs
    for (vector<int>::size_type j = 0; j != components.at(i).operands.size(); j++)
    {
        if (components.at(i).operands.at(j).getCircuitSignalPosition().find("op1") != string::npos)
        {
            op1 = components.at(i).operands.at(j).getCircuitSignalName();
            op1_pos = j;
            op1_width = stoi(components.at(i).operands.at(j).getCircuitSignalWidth());
            if (components.at(i).operands.at(j).getCircuitSignalDataType().find('U') != string::npos)
            {
                op1_signed = false;
            }
            else
            {
                op1_signed = true;
            }
        }
        if (components.at(i).operands.at(j).getCircuitSignalPosition().find("op2") != string::npos)
        {
            op2 = components.at(i).operands.at(j).getCircuitSignalName();
            op2_pos = j;
            op2_width = stoi(components.at(i).operands.at(j).getCircuitSignalWidth());
            if (components.at(i).operands.at(j).getCircuitSignalDataType().find('U') != string::npos)
            {
                op2_signed = false;
            }
            else
            {
                op2_signed = true;
            }
        }
        if (components.at(i).operands.at(j).getCircuitSignalPosition().find("result") != string::npos)
        {
            result = components.at(i).operands.at(j).getCircuitSignalName();
            result_pos = j;
            result_width = stoi(components.at(i).operands.at(j).getCircuitSignalWidth());
            if (components.at(i).operands.at(j).getCircuitSignalDataType().find('U') != string::npos)
            {
                result_signed = false;
            }
            else
            {
                result_signed = true;
            }
        }
    }
    
    // Adjust operand widths as needed : op1
    if (op1_width > result_width)
    {
		if((components.at(i).type != "SHL") && (components.at(i).type != "SSHL") && (components.at(i).type != "SHR") && (components.at(i).type != "SSHR"))
		{
			// Truncate the operand
			op1 = op1 + '[' + to_string(result_width - 1) + " : 0]";
		}
    }
    if (op1_width < result_width)
    {
		if((components.at(i).type != "SHL") && (components.at(i).type != "SSHL") && (components.at(i).type != "SHR") && (components.at(i).type != "SSHR"))
		{
			// Extend the operand
			difference = result_width - op1_width;
			if (op1_signed)
			{
				// Extend sign bit
				op1 = "{{" + to_string(difference) + "{" + op1 + "[" + to_string(op1_width - 1) + "]}}, " + op1 + "}";
			}
			else
			{
				// Pad with 0's
				op1 = "{{" + to_string(difference) + "{1'b0}}, " + op1 + "}";
			}
		}
    }

    // Typecast unsigned to signed
    if (result_signed && !op1_signed)
        op1 = "$signed(" + op1 + ")";

    // Adjust operand widths as needed : op2
    if (op2_width > result_width)
    {
		if((components.at(i).type != "SHL") && (components.at(i).type != "SSHL") && (components.at(i).type != "SHR") && (components.at(i).type != "SSHR"))
		{
			op2 = op2 + '[' + to_string(result_width - 1) + " : 0]";
		}
    }
    if (op2_width < result_width)
    {
		if((components.at(i).type != "SHL") && (components.at(i).type != "SSHL") && (components.at(i).type != "SHR") && (components.at(i).type != "SSHR"))
		{
			difference = result_width - op2_width;
			if (op2_signed)
			{
				op2 = "{{" + to_string(difference) + "{" + op2 + "[" + to_string(op2_width - 1) + "]}}, " + op2 + "}";
			}
			else
			{
				op2 = "{{" + to_string(difference) + "{1'b0}}, " + op2 + "}";
			}
		}
    }

    if (result_signed && !op2_signed)
        op2 = "$signed(" + op2 + ")";
}

void Process::getOperands(vector<CircuitComp> &components, vector<int>::size_type &i, string &result, string &op1, string &op2, string &sel)
{
    for (vector<int>::size_type j = 0; j != components.at(i).operands.size(); j++)
    {
        if (components.at(i).operands.at(j).getCircuitSignalPosition().find("op1") != string::npos)
            op1 = components.at(i).operands.at(j).getCircuitSignalName();
        if (components.at(i).operands.at(j).getCircuitSignalPosition().find("op2") != string::npos)
            op2 = components.at(i).operands.at(j).getCircuitSignalName();
        if (components.at(i).operands.at(j).getCircuitSignalPosition().find("sel") != string::npos)
            sel = components.at(i).operands.at(j).getCircuitSignalName();
        if (components.at(i).operands.at(j).getCircuitSignalPosition().find("result") != string::npos)
            result = components.at(i).operands.at(j).getCircuitSignalName();
    }
}

void Process::getOperands(vector<CircuitComp> &components, vector<int>::size_type &i, string &result, string &op1, string &op2, int &resultType)
{
    vector<int>::size_type op1_pos, op2_pos, result_pos;
    int op1_width, op2_width, difference;
    bool op1_signed, op2_signed, result_signed;

    // Store operand positions, widths, and signs
    for (vector<int>::size_type j = 0; j != components.at(i).operands.size(); j++)
    {
        if (components.at(i).operands.at(j).getCircuitSignalPosition().find("op1") != string::npos)
        {
            op1 = components.at(i).operands.at(j).getCircuitSignalName();
            op1_pos = j;
            op1_width = stoi(components.at(i).operands.at(j).getCircuitSignalWidth());
            if (components.at(i).operands.at(j).getCircuitSignalDataType().find('U') != string::npos)
            {
                op1_signed = false;
            }
            else
            {
                op1_signed = true;
            }
        }
        if (components.at(i).operands.at(j).getCircuitSignalPosition().find("op2") != string::npos)
        {
            op2 = components.at(i).operands.at(j).getCircuitSignalName();
            op2_pos = j;
            op2_width = stoi(components.at(i).operands.at(j).getCircuitSignalWidth());
            if (components.at(i).operands.at(j).getCircuitSignalDataType().find('U') != string::npos)
            {
                op2_signed = false;
            }
            else
            {
                op2_signed = true;
            }
        }
        if (components.at(i).operands.at(j).getCircuitSignalPosition().find("op3") != string::npos)
        {
            result = components.at(i).operands.at(j).getCircuitSignalName();
            resultType = GT;
            result_pos = j;
            if (components.at(i).operands.at(j).getCircuitSignalDataType().find('U') != string::npos)
            {
                result_signed = false;
            }
            else
            {
                result_signed = true;
            }
        }
        if (components.at(i).operands.at(j).getCircuitSignalPosition().find("op4") != string::npos)
        {
            result = components.at(i).operands.at(j).getCircuitSignalName();
            resultType = LT;
            result_pos = j;
            if (components.at(i).operands.at(j).getCircuitSignalDataType().find('U') != string::npos)
            {
                result_signed = false;
            }
            else
            {
                result_signed = true;
            }
        }
        if (components.at(i).operands.at(j).getCircuitSignalPosition().find("op5") != string::npos)
        {
            result = components.at(i).operands.at(j).getCircuitSignalName();
            resultType = EQ;
            result_pos = j;
            if (components.at(i).operands.at(j).getCircuitSignalDataType().find('U') != string::npos)
            {
                result_signed = false;
            }
            else
            {
                result_signed = true;
            }
        }
    }

    // Adjust operand widths as needed: op1
    if (op2_width > op1_width)
    {
		if((components.at(i).type != "SHL") && (components.at(i).type != "SSHL") && (components.at(i).type != "SHR") && (components.at(i).type != "SSHR"))
		{
			// Extend the operand
			difference = op2_width - op1_width;
			if (op1_signed)
			{
				// Extend sign bit
				op1 = "{{" + to_string(difference) + "{" + op1 + "[" + to_string(op1_width - 1) + "]}}, " + op1 + "}";
			}
			else
			{
				// Pad with 0's
				op1 = "{{" + to_string(difference) + "{1'b0}}, " + op1 + "}";
			}
		}
    }

    // Typecast unsigned to signed
    if (result_signed && !op1_signed)
        op1 = "$signed(" + op1 + ")";

    // Adjust operand widths as needed: op2
    if (op1_width > op2_width)
    {
		if((components.at(i).type != "SHL") && (components.at(i).type != "SSHL") && (components.at(i).type != "SHR") && (components.at(i).type != "SSHR"))
		{
			// Extend the operand
			difference = op1_width - op2_width;
			if (op2_signed)
			{
				// Extend sign bit
				op2 = "{{" + to_string(difference) + "{" + op2 + "[" + to_string(op2_width - 1) + "]}}, " + op2 + "}";
			}
			else
			{
				// Pad with 0's
				op2 = "{{" + to_string(difference) + "{1'b0}}, " + op2 + "}";
			}
		}
    }

    // Typecast unsigned to signed
    if (result_signed && !op2_signed)
        op2 = "$signed(" + op2 + ")";
}

double Process::addDependentDelaysOneOP(vector<CircuitComp> &circuitCompVector, string op1) //add passing in vector
{
	double addedDelay = 0;
	
	for (int i = 0; i != circuitCompVector.size(); i++)
	{
		if (op1 == circuitCompVector.at(i).result) //if the current operand was ever a result then this component needs to "wait" for that instantiation
		{
			//set the added delay based on the current component instantiations' data width and type
			addedDelay += delays.stringToDelay(circuitCompVector.at(i).type, circuitCompVector.at(i).width);
			cout<<"Found dependency, adding " << addedDelay << "ns" <<  << " Type: " << circuitCompVector.at(i).type << endl; //TODO Remove DEBUG print
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
		if (op1 == circuitCompVector.at(i).result) //if the current operand was ever a result then this component needs to "wait" for that instantiation
		{
			//set the added delay based on the current component instantiations' data width and type
			addedDelay += delays.stringToDelay(circuitCompVector.at(i).type, circuitCompVector.at(i).width);
			cout<<"Found dependency, adding " << addedDelay << "ns" <<  << " Type: " << circuitCompVector.at(i).type << endl; //TODO Remove DEBUG print
		}
	}
	
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++) //check Comp Vector if there's a dependency on op2
	{
		if (op2 == circuitCompVector.at(i).result) //if the current operand was ever a result then this component needs to "wait" for that instantiation
		{
			//set the added delay based on the current component instantiations' data width and type
			addedDelay += delays.stringToDelay(circuitCompVector.at(i).type, circuitCompVector.at(i).width);
			cout<<"Found dependency, adding " << addedDelay << "ns" << " Type: " << circuitCompVector.at(i).type << endl; //TODO Remove DEBUG print
		}
	}
	
	return addedDelay;
}

//loops through the Comp vector to find the element with the largest delay time and returns that time
double Process::CalculateCiritcalPath(vector<CircuitComp> &circuitCompVector)
{
	double maxTime = 0;
	for (vector<int>::size_type i = 0; i != circuitCompVector.size(); i++) 
	{
		if(circuitCompVector.at(i).delay > maxTime)
			maxTime = circuitCompVector.at(i).delay;
	}
	
	return maxTime;
	
}
