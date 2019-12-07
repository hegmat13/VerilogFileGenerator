/*int main () {
  ofstream myfile;
  myfile.open ("argv[1] argv[2]");
  myfile << "Writing this to a file.\n";
  myfile.close();
  return 0;
}*/

#include <fstream>
#include <iostream>
#include <algorithm>
#include "Process.h"

using namespace std;

void Process::Run(string input, string output) {
	//Read in Circuits
	ReadstartCircuitsFromFile(input);
	
	//Code needed here

	
	//Write Circuits to File
	WriteCircuitsToFile(output);

	//Success
}

void Process::ReadCircuitsFromFile(string inputFile)
{
	ifstream inStream;
	string input;

	inStream.open(inputFile);
	if (!inStream.is_open())
	{
		cout << "Could not open file " << inputFile << endl;
		return;
	}

	while (!inStream.eof())
	{

		if (inStream.good())
		{
			getline(inStream, input);
			
		}
		else
		{
			inStream.clear();
			inStream.ignore(numeric_limits<streamsize>::max(), '\n');
		}

	}
	inStream.close();


	return;
}

void Process::WriteCircuitsToFile() {
	ofstream outStream;
	outStream.open(_outputFilePath);
	if (!outStream.is_open())
	{
		cout << "Could not open file " << _outputFilePath << endl;
		return;
	}

	
	//This has to be rewritten for outputting circuit lines but keeping this as an example
	/*
	for (std::list<Card>::iterator it = _cards.begin(); it != _cards.end(); ++it)
	{
		outStream << it->GetContent() << "\r\n" << endl;
	}*/


	outStream.close();
}