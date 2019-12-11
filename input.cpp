//
//  input.cpp
//  dpgen
//
//  Created by Ian Hooks on 10/29/19.
//  Copyright © 2019 Ian Hooks. All rights reserved.
//
//hi
#include "input.hpp"
#include "inputs.hpp"
#include "Outputs.hpp"
#include "Wires.hpp"
#include "Equations.hpp"
#include "Registers.hpp"
#include <string>
#include <algorithm>
#include <ostream> 
#include <fstream>

using namespace std; 
 
void verilogSim::run(char* inputFileName, char* outputFileName) {
	_inputFileName = inputFileName;
	_outputFileName = outputFileName;
	//_latency = latency; 
}

void verilogSim::ReadCommandsFromFile() {

	ostringstream inputFile;
	inputFile << _inputFileName;
	std::string inputName = inputFile.str(); 

	fstream inputFileStream;
	//string input =  inputFile.str(); 
	inputFileStream.open(inputName); 
	//inputFileStream.open(input);

	string line;
	string iow;
	string type;
	string check1, check2, check3, check4, check5, check6, check7, check8;
	string variable, stuff, more, mux2, colon, sel;
	string extra = "0";
	char firstLetter;
	char firstNum;
	int len;

	if (!inputFileStream.is_open()) {
		cout << "did not open input\n";
		return;
	}

	while (!inputFileStream.eof()) {
		getline(inputFileStream, line);
		std::istringstream lineStream(line);
		extra = "";
		lineStream >> iow >> type >> variable >> extra;
		unsigned long int x = extra.length();

		cout << iow << type << variable << extra;

		cout << endl;
		cout << "iow: " << iow << endl;
		cout << "type: " << type << endl;
		cout << "variable: " << variable << endl;
		cout << "extra: " << extra.length() << endl;

		if ((iow.at(0) != '/')) {   //As long as the line isn't commented out 

			if (extra != "") {
				if (extra.at(extra.length() - 1) == ',') {
					extra.erase(extra.length() - 1);  //Added This 
					//extra.at(extra.length()-1) =  ' ';
				}
			}
			if (variable != "") {
				if (variable.at(variable.length() - 1) == ',') {
					variable.erase(variable.length() - 1); //Added This
					//variable.at(variable.length()-1) =  ' ';   

				}
			}
			firstLetter = type.at(0);
			len = type.length();

			//INPUTS
			if (line != "") {
				if ((iow == "input") && ((iow != check1) || (type != check2))) {
					Inputs temp;
					temp.SetVariableI(variable);
		//			temp.SetTimeSlotASAPI(0); 
			//		temp.SetTimeSlotALAPI(_latency); 

					if (firstLetter == 'I') {

						temp.SetDataTypeI('I');
						firstNum = type.at(3);

						if ((firstNum == '1') && (len == 4)) {
							temp.SetDataWidthI(1);
						}
						else if (firstNum == '2') {
							temp.SetDataWidthI(2);
						}
						else if (firstNum == '4') {
							temp.SetDataWidthI(4);
						}
						else if (firstNum == '8') {
							temp.SetDataWidthI(8);
						}
						else if ((firstNum == '1') && (len == 5)) {
							temp.SetDataWidthI(16);
						}
						else if (firstNum == '3') {
							temp.SetDataWidthI(32);
						}
						else if (firstNum == '6') {
							temp.SetDataWidthI(64);
						}
						_inputs.push_back(temp);
					}//end of signed int

					else if (firstLetter == 'U') {
						temp.SetDataTypeI('U');
						firstNum = type.at(4);
						if ((firstNum == '1') && (len == 5)) {
							temp.SetDataWidthI(1);
						}
						else if (firstNum == '2') {
							temp.SetDataWidthI(2);
						}
						else if (firstNum == '4') {
							temp.SetDataWidthI(4);
						}
						else if (firstNum == '8') {
							temp.SetDataWidthI(8);
						}
						else if ((firstNum == '1') && (len == 6)) {
							temp.SetDataWidthI(16);
						}
						else if (firstNum == '3') {
							temp.SetDataWidthI(32);
						}
						else if (firstNum == '6') {
							temp.SetDataWidthI(64);
						}
						_inputs.push_back(temp);
					}//end of unsigned int input
					check1 = iow;
					check2 = type;

					while (extra != "") {
						temp.SetVariableI(extra);
						_inputs.push_back(temp);
						lineStream >> extra;

						cout << extra;

						if (extra.at(extra.length() - 1) == ',') {
							extra.erase(extra.length() - 1);
						}

						if (extra == temp.GetVariableI() || (extra.at(0) == '/')) {
							break;
						}
					}
				}// end of all inputs





				//OUTPUTS
				if ((iow == "output") && ((iow != check3) || (type != check4))) {
					Outputs temp;
					temp.SetVariableO(variable);
				//	temp.SetTimeSlotASAPO(0);
				//	temp.SetTimeSlotALAPO(_latency);

					if (firstLetter == 'I') {

						temp.SetDataTypeO('I');
						firstNum = type.at(3);

						if ((firstNum == '1') && (len == 4)) {
							temp.SetDataWidthO(1);
						}
						else if (firstNum == '2') {
							temp.SetDataWidthO(2);
						}
						else if (firstNum == '4') {
							temp.SetDataWidthO(4);
						}
						else if (firstNum == '8') {
							temp.SetDataWidthO(8);
						}
						else if ((firstNum == '1') && (len == 5)) {
							temp.SetDataWidthO(16);
						}
						else if (firstNum == '3') {
							temp.SetDataWidthO(32);
						}
						else if (firstNum == '6') {
							temp.SetDataWidthO(64);
						}
						_outputs.push_back(temp);
					}//end of signed int

					else if (firstLetter == 'U') {
						temp.SetDataTypeO('U');
						firstNum = type.at(4);
						if ((firstNum == '1') && (len == 5)) {
							temp.SetDataWidthO(1);
						}
						else if (firstNum == '2') {
							temp.SetDataWidthO(2);
						}
						else if (firstNum == '4') {
							temp.SetDataWidthO(4);
						}
						else if (firstNum == '8') {
							temp.SetDataWidthO(8);
						}
						else if ((firstNum == '1') && (len == 6)) {
							temp.SetDataWidthO(16);
						}
						else if (firstNum == '3') {
							temp.SetDataWidthO(32);
						}
						else if (firstNum == '6') {
							temp.SetDataWidthO(64);
						}
						_outputs.push_back(temp);
					}//end of unsigned int input
					check3 = iow;
					check4 = type;

					while (extra != "") {

						/*added this to remove commas from wire Variable names*/
						if (extra.at(extra.length() - 1) == ',') {
							extra.erase(extra.length() - 1);
						}

						temp.SetVariableO(extra);
						_outputs.push_back(temp);
						lineStream >> extra;
						if (extra == temp.GetVariableO() || (extra.at(0) == '/')) {
							break;
						}
					}
				}// end o

				//REGISTERS
				/*
				if ((iow == "register") && ((iow != check7) || (type != check8))) {
					Registers temp;
					temp.SetVariableR(variable);

					if (firstLetter == 'I') {

						temp.SetDataTypeR('I');
						firstNum = type.at(3);

						if ((firstNum == '1') && (len == 4)) {
							temp.SetDataWidthR(1);
						}
						else if (firstNum == '2') {
							temp.SetDataWidthR(2);
						}
						else if (firstNum == '4') {
							temp.SetDataWidthR(4);
						}
						else if (firstNum == '8') {
							temp.SetDataWidthR(8);
						}
						else if ((firstNum == '1') && (len == 5)) {
							temp.SetDataWidthR(16);
						}
						else if (firstNum == '3') {
							temp.SetDataWidthR(32);
						}
						else if (firstNum == '6') {
							temp.SetDataWidthR(64);
						}
						_registers.push_back(temp);
					}//end of signed int

					else if (firstLetter == 'U') {
						temp.SetDataTypeR('U');
						firstNum = type.at(4);
						if ((firstNum == '1') && (len == 5)) {
							temp.SetDataWidthR(1);
						}
						else if (firstNum == '2') {
							temp.SetDataWidthR(2);
						}
						else if (firstNum == '4') {
							temp.SetDataWidthR(4);
						}
						else if (firstNum == '8') {
							temp.SetDataWidthR(8);
						}
						else if ((firstNum == '1') && (len == 6)) {
							temp.SetDataWidthR(16);
						}
						else if (firstNum == '3') {
							temp.SetDataWidthR(32);
						}
						else if (firstNum == '6') {
							temp.SetDataWidthR(64);
						}
						_registers.push_back(temp);
					}//end of unsigned int input
					check7 = iow;
					check8 = type;
					while (extra != "") {
						temp.SetVariableR(extra);
						_registers.push_back(temp);
						lineStream >> extra;
						if (extra == temp.GetVariableR()) {
							break;
						}
					}
				}//end registers */

				//WIRES
				if ((iow == "variable") && ((iow != check5) || (type != check6))) {
					Wires temp;
					temp.SetVariableW(variable);
				//	temp.SetTimeSlotASAPW(0);
				//	temp.SetTimeSlotALAPW(_latency);



					if (firstLetter == 'I') {
						//COMMENT
						//cout << "entered data width loop for signed" << endl;

						temp.SetDataTypeW('I');
						firstNum = type.at(3);

						if ((firstNum == '1') && (len == 4)) {
							temp.SetDataWidthW(1);
						}
						else if (firstNum == '2') {
							temp.SetDataWidthW(2);
						}
						else if (firstNum == '4') {
							temp.SetDataWidthW(4);
						}
						else if (firstNum == '8') {
							temp.SetDataWidthW(8);
						}
						else if ((firstNum == '1') && (len == 5)) {
							temp.SetDataWidthW(16);
						}
						else if (firstNum == '3') {
							temp.SetDataWidthW(32);
						}
						else if (firstNum == '6') {
							temp.SetDataWidthW(64);
						}
						_wires.push_back(temp);
					}//end of signed int

					else if (firstLetter == 'U') {
						//COMMENT
						//cout << "entered data width loop for UNsigned" << endl;

						temp.SetDataTypeW('U');
						firstNum = type.at(4);
						if ((firstNum == '1') && (len == 5)) {
							temp.SetDataWidthW(1);
						}
						else if (firstNum == '2') {
							temp.SetDataWidthW(2);
						}
						else if (firstNum == '4') {
							temp.SetDataWidthW(4);
						}
						else if (firstNum == '8') {
							temp.SetDataWidthW(8);
						}
						else if ((firstNum == '1') && (len == 6)) {
							temp.SetDataWidthW(16);
						}
						else if (firstNum == '3') {
							temp.SetDataWidthW(32);
						}
						else if (firstNum == '6') {
							temp.SetDataWidthW(64);
						}
						_wires.push_back(temp);
					}//end of unsigned int input
					//COMMENT ME OUT LATER
					//cout << temp.GetVariableW() << " with bitwidth "<< temp.GetDataWidthW() << endl;
					check3 = iow;
					check4 = type;

					while (extra != "") {
						//COMMENT ME OUT LATER
						//cout << temp.GetVariableW() << " with bitwidth "<< temp.GetDataWidthW() << endl;

						/*added this to remove commas from wire Variable names*/
						if (extra.at(extra.length() - 1) == ',') {
							extra.erase(extra.length() - 1);
						}

						temp.SetVariableW(extra);
						_wires.push_back(temp);
						extra = "";
						lineStream >> extra;

						if (extra == "" || (extra.at(0) == '/')) {
							break;
						}
					}
				}// end wires


				//EQUATIONS

				//lineStream >> iow >> type >> variable >> extra;
				if (type == "=") {
					Equations temp;
					temp.SetOut(iow);
					temp.SetFirst(variable);
					temp.SetOperation(extra);
				//	temp.SetTimeSlotASAPE(0);
				//	temp.SetTimeSlotALAPE(_latency);

					if (extra == "?") {
						lineStream >> mux2 >> colon >> sel;
						temp.SetSecond(mux2);
						temp.SetMuxSel(sel);
					}
					else {
						lineStream >> extra;
						temp.SetSecond(extra);
						temp.SetMuxSel("0");
					}
					_equations.push_back(temp);
				}

				//lineStream >> iow >> type >> variable >> extra;
				//// if else
				if ((iow == "if") || (iow == "else")) {
					Equations temp;
					temp.SetOperation(iow);
					temp.SetOut(variable); 
					_equations.push_back(temp); 
				}
				//end equations
			}
		}
	}
	inputFileStream.close();

}

