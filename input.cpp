//
//  input.cpp
//  dpgen
//
//  Created by Ian Hooks on 10/29/19.
//  Copyright © 2019 Ian Hooks. All rights reserved.
//

#include "input.hpp"
#include "Inputs.hpp"
#include "Outputs.hpp"
#include "Wires.hpp"
#include "Equations.hpp"
#include <string>

using namespace std;

void verilogSim::run(char* inputFileName, char* outputFileName) {
    _inputFileName = inputFileName;
    _outputFileName = outputFileName;
}

void verilogSim::ReadCommandsFromFile() {
    
    stringstream inputFile;
    inputFile << _inputFileName;
    
    ifstream inputFileStream;
    inputFileStream.open(inputFile.str());
    
    
    string line;
    
    string iow;
    string type;
    string check1, check2, check3, check4, check5, check6;
    string variable, stuff, more, mux2, colon, sel;
    string extra = "0";
    char firstLetter;
    char firstNum;
    int len;
    
    if(!inputFileStream.is_open()) {
        cout << "did not open input\n";
        return;
    }

    while (!inputFileStream.eof()) {
        getline(inputFileStream, line);
        std::istringstream lineStream(line);
        extra = "";
        lineStream >> iow >> type >> variable >> extra;
        unsigned long int x = extra.length();
        if(extra != ""){
        if(extra.at(extra.length()-1) == ','){
            extra.at(extra.length()-1) =  '\0';
             }
           }
        if(variable != ""){
            if(variable.at(variable.length()-1) == ','){
                variable.at(variable.length()-1) =  '\0';
            }
        }
        firstLetter = type.at(0);
        len = type.length();
        
        //INPUTS
        if((iow == "input") && ((iow != check1) || (type != check2))){
            Inputs temp;
            temp.SetVariableI(variable);
            
            if(firstLetter == 'I') {
                
                temp.SetDataTypeI('I');
                firstNum = type.at(3);
                
                if((firstNum == '1')&& (len == 4)){
                    temp.SetDataWidthI(1);
                }
                else if(firstNum == '2') {
                    temp.SetDataWidthI(2);
                }
                else if(firstNum == '4'){
                    temp.SetDataWidthI(4);
                }
                else if(firstNum == '8'){
                    temp.SetDataWidthI(8);
                }
                else if((firstNum == '1') && (len == 5)){
                    temp.SetDataWidthI(16);
                }
                else if(firstNum == '3') {
                    temp.SetDataWidthI(32);
                }
                else if(firstNum == '6') {
                    temp.SetDataWidthI(64);
                }
                _inputs.push_back(temp);
            }//end of signed int
            
            else if(firstLetter == 'U'){
                temp.SetDataTypeI('U');
                firstNum = type.at(4);
                if((firstNum == '1')&& (len == 5)){
                    temp.SetDataWidthI(1);
                }
                else if(firstNum == '2') {
                    temp.SetDataWidthI(2);
                }
                else if(firstNum == '4'){
                    temp.SetDataWidthI(4);
                }
                else if(firstNum == '8'){
                    temp.SetDataWidthI(8);
                }
                else if((firstNum == '1') && (len == 6)){
                    temp.SetDataWidthI(16);
                }
                else if(firstNum == '3') {
                    temp.SetDataWidthI(32);
                }
                else if(firstNum == '6') {
                    temp.SetDataWidthI(64);
                }
                _inputs.push_back(temp);
            }//end of unsigned int input
            check1 = iow;
            check2 = type;
            
        while(extra != "") {
            temp.SetVariableI(extra);
            _inputs.push_back(temp);
            lineStream >> extra;
            if(extra == temp.GetVariableI()){
                break;
            }
          }
        }// end of all inputs
        
        
        
        
        
        //OUTPUTS
        if((iow == "output") && ((iow != check3) || (type != check4))){
            Outputs temp;
            temp.SetVariableO(variable);
            
            if(firstLetter == 'I') {
                
                temp.SetDataTypeO('I');
                firstNum = type.at(3);
                
                if((firstNum == '1')&& (len == 4)){
                    temp.SetDataWidthO(1);
                }
                else if(firstNum == '2') {
                    temp.SetDataWidthO(2);
                }
                else if(firstNum == '4'){
                    temp.SetDataWidthO(4);
                }
                else if(firstNum == '8'){
                    temp.SetDataWidthO(8);
                }
                else if((firstNum == '1') && (len == 5)){
                    temp.SetDataWidthO(16);
                }
                else if(firstNum == '3') {
                    temp.SetDataWidthO(32);
                }
                else if(firstNum == '6') {
                    temp.SetDataWidthO(64);
                }
                _outputs.push_back(temp);
            }//end of signed int
            
            else if(firstLetter == 'U'){
                temp.SetDataTypeO('U');
                firstNum = type.at(4);
                if((firstNum == '1')&& (len == 5)){
                    temp.SetDataWidthO(1);
                }
                else if(firstNum == '2') {
                    temp.SetDataWidthO(2);
                }
                else if(firstNum == '4'){
                    temp.SetDataWidthO(4);
                }
                else if(firstNum == '8'){
                    temp.SetDataWidthO(8);
                }
                else if((firstNum == '1') && (len == 6)){
                    temp.SetDataWidthO(16);
                }
                else if(firstNum == '3') {
                    temp.SetDataWidthO(32);
                }
                else if(firstNum == '6') {
                    temp.SetDataWidthO(64);
                }
                _outputs.push_back(temp);
            }//end of unsigned int input
            check3 = iow;
            check4 = type;
            while(extra != "") {
                temp.SetVariableO(extra);
                _outputs.push_back(temp);
                lineStream >> extra;
                if(extra == temp.GetVariableO()){
                    break;
                }
            }
        }// end o
        
        
        //WIRES
        if((iow == "wire") && ((iow != check5) || (type != check6))){
            Wires temp;
            temp.SetVariableW(variable);
            
            if(firstLetter == 'I') {
                
                temp.SetDataTypeW('I');
                firstNum = type.at(3);
                
                if((firstNum == '1')&& (len == 4)){
                    temp.SetDataWidthW(1);
                }
                else if(firstNum == '2') {
                    temp.SetDataWidthW(2);
                }
                else if(firstNum == '4'){
                    temp.SetDataWidthW(4);
                }
                else if(firstNum == '8'){
                    temp.SetDataWidthW(8);
                }
                else if((firstNum == '1') && (len == 5)){
                    temp.SetDataWidthW(16);
                }
                else if(firstNum == '3') {
                    temp.SetDataWidthW(32);
                }
                else if(firstNum == '6') {
                    temp.SetDataWidthW(64);
                }
                _wires.push_back(temp);
            }//end of signed int
            
            else if(firstLetter == 'U'){
                temp.SetDataTypeW('U');
                firstNum = type.at(4);
                if((firstNum == '1')&& (len == 5)){
                    temp.SetDataWidthW(1);
                }
                else if(firstNum == '2') {
                    temp.SetDataWidthW(2);
                }
                else if(firstNum == '4'){
                    temp.SetDataWidthW(4);
                }
                else if(firstNum == '8'){
                    temp.SetDataWidthW(8);
                }
                else if((firstNum == '1') && (len == 6)){
                    temp.SetDataWidthW(16);
                }
                else if(firstNum == '3') {
                    temp.SetDataWidthW(32);
                }
                else if(firstNum == '6') {
                    temp.SetDataWidthW(64);
                }
                _wires.push_back(temp);
            }//end of unsigned int input
            check3 = iow;
            check4 = type;
            while(extra != "") {
                temp.SetVariableW(extra);
                _wires.push_back(temp);
                lineStream >> extra;
                if(extra == temp.GetVariableW()){
                    break;
                }
            }
        }// end wires
        
        
        //EQUATIONS
        
        //lineStream >> iow >> type >> variable >> extra;
        if (type == "="){
            Equations temp;
            temp.SetOut(iow);
            temp.SetFirst(variable);
            temp.SetOperation(extra);
            if(extra == "?"){
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
        
        //end equations
    }
}


void verilogSim::TestValid() {
    
    unsigned long int lenI, lenO, lenW, lenE;
    unsigned int i = 0, j = 0;
    
    Equations temp;
    lenE = _equations.size();
    while(i<lenE){
        temp = _equations.at(i);
    
      if((temp.GetOperation() != "+") || (temp.GetOperation() != "-") ||(temp.GetOperation() != "*") ||(temp.GetOperation() != ">") || (temp.GetOperation() != "") || (temp.GetOperation() != "<") || (temp.GetOperation() != "?") || (temp.GetOperation() != ">>") || (temp.GetOperation() != "<<") || (temp.GetOperation() != "==")){ //here the equations is invalid if the operator so we just need to say invalid or something idk
        //what teh requirements are
         }
        i++;
    }
    
    i = 0;
    int check = 0;
    
    lenI = _inputs.size();
    lenO = _outputs.size();
    lenW = _wires.size();
    lenE = _equations.size();
    
    Inputs tempI;
    Outputs tempO;
    Wires tempW;
   //we have to loop through each equations first and second variable and
    // check it with every output,input, and wire wariable
    //if we find it in one of those three we are good if not its invalid
    
    while(i < lenE) {
        temp = _equations.at(i);
        while(j<lenI){
            if((temp.GetFirst() == tempI.GetVariableI()) || (temp.GetSecond() == tempI.GetVariableI())){
                check = 1;
            }
            j++;
        }
        j=0;
        while(j<lenO){
            if((temp.GetFirst() == tempO.GetVariableO()) || (temp.GetSecond() == tempO.GetVariableO())){
                check = 1;
            }
            j++;
        }
        j=0;
        while(j<lenO){
            if((temp.GetFirst() == tempO.GetVariableO()) || (temp.GetSecond() == tempO.GetVariableO())){
                check = 1;
            }
            j++;
        }
        j=0;
        if(check == 0){
            //here one of the equations is using an input/output/wire that isnt declared
            // so we need to output that error accordingly
            
        }
        i++;
    }
    
    
}

void verilogSim::WriteCommandsToFile() {
    
    
    ifstream outputFileStream;
    stringstream outputFile;
    outputFile << _outputFileName;
    outputFileStream.open(outputFile.str());
    
    //okey the file to write to is open here
    //now we just have to write to the file based off the 4 vectors
    //if type = "I" or "U" print out int or uint for that line
    //subtract one from the datawidth to put into the brackets in the brackets of verilog
    //and add the variable to the end
    
    //so on and so forth for the outputs wires and equaitons
    
    //and somewhere we have to check the bitwidth of things inorder to concatanate the smaller ones
    


}
