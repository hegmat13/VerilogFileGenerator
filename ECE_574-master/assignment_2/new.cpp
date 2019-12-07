#include "Process.h"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "CircuitSignal.h"

using namespace std;

Process::Process()
{
}

Process::~Process()
{
}

int Process::parseNetlist(ifstream &netList, ofstream &verilogFile)
{
	string comment =  "//";
	CircuitSignal circuitSignal;
	vector<CircuitSignal> circuitVector;

	for (string line; getline(netList, line); )
	{
		// first we will remove all comments from the line
		cout << line << endl;
		string::size_type commentIndex = line.find(comment);

		if (commentIndex != string::npos)
			line.erase(commentIndex, line.length());

		cout << line << endl;

		if(line.find("input") != string::npos)
		{
			stringstream lineStream(line);
			string signal, dataType, variable;
			//string dataWidth

			lineStream >> signal >> dataType;
		//	dataWidth = parseDataWidth(dataType); //pass dataType string into parse function
		//	linestrean >> variable; //continue 

			circuitSignal.setCircuitSignalType(signal);
			circuitSignal.setCircuitSignalDataType(dataType);
			circuitSignal.setCircuitSignalName(variable);
		//	circuitSignal.setCircuitSignalWidth(dataWidth) //also need to change function parameter to string from int

			circuitVector.push_back(circuitSignal);
			cout << "Signal is: " << circuitVector.front().getCircuitSignalType() << " DataType is: " << circuitVector.front().getCircuitSignalDataType() << " Variable is: " << variable << endl;
		}
		if(line.find("output") != string::npos)
		{
			stringstream lineStream(line);
			string signal, dataType, variable;

			lineStream >> signal >> dataType >> variable;

			circuitSignal.setCircuitSignalType(signal);
			circuitSignal.setCircuitSignalDataType(dataType);
			circuitSignal.setCircuitSignalName(variable);

			circuitVector.push_back(circuitSignal);
			cout << "Signal is: " << circuitVector.at(1).getCircuitSignalType() << " DataType is: " << circuitVector.at(1).getCircuitSignalDataType() << " Variable is: " << variable << endl;

		}
		if(line.find("wire") != string::npos)
		{
			stringstream lineStream(line);
			string signal, dataType, variable;

			lineStream >> signal >> dataType >> variable;

			circuitSignal.setCircuitSignalType(signal);
			circuitSignal.setCircuitSignalDataType(dataType);
			circuitSignal.setCircuitSignalName(variable);

			circuitVector.push_back(circuitSignal);
			cout << "Signal is: " << circuitVector.at(2).getCircuitSignalType() << " DataType is: " << circuitVector.at(2).getCircuitSignalDataType() << " Variable is: " << variable << endl;

		}
		else
		{

		}
	}

	return 0;
}


int parseDataWidth (string &widthString)
{
    std::string width = 0;
	
	stize_t pos = withString.find("Int");
    
	width = widthString.substr(pos+2, widthString.end()); //create string which represents the data width after "Int"
	
	widthString = widthString.substr(widthString.begin(),pos+2); // change widthString to be just "Int" or "UInt" need to test
	
	return width;

}