unsigned int verilogSim::TestValid() {
    
    unsigned long int lenI, lenO, lenW, lenE, lenR, lenDecVars, lenEqVars;
    unsigned int i = 0, j = 0;
	bool declared = false;
    
    Equations temp; 
    lenE = _equations.size();
	//lenI = _inputs.size(); 
	//lenO = _outputs.size(); 
	//lenW = _wires.size(); 
	//lenR = _registers.size(); 

    while (i < lenE) {
        temp = _equations.at(i);
        string test = temp.GetOperation();
        
        if ((temp.GetOperation() == "+") || (temp.GetOperation() == "-") || (temp.GetOperation() == "*") || (temp.GetOperation() == ">") || (temp.GetOperation() == "") || (temp.GetOperation() == "<") || (temp.GetOperation() == "?") || (temp.GetOperation() == ">>") || (temp.GetOperation() == "<<") || (temp.GetOperation() == "==")) {
            //here the equations is invalid if the operator so we just need to say invalid or something idk
            //what teh requirements are
            //cout << "invaldi operation" << endl;
            break;
        }
        else {
            return (1);
        }
        i++;
    }

	/*for (i = 0; i < lenE; i++) {
		for(j = 0; j < )
	 }*/
    
//     i = 0;
//     int checkI = 0, checkO = 0, checkW = 0, checkR = 0;
    
     lenI = _inputs.size();
     lenO = _outputs.size();
     lenW = _wires.size();
     lenE = _equations.size();
     lenR = _registers.size();
    
    
	std::vector<string> eqVars;    //All variables that appear in the equations 
	std::vector<string> decVars;   //All variables that are declared at the top 

     for(i = 0; i < lenI; i++) {
         decVars.push_back(_inputs.at(i).GetVariableI());
     }
     for(i = 0; i < lenO; i++) {
         decVars.push_back(_outputs.at(i).GetVariableO());
     }
     for(i = 0; i < lenW; i++) {
         decVars.push_back(_wires.at(i).GetVariableW());
     }
     for(i = 0; i < lenR; i++) {
         decVars.push_back(_registers.at(i).GetVariableR());
     }

	 for (i = 0; i < lenE; i++) {
		 eqVars.push_back(_equations.at(i).GetFirst()); 
		 eqVars.push_back(_equations.at(i).GetSecond());
		 eqVars.push_back(_equations.at(i).GetMuxSel());
		 eqVars.push_back(_equations.at(i).GetOut());
	 }

	 lenDecVars = decVars.size(); 
	 lenEqVars = eqVars.size(); 

	 for (i = 0; i < lenEqVars; i++) { //Run through all variables and zeros that show up in the equations 
		// auto tempS = eqVars.at(i); 
		 if ((eqVars.at(i) == "0") || (eqVars.at(i) == "")) { //If variable in the equation is a 0 mark it as declared to prevent it from breaking 
			 declared = true; 
		 }
		 else {
			 for (j = 0; j < lenDecVars; j++) {  //Check if variable in equation matches one of the declared variables 
				 if (eqVars.at(i) == decVars.at(j)) { //If it matches one of the declared variables set declared to true
					 declared = true;
				 }
			 }
		 }
		 if (declared != true) { //If the variable is never found in the declared variables then return error code 2
			 return(2); 
		 }
		 declared = false; 
	 }

//     eqvars.push_back(_equations.at(0).getout());
  //   unsigned int len = eqVars.size();
//     //eqVars.push_back(_equations.at(i).GetOut());
//     //eqVars.push_back(_equations.at(i).GetFirst());
//     //eqVars.push_back(_equations.at(i).GetSecond());
//     for(i = 0; i < lenE; i++) {
//         if (std::find(eqVars.begin(), eqVars.end(), _equations.at(i).GetOut()) == eqVars.end()) {
//             // someName not in name, add it
//             eqVars.push_back(_equations.at(i).GetOut());
//         }

//         if (std::find(eqVars.begin(), eqVars.end(), _equations.at(i).GetFirst()) == eqVars.end()) {
//             // someName not in name, add it
//             eqVars.push_back(_equations.at(i).GetFirst());
//         }
//         if (std::find(eqVars.begin(), eqVars.end(), _equations.at(i).GetSecond()) == eqVars.end()) {
//             // someName not in name, add it
//             eqVars.push_back(_equations.at(i).GetSecond());
//         }

//         }
    
// //    for(i=0;i<eqVars.size();i++){
// //        cout << eqVars.at(i);
// //    }
//     int finalCheck = 0;
//     string missingVar = "";
    
//     for(i=0;i<eqVars.size() - 1;i++){
//         finalCheck = 0;
// //        if(i == eqVars.size() - 1){
// //            cout << "vfffff";
// //        }
//         for(j=0;j<allVars.size();j++){
//             if(eqVars.at(i) == allVars.at(j)){
//                 finalCheck = 1;
//             }
//             if ((finalCheck == 0) && (j == allVars.size() - 1)){
//                 cout << "You have an undeclared varibale in your equations, (" << eqVars.at(i) << ")" << endl;
//                 missingVar = eqVars.at(i);
//                 return (2);
                
                
//             }
//        }
//     }

    return 0;
}

