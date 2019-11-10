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
#include <algorithm>

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
                extra.at(extra.length()-1) =  ' ';
            }
        }
        if(variable != ""){
            if(variable.at(variable.length()-1) == ','){
                variable.at(variable.length()-1) =  ' ';
            }
        }
        firstLetter = type.at(0);
        len = type.length();

        //INPUTS
        if((iow == "input") && ((iow != check1) || (type != check2))){
            Inputs temp;
            temp.SetVariableI(*variable.c_str());

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
                temp.SetVariableI(*extra.c_str());
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
                //COMMENT
                //cout << "entered data width loop for signed" << endl;

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
                //COMMENT
                //cout << "entered data width loop for UNsigned" << endl;

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
            //COMMENT ME OUT LATER
            //cout << temp.GetVariableW() << " with bitwidth "<< temp.GetDataWidthW() << endl;
            check3 = iow;
            check4 = type;
            while(extra != "") {
                temp.SetVariableW(extra);
                _wires.push_back(temp);
                lineStream >> extra;
                //COMMENT ME OUT LATER
                //cout << temp.GetVariableW() << " with bitwidth "<< temp.GetDataWidthW() << endl;
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

    inputFileStream.close();
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
            return;
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
    // check it with every output,input, and wire variable
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
            return;
        }
        i++;
    }
}

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
    outputFile << "module circuitN(";

    //loop to add in all inputs and outputs to the module declaration
    //this is all the first line of the .v file
    for (auto a = 0; a < _inputs.size(); a++) {
        outputFile << _inputs.at(a).GetVariableI() << ", ";
    }
    for (auto b = 0; b < _outputs.size() - 1; b++) {
        outputFile << _outputs.at(b).GetVariableO() << ", ";
    }
    outputFile << _outputs.back().GetVariableO() << ");" << endl;

    //now we begin adding the inputs, they're all individual for now
    //if you figure out how to group them all by sizes props
    for (auto & _input : _inputs) {
        outputFile << "input ";

        //tack on signed if the type is signed, unsigneds don't need to be declared
        if (_input.GetDataTypeI() == 'I') {
            outputFile << "signed ";
        }

        //add in the bitwidth part of the text
        if (_input.GetDataWidthI() == 1) {
            outputFile << _input.GetVariableI() << ";" << endl;
        } else {
            outputFile << "[" << std::to_string(_input.GetDataWidthI() - 1) << ":0] " << _input.GetVariableI() << ";" << endl;
        }
    }

    //outputs are also all on their own
    for (auto & _output : _outputs) {
        outputFile << "output reg ";

        //tack on signed if the type is signed
        if (_output.GetDataTypeO() == 'I') {
            outputFile << "signed ";
        }

        //add in the bitwidth part
        if (_output.GetDataWidthO() == 1) {
            outputFile << _output.GetVariableO() << ";" << endl;
        } else {
            outputFile << "[" << std::to_string(_output.GetDataWidthO() - 1) << ":0] " << _output.GetVariableO() << ";" << endl;
        }
    }

    //wires are also all on their own
    for (auto w = 0; w < _wires.size() - 1; w++) {
        outputFile << "wire ";

        //tack on signed if the type is signed
        if (_wires.at(w).GetDataTypeW() == 'I') {
            outputFile << "signed ";
        }

        //add in the bitwidth part
        if (_wires.at(w).GetDataWidthW() == 1) {
            outputFile << _wires.at(w).GetVariableW() << ";" << endl;
        } else {
            outputFile << "[" << std::to_string(_wires.at(w).GetDataWidthW() - 1) << ":0] " << _wires.at(w).GetVariableW() << ";" << endl;
        }
    }

    //start printing the equations
    for (auto f = 0; f < _equations.size(); f++) {
        //format is generally (changes for certain ops):
        //      ADD #(32) Add9(l00, l01, l10);

        //these variables are for getting the bitwidth that is placed in parentheses like #(64)
        auto first = _equations.at(f).GetFirst();
        auto second = _equations.at(f).GetSecond();
        auto out = _equations.at(f).GetOut();
        auto muxSel = _equations.at(f).GetMuxSel();
        auto fWidth = 0;
        auto sWidth = 0;
        auto bitWidth = 0;

//        cout << _inputs.at(0).GetVariableI() << endl;
//        cout << _inputs.at(1).GetVariableI() << endl;
//        cout << _inputs.at(0).GetDataWidthI() << endl;
//        cout << _inputs.at(1).GetDataWidthI() << endl;

        //compare the first and second operands to find their bitwidths
        //starting with an inputs comparison
        for (auto & _input : _inputs) {
            if (first == _input.GetVariableI()) {
                fWidth = _input.GetDataWidthI();
                //cout << "first got: " << _inputs.at(g).GetDataWidthI() << endl;
            }
            if (second == _input.GetVariableI()) {
                sWidth = _input.GetDataWidthI();
                //cout << "second got: " << _inputs.at(g).GetDataWidthI() << endl;
            }
        }
        //next an outputs comparison
        for (auto & _output : _outputs) {
//            if (out == _outputs.at(h).GetVariableO()) {
//                cout << _outputs.at(0).GetVariableO() << endl;
//                cout << "it entered the output check loop!" << endl;
//            }

            if (first == _output.GetVariableO()) {
                fWidth = _output.GetDataWidthO();
            }
            if (second == _output.GetVariableO()) {
                sWidth = _output.GetDataWidthO();
            }
        }
        //finally a wires comparison
        for (auto & _wire : _wires) {
            auto temp1Width = _wire.GetDataWidthW();
            if (first == _wire.GetVariableW()) {
                //fWidth = _wire.GetDataWidthW();
                fWidth = temp1Width;
                //cout << "width for first is " << fWidth << endl;
            }
            auto temp2Width = _wire.GetDataWidthW();
            if (second == _wire.GetVariableW()) {
                //sWidth = _wire.GetDataWidthW();
                sWidth = temp2Width;
                //cout << "width for second is " << sWidth << endl;
            }
        }

        //now we do a comparison to find the bigger bitwidth
        bitWidth = std::max(fWidth, sWidth);

        cout << "first Width is currently " << fWidth << endl;
        cout << "second Width is currently " << sWidth << endl;
        cout << "bitWidth is currently " << bitWidth << endl;

        //now a big ol chain for checking what the operation is and writing it
        if (_equations.at(f).GetOperation() == "+") {
            outputFile << "ADD #(" << bitWidth << ") Add" << f << "(" << first << ", " << second << ", " << out << ");" << endl;
            //cout << "ADD #(" << bitWidth << ") " << "Add" << f << "(" << first << ", "<< second << ", " << out << ");" << endl;
        } else if (_equations.at(f).GetOperation() == "-") {
            outputFile << "SUB #(" << bitWidth << ") Sub" << f << "(" << first << ", " << second << ", " << out << ");" << endl;
        } else if (_equations.at(f).GetOperation() == "*") {
            outputFile << "MUL #(" << (bitWidth * 2) << ") Mult" << f << "(" << first << ", " << second << ", " << out << ");" << endl;
        } else if (_equations.at(f).GetOperation() == ">") {
            outputFile << "COMP #(" << bitWidth << ") Comp" << f << "(" << first << ", " << second << ", " << out << ", " <<
                       "unconnected_received_data" << ", " << "unconnected_received_data" << ");" << endl;
        } else if (_equations.at(f).GetOperation() == "<") {
            outputFile << "COMP #(" << bitWidth << ") Comp" << f << "(" << first << ", " << second << ", "  <<
                       "unconnected_received_data" << ", " << out << ", " << "unconnected_received_data" << ");" << endl;
        } else if (_equations.at(f).GetOperation() == "==") {
            outputFile << "COMP #(" << bitWidth << ") Comp" << f << "(" << first << ", " << second <<  ", " <<
                       "unconnected_received_data" << ", " << "unconnected_received_data" << ", " << out << ");" << endl;
        } else if (_equations.at(f).GetOperation() == "?") {
            //FIX ME
            outputFile << "MUX2x1 #(" << bitWidth << ") Mux" << f << "(" << first << ", " << second << ", " << muxSel << ", " << out << ");" << endl;
        } else if (_equations.at(f).GetOperation() == ">>") {
            //FIX ME
            outputFile << "SHR #(" << bitWidth << ") Shr" << f << "(" << first << ", " << second << ", " << out << ");" << endl;
        } else if (_equations.at(f).GetOperation() == "<<") {
            //FIX ME
            outputFile << "SHL #(" << bitWidth << ") Shl" << f << "(" << first << ", " << second << ", " << out << ");" << endl;
        } else if (_equations.at(f).GetOperation() == "") {
            outputFile << "REG #(" << bitWidth << ") Reg" << f << "(" << first << ", " << "Clk, Rst, " << out << ");" << endl;
        }
    }

    //last line of the .v file
    outputFile << "endmodule";

    //close the fstream after writing the file
    outputFile.close();
}
