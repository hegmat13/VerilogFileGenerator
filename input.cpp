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
    stringstream outputFile;
    outputFile << _outputFileName;
    ifstream inputFileStream;
    ifstream outputFileStream;
    inputFileStream.open(inputFile.str());
    outputFileStream.open(outputFile.str());
    
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
            temp.SetVariableI(variable.at(0));
            
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
            temp.SetVariableI(extra.at(0));
            _inputs.push_back(temp);
            lineStream >> extra;
            if(extra.at(0) == temp.GetVariableI()){
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
        if (type == "=") {
            Equations temp;
            temp.SetOut(iow);
            temp.SetFirst(variable);
            temp.SetOperation(extra.at(0));
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
        }//end equations
    }
}