/*
void verilogSim::TimeSlotScheduling() {

	unsigned int firstTimeSlotASAP = 0; 
	unsigned int secondTimeSlotASAP = 0; 
	unsigned int equationTimeSlotALAP = 0; 
	int i, j, k1,k2;
	int k;
	string flag_variable1, flag_variable_2;

	int lenI = _inputs.size();
	int lenO = _outputs.size();
	int lenW = _wires.size();
	int lenE = _equations.size();

	//ASAP time slot scheduling 
	for (j = 0; j < lenE; j++) {
		k1 = 0;
		k2 = 0;
		for (i = 0; i < lenI; i++) {
			//Scheduling for Inputs
			if (_inputs.at(i).GetVariableI() == _equations.at(j).GetFirst()) {
				firstTimeSlotASAP = _inputs.at(i).GetTimeSlotASAPI();
				k1 = j;
			}
			else if (_inputs.at(i).GetVariableI() == _equations.at(j).GetSecond()) {
				secondTimeSlotASAP = _inputs.at(i).GetTimeSlotASAPI();
				k2 = j;
			}
		}
		for (i = 0; i < lenO; i++) {
			//Scheduling for Outputs
			if (_outputs.at(i).GetVariableO() == _equations.at(j).GetFirst()) {
				firstTimeSlotASAP = _outputs.at(i).GetTimeSlotASAPO();
				k1 = j;
			}
			else if (_inputs.at(i).GetVariableI() == _equations.at(j).GetSecond()) {
				secondTimeSlotASAP = _outputs.at(i).GetTimeSlotASAPO();
				k2 = j;
			}
		}
		for (i = 0; i < lenW; i++) {
			//Scheduling for Variables
			if (_wires.at(i).GetVariableW() == _equations.at(j).GetFirst()) {
				firstTimeSlotASAP = _wires.at(i).GetTimeSlotASAPW();
				k1 = j;
			}
			else if (_wires.at(i).GetVariableW() == _equations.at(j).GetSecond()) {
				secondTimeSlotASAP = _wires.at(i).GetTimeSlotASAPW();
				k2 = j;
			}
			
		}
			if (firstTimeSlotASAP >= secondTimeSlotASAP) {
				_equations.at(j).SetTimeSlotASAPE(firstTimeSlotASAP + 1);
			}
			else {
				_equations.at(j).SetTimeSlotASAPE(secondTimeSlotASAP + 1);
			}
			//finding and changing asap time of variables
			for (i = 0; i < lenI; i++) {
				if ((_inputs.at(i).GetVariableI() == _equations.at(j).GetOut()) || (_inputs.at(i).GetVariableI() == _equations.at(j).GetOut())) {
					if (_equations.at(j).GetOperation() == "*") {
						_inputs.at(i).SetTimeSlotASAPI(_equations.at(j).GetTimeSlotASAPE() + 1);
					}
					else {
						_inputs.at(i).SetTimeSlotASAPI(_equations.at(j).GetTimeSlotASAPE());
					}
				}
			}
			for (i = 0; i < lenO; i++) {
				if ((_outputs.at(i).GetVariableO() == _equations.at(j).GetOut()) || (_outputs.at(i).GetVariableO() == _equations.at(j).GetOut())) {
					if (_equations.at(j).GetOperation() == "*") {
						_outputs.at(i).SetTimeSlotASAPO(_equations.at(j).GetTimeSlotASAPE() + 1);
					}
					else {
						_outputs.at(i).SetTimeSlotASAPO(_equations.at(j).GetTimeSlotASAPE());
					}
				}
			}
			for (i = 0; i < lenW; i++) {
				if ((_wires.at(i).GetVariableW() == _equations.at(j).GetOut()) || (_wires.at(i).GetVariableW() == _equations.at(j).GetOut())) {
					if (_equations.at(j).GetOperation() == "*") {
						_wires.at(i).SetTimeSlotASAPW(_equations.at(j).GetTimeSlotASAPE() + 1);
					}
					else {
						_wires.at(i).SetTimeSlotASAPW(_equations.at(j).GetTimeSlotASAPE());
					}
				}
			}
	}

	//ALAP time slot scheduling 
	
	for (j = lenE - 1; j >= 0; j--) {
		equationTimeSlotALAP = _equations.at(j).GetTimeSlotALAPE() + 1;
		if (j < lenE - 1) {
			for (k = 0; k < lenE; k++) {
				if ((_equations.at(k).GetOut() == _equations.at(j + 1).GetFirst()) || (_equations.at(k).GetOut() == _equations.at(j + 1).GetSecond())) {
					equationTimeSlotALAP = _equations.at(j + 1).GetTimeSlotALAPE();
					break;
				}
				
			}
		}

		if (_equations.at(j).GetOperation() == "*") {
			equationTimeSlotALAP = equationTimeSlotALAP - 2;
		}
		else {
			equationTimeSlotALAP = equationTimeSlotALAP - 1;
		}
		for (i = 0; i < lenI; i++) {
			//Scheduling for Inputs ALAP
			if ((_inputs.at(i).GetVariableI() == _equations.at(j).GetFirst()) || (_inputs.at(i).GetVariableI() == _equations.at(j).GetSecond())) {
				_inputs.at(i).SetTimeSlotALAPI(equationTimeSlotALAP);
				_equations.at(j).SetTimeSlotALAPE(_inputs.at(i).GetTimeSlotALAPI());
			}
		}
		for (i = 0; i < lenO; i++) {
			//Scheduling for Outputs ALAP
			if ((_outputs.at(i).GetVariableO() == _equations.at(j).GetFirst()) || (_outputs.at(i).GetVariableO() == _equations.at(j).GetSecond())) {
				_outputs.at(i).SetTimeSlotALAPO(equationTimeSlotALAP);
				_equations.at(j).SetTimeSlotALAPE(_outputs.at(i).GetTimeSlotALAPO());
			}
		}
		for (i = 0; i < lenW; i++) {
			//Scheduling for Variables ALAP
			if ((_wires.at(i).GetVariableW() == _equations.at(j).GetFirst()) || (_wires.at(i).GetVariableW() == _equations.at(j).GetSecond())) {
				_wires.at(i).SetTimeSlotALAPW(equationTimeSlotALAP);
				_equations.at(j).SetTimeSlotALAPE(_wires.at(i).GetTimeSlotALAPW());
			}
		}
	}
	}
	*/

