//
//  Wires.cpp
//  dpgen
//
//  Created by Ian Hooks on 10/29/19.
//  Copyright © 2019 Ian Hooks. All rights reserved.
//

#include "Wires.hpp"
#include <string>

Wires::Wires(char dataTypeW, unsigned int dataWidthW, string variableW) {
    
    _dataTypeW = dataTypeW;
    _dataWidthW = dataWidthW;
    _variableW = variableW;
}

Wires::Wires(){
    
    SetDataTypeW('0');
    SetDataWidthW(0.0);
    SetVariableW("0");
    
    _dataTypeW = '0';
    _dataWidthW = 0;
    _variableW = "0";
}

char Wires::GetDataTypeW() const {
    char dataTypeW = _dataTypeW;
    return dataTypeW;
}

void Wires::SetDataTypeW(char dataTypeW) {
    _dataTypeW = dataTypeW;
}

unsigned int Wires::GetDataWidthW() const {
    unsigned int dataWidthW = _dataWidthW;
    return dataWidthW;
}
void Wires::SetDataWidthW(unsigned int dataWidthW) {
    _dataWidthW = dataWidthW;
}

string Wires::GetVariableW() const {
    string variableW = _variableW;
    return variableW;
}

void Wires::SetVariableW(string variableW) {
    _variableW = variableW;
}
