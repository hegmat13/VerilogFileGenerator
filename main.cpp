//
//  main.m
//  dpgen
//
//  Created by Ian Hooks on 10/28/19.
//  Copyright © 2019 Ian Hooks. All rights reserved.
//

#include <stdio.h>
#include "input.hpp"
#include "inputs.hpp"
#include "Outputs.hpp"
#include "Wires.hpp"
#include "Equations.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	// console out the names of the files coming in from the terminal 
    cout << "Argument 1 is : " << argv[1] << endl;
    cout << "Argument 2 is : " << argv[2] << endl;
	//
	/*
	char* inputfile; 
	strcpy(inputfile, "474a_circuit1.txt"); 
	char* outputfile;
	strcpy(outputfile, "output.v"); */

	// argv[1] = const_cast<char*>("474a_circuit4.txt");//hard code files for debudding
	// argv[2] = const_cast<char*>("output.v");//comment out to run from the terminal
	// int latency = (int)argv[2]; 

	verilogSim sim;
	sim.run(argv[1], argv[2]);
	sim.ReadCommandsFromFile();
	int valid = sim.TestValid();
	if (valid == 1) {
		cout << "Error: There is an invalid operator in your equations" << endl;

	}
	else if (valid == 2) {
		cout << "Error: One of the variables used in the equations is undeclared" << endl; 
	}
	else if (valid == 0) {
		sim.WriteCommandsToFile();
	}
	//return 0;
	//}

	//JUST IGNORE ALL THIS THESE WERE TRIAL AND ERROR TESTS

//    int counter;
//    printf("Program Name Is: %s",argv[0]);
//    if(argc==1)
//        printf("\nNo Extra Command Line Argument Passed Other Than Program Name");
//    if(argc>=2)
//    {
//        printf("\nNumber Of Arguments Passed: %d",argc);
//        printf("\n----Following Are The Command Line Arguments Passed----");
//        for(counter=0;counter<argc;counter++)
//            printf("\nargv[%d]: %s",counter,argv[counter]);
//    }

	//stringstream inputFile;
	//inputFile << argv[1]; //fix for commmand line entry
	//inputFile << "474a_circuit1.txt";
	//stringstream outputFile;
	//outputFile << argv[2]; //fix for commmand line entry
	//outputFile << "verilogFile";
	//ifstream inputFileStream;
	//ifstream outputFileStream;
	//inputFileStream.open(inputFile.str());
	//outputFileStream.open(outputFile.str());

//    string test1, test2;
//    string test3, test4;
//    inputFileStream >> test1 >> test2 >> test3 >> test4;

	return 0;
}