void verilogSim::WriteCommandsToFile() {

	ofstream outputFile;
	outputFile.open(_outputFileName);

	//okay the file to write to is open here
	//now we just have to write to the file based off the 4 vectors
	//if type = "I" or "U" print out int or uint for that line//subtract one from the datawidth to put into the brackets in the brackets of verilog
	//and add the variable to the end
	//
	//so on and so forth for the outputs wires and equations
	//and somewhere we have to check the bitwidth of things in order to concatenate the smaller ones

	//start writing the module
	outputFile << "`timescale 1ns / 1ps" << endl;
	outputFile << "//////////////////////////////////////////////////////////" << endl;
	outputFile << "module HLSM(Clk, Rst, Start, ";

	//loop to add in all inputs and outputs to the module declaration
	//this is all the first line of the .v file
	for (auto a = 0; a < _inputs.size(); a++) {
		outputFile << _inputs.at(a).GetVariableI() << ", ";
	}
	outputFile << "Done, ";
	for (auto b = 0; b < _outputs.size() - 1; b++) {
		outputFile << _outputs.at(b).GetVariableO() << ", ";
	}
	outputFile << _outputs.back().GetVariableO() << ");" << endl;

	//make sure the necessary HLSM inputs exist
	outputFile << "input Clk, Rst, Start;" << endl;
	
	//now we begin adding the inputs, they're all individual for now
	//if you figure out how to group them all by sizes props
	for (auto a = 0; a < _inputs.size(); a++) {
		outputFile << "input ";

		//tack on signed if the type is signed, unsigneds don't need to be declared
		if (_inputs.at(a).GetDataTypeI() == 'I') {
			outputFile << "signed ";
		}

		//add in the bitwidth part of the text
		if (_inputs.at(a).GetDataWidthI() == 1) {
			outputFile << _inputs.at(a).GetVariableI() << ";" << endl;
		}
		else {
			outputFile << "[" << _inputs.at(a).GetDataWidthI() - 1 << ":0] " << _inputs.at(a).GetVariableI() << ";" << endl;
		}
	}
	
	//make sure necessary HLSM output exists
	outputFile << "output reg Done;" << endl;

	//outputs are also all on their own
	for (auto a = 0; a < _outputs.size(); a++) {
		outputFile << "output ";

		//tack on signed if the type is signed
		if (_outputs.at(a).GetDataTypeO() == 'I') {
			outputFile << "signed ";
		}

		//add in the bitwidth part
		if (_outputs.at(a).GetDataWidthO() == 1) {
			outputFile << _outputs.at(a).GetVariableO() << ";" << endl;
		}
		else {
			outputFile << "[" << _outputs.at(a).GetDataWidthO() - 1 << ":0] " << _outputs.at(a).GetVariableO() << ";" << endl;
		}
	}

	//wires are also all on their own
	for (auto w = 0; w < _wires.size(); w++) {
		outputFile << "wire ";

		//tack on signed if the type is signed
		if (_wires.at(w).GetDataTypeW() == 'I') {     
			outputFile << "signed ";
		}

		//add in the bitwidth part
		if (_wires.at(w).GetDataWidthW() == 1) {
			outputFile << _wires.at(w).GetVariableW() << ";" << endl;
		}
		else {
			outputFile << "[" << _wires.at(w).GetDataWidthW() - 1 << ":0] " << _wires.at(w).GetVariableW() << ";" << endl;
		}
	}

	cout << "The _register.size() - 1 is: " << _registers.size() - 1 << endl;

	//Registers are also all on their own
	for (auto r = 0; r < _registers.size(); r++) {
		outputFile << "wire ";

		//tack on signed if the type is signed
		if (_registers.at(r).GetDataTypeR() == 'I') {     
			outputFile << "signed ";
		}

		//add in the bitwidth part
		if (_registers.at(r).GetDataWidthR() == 1) {
			outputFile << _registers.at(r).GetVariableR() << ";" << endl;
		}
		else {
			outputFile << "[" << _registers.at(r).GetDataWidthR() - 1 << ":0] " << _registers.at(r).GetVariableR() << ";" << endl;
		}
	}

	
	//start printing the equations
	for (auto f = 0; f < _equations.size(); f++) {
		//format is generally (changes for certain ops):
		//      ADD #(32) Add9(l00, l01, l10);

		//these variables are for getting the bitwidth that is placed in parentheses like #(64)
		auto first = _equations.at(f).GetFirst();
		auto firstType = 0;							//First Variable Type: 0 = input, 1 = wire, initialized to input

		auto second = _equations.at(f).GetSecond();
		auto secondType = 0;						//Second Variable Type: 0 = input, 1 = wire, initialized to input

		Inputs initI;   //Empty Initialization Variables 
		Wires initW;
		Registers initR; 
		auto firstVariableI = initI;    //initialize first input to first input in vector
		auto firstVariableW = initW;	//intialize first wire variable to first wire in vector
		auto firstVariableR = initR;	//Initialise first register variable to first register in vector 
		auto secondVariableI = initI;	//initialize second variable input to first input in vector
		auto secondVariableW = initW;	//initialize second variable wire to first wire in vector
		auto secondVariableR = initR;   //initialize second variable register to first register in vector

		auto op = _equations.at(f).GetOperation(); 
		auto out = _equations.at(f).GetOut();
		auto muxSel = _equations.at(f).GetMuxSel();
		int fWidth = 0;
		int sWidth = 0;
		int oWidth = 0;   //Added this for setting bitwidth when output is larger 
		int bitWidth = 0;
		int bitWidthc = 0;  //Bitwidth of output of comparator 
		int difference = 0; //Difference between bitwidth of output and input
		int sign = 0; //Added this for keeping track of the sign of the function

		


//        cout << _inputs.at(0).GetVariableI() << endl;
//        cout << _inputs.at(1).GetVariableI() << endl;
//        cout << _inputs.at(0).GetDataWidthI() << endl;
//        cout << _inputs.at(1).GetDataWidthI() << endl;


		//compare the first and second operands to find their bitwidths
		//starting with an inputs comparison
		for (auto a = 0; a < _inputs.size(); a++) {
			if (first == _inputs.at(a).GetVariableI()) {
				fWidth = _inputs.at(a).GetDataWidthI();
				firstVariableI = _inputs.at(a);

				//cout << "first got: " << _inputs.at(g).GetDataWidthI() << endl;
			}
			if (second == _inputs.at(a).GetVariableI()) {
				sWidth = _inputs.at(a).GetDataWidthI();
				secondVariableI = _inputs.at(a);

				//cout << "second got: " << _inputs.at(g).GetDataWidthI() << endl;
			}
		}
		//next an outputs comparison
		for (auto b = 0; b < _outputs.size(); b++) {
			//            if (out == _outputs.at(h).GetVariableO()) {
			//                cout << _outputs.at(0).GetVariableO() << endl;
			//                cout << "it entered the output check loop!" << endl;
			//            }

			if (first == _outputs.at(b).GetVariableO()) {
				fWidth = _outputs.at(b).GetDataWidthO();
			}
			if (second == _outputs.at(b).GetVariableO()) {
				sWidth = _outputs.at(b).GetDataWidthO();
			}

			if (out == _outputs.at(b).GetVariableO()) {     //If the output variable is a signed output, set sign to true
				if (_outputs.at(b).GetDataTypeO() == 'I') {
					sign = 1;
				}
				oWidth = _outputs.at(b).GetDataWidthO();   //Set width of output 
			}
		}
		//finally a wires comparison
		for (auto c = 0; c < _wires.size(); c++) {
			auto tempVariableW = _wires.at(c).GetVariableW();

			/*if (tempVariableW.size() == 2)
			{
				tempVariableW.erase(_wire.GetVariableW().length() - 1);
			}  //Erase Extra Space Character */

			if (tempVariableW.compare(first) == 0) {
				fWidth = _wires.at(c).GetDataWidthW();
				firstVariableW = _wires.at(c);
				firstType = 1;
				//fWidth = temp1Width;
				cout << "width for first is " << fWidth << endl;
			}

			if (tempVariableW.compare(second) == 0) {
				sWidth = _wires.at(c).GetDataWidthW();
				secondVariableW = _wires.at(c);
				secondType = 1;
				//sWidth = temp2Width;
				cout << "width for second is " << sWidth << endl;
			}

			if (out == _wires.at(c).GetVariableW()) {     //If the output of the equation is a signed wire, set sign to true 
				if (_wires.at(c).GetDataTypeW() == 'I') {
					sign = 1;
				}
				oWidth = _wires.at(c).GetDataWidthW();   //Set width of output 
			}
		}

		for (auto d = 0; d < _registers.size(); d++) {
			auto tempVariableR = _registers.at(d).GetVariableR();

			/*if (tempVariableW.size() == 2)
			{
				tempVariableW.erase(_wire.GetVariableW().length() - 1);
			}  //Erase Extra Space Character */

			if (tempVariableR.compare(first) == 0) {
				fWidth = _registers.at(d).GetDataWidthR();
				firstVariableR = _registers.at(d);
				firstType = 2;
				//fWidth = temp1Width;
				//cout << "width for first is " << fWidth << endl;
			}

			if (tempVariableR.compare(second) == 0) {
				sWidth = _registers.at(d).GetDataWidthR();
				secondVariableR = _registers.at(d);
				secondType = 2;
				//sWidth = temp2Width;
				//cout << "width for second is " << sWidth << endl;
			}

			if (out == _registers.at(d).GetVariableR()) {     //If the output of the equation is a register 
				if (_registers.at(d).GetDataTypeR() == 'I') {  //If the register is signed, set signed to true 
					sign = 1;
				}
				oWidth = _registers.at(d).GetDataWidthR();   //Set width of output 
			}
		}




		/*
		if (first == _wire.GetVariableW()) {
			fWidth = _wire.GetDataWidthW();
			//fWidth = temp1Width;
			cout << "width for first is " << fWidth << endl;
		}
		auto temp2Width = _wire.GetDataWidthW();
		if (second == _wire.GetVariableW()) {
			sWidth = _wire.GetDataWidthW();
			//sWidth = temp2Width;
			cout << "width for second is " << sWidth << endl;
		}
	} */

	//now we do a comparison to find the bigger bitwidth
		bitWidth = oWidth;                             //Bitwidth of all components besides comparator is size of output
		bitWidthc = std::max(fWidth, sWidth);          //Bitwidth of comparator is max of inputs 

		cout << op; 

		std::ostringstream strm;   //Holds the all of the difference streams

		std::string diffStr;   //Holds bitwidth difference string 
		std::string diffStr1;  //Holds bitwidth difference minus 1
		std::string diffStr3;  //Holds the bitwidth when concatination is needed 
		
		if ((op == ">") || (op == "==") || (op == "<")) {  //If operation is  a comparator, use the bitWidthc
			if (firstType == 0) {  //If the first variable is of type input
				difference = bitWidthc - firstVariableI.GetDataWidthI(); //Get Difference when bitWidth is larger 
				strm << difference; 
				diffStr = strm.str(); //store difference stream in string

				difference = difference - 1; //Subtract 1 from N to put in bidwidth brackets 
				strm << difference; 
				diffStr1 = strm.str(); //store difference - 1 stream in string

				difference = firstVariableI.GetDataWidthI() - bitWidthc; //Get difference when input has larger bitWidth
				strm << difference; 
				difference = firstVariableI.GetDataWidthI() - difference - 1; //get difference if first input has a wider bit width than output 
				strm << difference; 
				diffStr3 = strm.str();  //store bitwidth needed to concatinate to
				
				if ((firstVariableI.GetDataWidthI() < bitWidthc) && (firstVariableI.GetDataTypeI() == 'I')) {  //If first input is fewer bits than output, signed, and smaller than owidth, then sign extend it
					first = "{{" + diffStr + '{' + first + "[" + diffStr1 + "]}}, " + first + '}';
				}
				if ((firstVariableI.GetDataWidthI() < bitWidthc) && (firstVariableI.GetDataTypeI() == 'U')) { //If first input is fewer bits than output and unsigned, and smaller than owidth , then pad with zeros
					first = "{" + diffStr + "'b0, " + first + "}";
				}
				if ((firstVariableI.GetDataWidthI() > bitWidthc)) {   //If first input has a wider bit width than output, connect least significant bits
					first = first + "[" + diffStr3 + ":0]";
				}
				//y[15:0]
			}
			else if (firstType == 1) { //If the first variable of equation is of type wire
				difference = bitWidthc - firstVariableW.GetDataWidthW(); //Get Difference when bitWidth is larger 
				strm << difference;
				diffStr = strm.str(); //store difference stream in string

				difference = difference - 1; //Subtract 1 from N to put in bidwidth brackets 
				strm << difference;
				diffStr1 = strm.str(); //store difference - 1 stream in string

				difference = firstVariableW.GetDataWidthW() - bitWidthc; //Get difference when input has larger bitWidth
				strm << difference;
				difference = firstVariableW.GetDataWidthW() - difference - 1; //get difference if first input has a wider bit width than output 
				strm << difference;
				diffStr3 = strm.str();  //store bitwidth needed to concatinate to

				if ((firstVariableW.GetDataWidthW() < bitWidthc) && (firstVariableW.GetDataTypeW() == 'I')) { //If first wire is fewer bits than output and signed, then sign extend it
					first = "{{" + diffStr + '{' + first + "[" + diffStr1 + "]}}, " + first + '}';
				}
				if ((firstVariableW.GetDataWidthW() < bitWidthc) && (firstVariableW.GetDataTypeW() == 'U')) { //If first wire is fewer bits than output and unsigned, then pad with zeros
					first = "{" + diffStr + "'b0, " + first + "}";
				}
				if ((firstVariableW.GetDataWidthW() > bitWidthc)) { //If first input has a wider bit width than output, connect least significant bits
					first = first + "[" + diffStr3 + ":0]";
				}
			}

			else if (firstType == 2) { //If the first variable of equation is of type register 
				difference = bitWidthc - firstVariableR.GetDataWidthR(); //Get Difference when bitWidth is larger 
				strm << difference;
				diffStr = strm.str(); //store difference stream in string

				difference = difference - 1; //Subtract 1 from N to put in bidwidth brackets 
				strm << difference;
				diffStr1 = strm.str(); //store difference - 1 stream in string

				difference = firstVariableR.GetDataWidthR() - bitWidthc; //Get difference when input has larger bitWidth
				strm << difference;
				difference = firstVariableR.GetDataWidthR() - difference - 1; //get difference if first input has a wider bit width than output 
				strm << difference;
				diffStr3 = strm.str();  //store bitwidth needed to concatinate to

				if ((firstVariableR.GetDataWidthR() < bitWidthc) && (firstVariableR.GetDataTypeR() == 'I')) { //If first register is fewer bits than output and signed, then sign extend it
					first = "{{" + diffStr + '{' + first + "[" + diffStr1 + "]}}, " + first + '}';
				}
				if ((firstVariableR.GetDataWidthR() < bitWidthc) && (firstVariableR.GetDataTypeR() == 'U')) { //If the first register is fewer bits than output and unsigned, then pad with zeros
					first = "{" + diffStr + "'b0, " + first + "}";
				}
				if ((firstVariableR.GetDataWidthR() > bitWidthc)) { //If first input has a wider bit width than output, connect least significant bits
					first = first + "[" + diffStr3 + ":0]";
				}
			}


			if (secondType == 0) {  //If the second variable is of type input
				difference = bitWidthc - secondVariableI.GetDataWidthI(); //Get Difference when bitWidth is larger 
				strm << difference;
				diffStr = strm.str(); //store difference stream in string

				difference = difference - 1; //Subtract 1 from N to put in bidwidth brackets 
				strm << difference;
				diffStr1 = strm.str(); //store difference - 1 stream in string

				difference = secondVariableI.GetDataWidthI() - bitWidthc; //Get difference when input has larger bitWidth
				strm << difference;
				difference = secondVariableI.GetDataWidthI() - difference - 1; //get difference if first input has a wider bit width than output 
				strm << difference;
				diffStr3 = strm.str();  //store bitwidth needed to concatinate to

				if ((secondVariableI.GetDataWidthI() < bitWidthc) && (secondVariableI.GetDataTypeI() == 'I')) { //If second input is fewer bits than output and signed, sign extend
					second = "{{" + diffStr + '{' + second + "[" + diffStr1 + "]}}, " + second + '}';
				}

				if ((secondVariableI.GetDataWidthI() < bitWidthc) && (secondVariableI.GetDataTypeI() == 'U')) { //If second input is fewer bits than output and unsigned, pad with zeros
					second = "{" + diffStr + "'b0, " + second + "}";
				}
				if ((secondVariableI.GetDataWidthI() > bitWidthc)) {   //If second input has a wider bit width than output, connect least significant bits
					second = second + "[" + diffStr3 + ":0]";
				}
			}
			else if (secondType == 1) { //If the second variable is of type wire
				difference = bitWidthc - secondVariableW.GetDataWidthW(); //Get Difference when bitWidth is larger 
				strm << difference;
				diffStr = strm.str(); //store difference stream in string

				difference = difference - 1; //Subtract 1 from N to put in bidwidth brackets 
				strm << difference;
				diffStr1 = strm.str(); //store difference - 1 stream in string

				difference = secondVariableW.GetDataWidthW() - bitWidthc; //Get difference when input has larger bitWidth
				strm << difference;
				difference = secondVariableW.GetDataWidthW() - difference - 1; //get difference if first input has a wider bit width than output 
				strm << difference;
				diffStr3 = strm.str();  //store bitwidth needed to concatinate to

				if ((secondVariableW.GetDataWidthW() < bitWidthc) && (secondVariableW.GetDataTypeW() == 'I')) { //If second wire is fewer bits than output and signed, sign extend
					second = "{{" + diffStr + '{' + second + "[" + diffStr1 + "]}}, " + second + '}';
				}

				if ((secondVariableW.GetDataWidthW() < bitWidthc) && (secondVariableW.GetDataTypeW() == 'U')) { //If second wire is fewer bits than output and unsigned, pad with zeros
					second = "{" + diffStr + "'b0, " + second + "}";
				}
				if ((secondVariableW.GetDataWidthW() > bitWidthc)) {   //If second wire has a wider bit width than output, connect least significant bits
					second = second + "[" + diffStr3 + ":0]";
				}
			}

			else if (secondType == 2) { //If the second variable is of type register
				difference = bitWidthc - secondVariableR.GetDataWidthR(); //Get Difference when bitWidth is larger 
				strm << difference;
				diffStr = strm.str(); //store difference stream in string

				difference = difference - 1; //Subtract 1 from N to put in bidwidth brackets 
				strm << difference;
				diffStr1 = strm.str(); //store difference - 1 stream in string

				difference = secondVariableR.GetDataWidthR() - bitWidthc; //Get difference when input has larger bitWidth
				strm << difference;
				difference = secondVariableR.GetDataWidthR() - difference - 1; //get difference if first input has a wider bit width than output 
				strm << difference;
				diffStr3 = strm.str();  //store bitwidth needed to concatinate to

				if ((secondVariableR.GetDataWidthR() < bitWidthc) && (secondVariableR.GetDataTypeR() == 'I')) { //If second wire is fewer bits than output and signed, sign extend
					second = "{{" + diffStr + '{' + second + "[" + diffStr1 + "]}}, " + second + '}';
				}

				if ((secondVariableR.GetDataWidthR() < bitWidthc) && (secondVariableR.GetDataTypeR() == 'U')) { //If second wire is fewer bits than output and unsigned, pad with zeros
					second = "{" + diffStr + "'b0, " + second + "}";
				}
				if ((secondVariableR.GetDataWidthR() > bitWidthc)) {   //If second wire has a wider bit width than output, connect least significant bits
					second = second + "[" + diffStr3 + ":0]";
				}
			}
		}

		else {  //If operation is not a comparator, use the oWidth
			if (firstType == 0) {  //If the first variable is of type input
				difference = oWidth - firstVariableI.GetDataWidthI(); //Get Difference when bitWidth is larger 
				strm << difference;
				diffStr = strm.str(); //store difference stream in string

				difference = difference - 1; //Subtract 1 from N to put in bidwidth brackets 
				strm << difference;
				diffStr1 = strm.str(); //store difference - 1 stream in string

				difference = firstVariableI.GetDataWidthI() - oWidth; //Get difference when input has larger bitWidth
				strm << difference;
				difference = firstVariableI.GetDataWidthI() - difference - 1; //get difference if first input has a wider bit width than output 
				strm << difference;
				diffStr3 = strm.str();  //store bitwidth needed to concatinate to

				if ((firstVariableI.GetDataWidthI() < oWidth) && (firstVariableI.GetDataTypeI() == 'I')) {  //If first input is fewer bits than output, signed, and smaller than owidth, then sign extend it
					first = "{{" + diffStr + '{' + first + "[" + diffStr1 + "]}}, " + first + '}';
				}
				if ((firstVariableI.GetDataWidthI() < oWidth) && (firstVariableI.GetDataTypeI() == 'U')) { //If first input is fewer bits than output and unsigned, and smaller than owidth , then pad with zeros
					first = "{" + diffStr + "'b0, " + first + "}";
				}
				if ((firstVariableI.GetDataWidthI() > oWidth)) {   //If first input has a wider bit width than output, connect least significant bits
					first = first + "[" + diffStr3 + ":0]";
				}
			//y[15:0]
			}
			else if (firstType == 1) { //If the first variable of equation is of type wire
				if (op != "?") {
				difference = oWidth - firstVariableW.GetDataWidthW(); //Get Difference when bitWidth is larger 
				strm << difference;
				diffStr = strm.str(); //store difference stream in string

				difference = difference - 1; //Subtract 1 from N to put in bidwidth brackets 
				strm << difference;
				diffStr1 = strm.str(); //store difference - 1 stream in string

				difference = firstVariableW.GetDataWidthW() - oWidth; //Get difference when input has larger bitWidth
				strm << difference;
				difference = firstVariableW.GetDataWidthW() - difference - 1; //get difference if first input has a wider bit width than output 
				strm << difference;
				diffStr3 = strm.str();  //store bitwidth needed to concatinate to

				diffStr3 = strm.str();  //store bitwidth needed to concatinate to
				if ((firstVariableW.GetDataWidthW() < oWidth) && (firstVariableW.GetDataTypeW() == 'I')) { //If first wire is fewer bits than output and signed, then sign extend it
					first = "{{" + diffStr + '{' + first + "[" + diffStr1 + "]}}, " + first + '}';
				}
				if ((firstVariableW.GetDataWidthW() < oWidth) && (firstVariableW.GetDataTypeW() == 'U')) { //If first wire is fewer bits than output and unsigned, then pad with zeros
					first = "{" + diffStr + "'b0, " + first + "}";
				}
				if ((firstVariableW.GetDataWidthW() > oWidth)) { //If first input has a wider bit width than output, connect least significant bits
					first = first + "[" + diffStr3 + ":0]";
				}
				}
			}

			else if (firstType == 2) { //If the first variable of equation is of type register 
				difference = oWidth - firstVariableR.GetDataWidthR(); //Get Difference when bitWidth is larger 
				strm << difference;
				diffStr = strm.str(); //store difference stream in string

				difference = difference - 1; //Subtract 1 from N to put in bidwidth brackets 
				strm << difference;
				diffStr1 = strm.str(); //store difference - 1 stream in string

				difference = firstVariableR.GetDataWidthR() - oWidth; //Get difference when input has larger bitWidth
				strm << difference;
				difference = firstVariableR.GetDataWidthR() - difference - 1; //get difference if first input has a wider bit width than output 
				strm << difference;
				diffStr3 = strm.str();  //store bitwidth needed to concatinate to

				if ((firstVariableR.GetDataWidthR() < oWidth) && (firstVariableR.GetDataTypeR() == 'I')) { //If first register is fewer bits than output and signed, then sign extend it
					first = "{{" + diffStr + '{' + first + "[" + diffStr1 + "]}}, " + first + '}';
				}
				if ((firstVariableR.GetDataWidthR() < oWidth) && (firstVariableR.GetDataTypeR() == 'U')) { //If the first register is fewer bits than output and unsigned, then pad with zeros
					first = "{" + diffStr + "'b0, " + first + "}";
				}
				if ((firstVariableR.GetDataWidthR() > oWidth)) { //If first input has a wider bit width than output, connect least significant bits
					first = first + "[" + diffStr3 + ":0]";
				}
			}


			if (secondType == 0) {  //If the second variable is of type input
				difference = oWidth - secondVariableI.GetDataWidthI(); //Get Difference when bitWidth is larger 
				strm << difference;
				diffStr = strm.str(); //store difference stream in string

				difference = difference - 1; //Subtract 1 from N to put in bidwidth brackets 
				strm << difference;
				diffStr1 = strm.str(); //store difference - 1 stream in string

				difference = secondVariableI.GetDataWidthI() - oWidth; //Get difference when input has larger bitWidth
				strm << difference;
				difference = secondVariableI.GetDataWidthI() - difference - 1; //get difference if first input has a wider bit width than output 
				strm << difference;
				diffStr3 = strm.str();  //store bitwidth needed to concatinate to

				if ((secondVariableI.GetDataWidthI() < oWidth) && (secondVariableI.GetDataTypeI() == 'I')) { //If second input is fewer bits than output and signed, sign extend
					second = "{{" + diffStr + '{' + second + "[" + diffStr1 + "]}}, " + second + '}';
				}

				if ((secondVariableI.GetDataWidthI() < oWidth) && (secondVariableI.GetDataTypeI() == 'U')) { //If second input is fewer bits than output and unsigned, pad with zeros
					second = "{" + diffStr + "'b0, " + second + "}";
				}
				if ((secondVariableI.GetDataWidthI() > oWidth)) {   //If second input has a wider bit width than output, connect least significant bits
					second = second + "[" + diffStr3 + ":0]";
				}
			}
			else if (secondType == 1) { //If the second variable is of type wire
				if ((op == "<<" || op == ">>") && (secondVariableW.GetDataWidthW() == 1)) {}
				else {
				difference = oWidth - secondVariableW.GetDataWidthW(); //Get Difference when bitWidth is larger 
				strm << difference;
				diffStr = strm.str(); //store difference stream in string

				difference = difference - 1; //Subtract 1 from N to put in bidwidth brackets 
				strm << difference;
				diffStr1 = strm.str(); //store difference - 1 stream in string

				difference = secondVariableW.GetDataWidthW() - oWidth; //Get difference when input has larger bitWidth
				strm << difference;
				difference = secondVariableW.GetDataWidthW() - difference - 1; //get difference if first input has a wider bit width than output 
				strm << difference;
				diffStr3 = strm.str();  //store bitwidth needed to concatinate to

				if ((secondVariableW.GetDataWidthW() < oWidth) && (secondVariableW.GetDataTypeW() == 'I')) { //If second wire is fewer bits than output and signed, sign extend
					second = "{{" + diffStr + '{' + second + "[" + diffStr1 + "]}}, " + second + '}';
				}

				if ((secondVariableW.GetDataWidthW() < oWidth) && (secondVariableW.GetDataTypeW() == 'U')) { //If second wire is fewer bits than output and unsigned, pad with zeros
					second = "{" + diffStr + "'b0, " + second + "}";
				}
				if ((secondVariableW.GetDataWidthW() > oWidth)) {   //If second wire has a wider bit width than output, connect least significant bits
					second = second + "[" + diffStr3 + ":0]";
				}
				}
			}

			else if (secondType == 2) { //If the second variable is of type register
				difference = oWidth - secondVariableR.GetDataWidthR(); //Get Difference when bitWidth is larger 
				strm << difference;
				diffStr = strm.str(); //store difference stream in string

				difference = difference - 1; //Subtract 1 from N to put in bidwidth brackets 
				strm << difference;
				diffStr1 = strm.str(); //store difference - 1 stream in string

				difference = secondVariableR.GetDataWidthR() - oWidth; //Get difference when input has larger bitWidth
				strm << difference;
				difference = secondVariableR.GetDataWidthR() - difference - 1; //get difference if first input has a wider bit width than output 
				strm << difference;
				diffStr3 = strm.str();  //store bitwidth needed to concatinate to

				if ((secondVariableR.GetDataWidthR() < oWidth) && (secondVariableR.GetDataTypeR() == 'I')) { //If second wire is fewer bits than output and signed, sign extend
					second = "{{" + diffStr + '{' + second + "[" + diffStr1 + "]}}, " + second + '}';
				}

				if ((secondVariableR.GetDataWidthR() < oWidth) && (secondVariableR.GetDataTypeR() == 'U')) { //If second wire is fewer bits than output and unsigned, pad with zeros
					second = "{" + diffStr + "'b0, " + second + "}";
				}
				if ((secondVariableR.GetDataWidthR() > oWidth)) {   //If second wire has a wider bit width than output, connect least significant bits
					second = second + "[" + diffStr3 + ":0]";
				}
			}
		}




		// cout << "first Width is currently " << fWidth << endl;
		// cout << "second Width is currently " << sWidth << endl;
		// cout << "bitWidth is currently " << bitWidth << endl;

		 //now a big ol chain for checking what the operationis, signed or unsigned, and writing it
		if ((_equations.at(f).GetOperation() == "+") && (sign == 0)) {
			outputFile << "ADD #(" << bitWidth << ") Add" << f << "(" << first << ", " << second << ", " << out << ");" << endl;
		}
		//////////////// if else 
		else if (_equations.at(f).GetOut() == "}") {
			outputFile << "end" << endl;
		}
		else if (_equations.at(f).GetOperation() == "if") {
			outputFile << "if (" << _equations.at(f).GetOut() << ")" << endl << "begin" << endl;
		}
		else if (_equations.at(f).GetOperation() == "else") {
			outputFile << "else (" << _equations.at(f).GetOut() << ")" << endl << "begin" << endl;
		}
		//////////////// if else
		else if ((_equations.at(f).GetOperation() == "+") && (sign == 1)) {
			outputFile << "SADD #(" << bitWidth << ") SAdd" << f << "(" << first << ", " << second << ", " << out << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == "-") && (sign == 0)) {
			outputFile << "SUB #(" << bitWidth << ") Sub" << f << "(" << first << ", " << second << ", " << out << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == "-") && (sign == 1)) {
			outputFile << "SSUB #(" << bitWidth << ") SSub" << f << "(" << first << ", " << second << ", " << out << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == "*") && (sign == 0)) {
			outputFile << "MUL #(" << (bitWidth * 2) << ") Mult" << f << "(" << first << ", " << second << ", " << out << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == "*") && (sign == 1)) {
			outputFile << "SMUL #(" << (bitWidth * 2) << ") SMult" << f << "(" << first << ", " << second << ", " << out << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == ">") && (sign == 0)) {
			outputFile << "COMP #(" << bitWidthc << ") Comp" << f << "(" << first << ", " << second << ", " << out << ", " <<
				"0" << ", " << "0" << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == "<") && (sign == 0)) {
			outputFile << "COMP #(" << bitWidthc << ") Comp" << f << "(" << first << ", " << second << ", " <<
				"0" << ", " << out << ", " << "0" << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == "==") && (sign == 0)) {
			outputFile << "COMP #(" << bitWidthc << ") Comp" << f << "(" << first << ", " << second << ", " <<
				"0" << ", " << "0" << ", " << out << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == ">") && (sign == 1)) {
			outputFile << "SCOMP #(" << bitWidthc << ") SComp" << f << "(" << first << ", " << second << ", " << out << ", " <<
				"0" << ", " << "0" << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == "<") && (sign == 1)) {
			outputFile << "SCOMP #(" << bitWidthc << ") SComp" << f << "(" << first << ", " << second << ", " <<
				"0" << ", " << out << ", " << "0" << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == "==") && (sign == 1)) {
			outputFile << "SCOMP #(" << bitWidthc << ") SComp" << f << "(" << first << ", " << second << ", " <<
				"0" << ", " << "0" << ", " << out << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == "?") && (sign == 0)) {
			//FIX ME
			outputFile << "MUX2x1 #(" << bitWidth << ") Mux" << f << "(" << second << ", " << muxSel << ", " << first << ", " << out << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == "?") && (sign == 1)) {
			//FIX ME
			outputFile << "SMUX2x1 #(" << bitWidth << ") SMux" << f << "(" << second << ", " << muxSel << ", " << first << ", " << out << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == ">>") && (sign == 0)) {
			//FIX ME
			outputFile << "SHR #(" << bitWidth << ") Shr" << f << "(" << first << ", " << second << ", " << out << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == ">>") && (sign == 1)) {
			//FIX ME
			outputFile << "SSHR #(" << bitWidth << ") SShr" << f << "(" << first << ", " << second << ", " << out << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == "<<") && (sign == 0)) {
			//FIX ME
			outputFile << "SHL #(" << bitWidth << ") Shl" << f << "(" << first << ", " << second << ", " << out << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == "<<") && (sign == 1)) {
			//FIX ME
			outputFile << "SSHL #(" << bitWidth << ") SShl" << f << "(" << first << ", " << second << ", " << out << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == "") && (sign == 0)) {
			outputFile << "REG #(" << bitWidth << ") Reg" << f << "(" << first << ", " << "0, 0, " << out << ");" << endl;
		}
		else if ((_equations.at(f).GetOperation() == "") && (sign == 1)) {
			outputFile << "SREG #(" << bitWidth << ") SReg" << f << "(" << first << ", " << "0, 0, " << out << ");" << endl;
		}
	}

	//last line of the .v file
	outputFile << "endmodule";

	//close the fstream after writing the file
	outputFile.close();
}
