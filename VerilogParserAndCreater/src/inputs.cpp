//
//  Inputs.cpp
//  dpgen
//
//  Created by Ian Hooks on 10/29/19.
//  Copyright © 2019 Ian Hooks. All rights reserved.
//

#include "inputs.hpp"


Inputs::Inputs(char dataTypeI, unsigned int dataWidthI, string variableI) {
    
    _dataTypeI = dataTypeI;
    _dataWidthI = dataWidthI;
    _variableI = variableI;
    
}

Inputs::Inputs(){
    
    SetDataTypeI('0');
    SetDataWidthI(0.0);
    SetVariableI("0");
    
    _dataTypeI = '0';
    _dataWidthI = 0;
    _variableI = "0";
    
}

char Inputs::GetDataTypeI() const {
    char dataTypeI = _dataTypeI;
    return dataTypeI;
}

void Inputs::SetDataTypeI(char dataTypeI) {
    _dataTypeI = dataTypeI;
}

unsigned int Inputs::GetDataWidthI() const {
    unsigned int dataWidthI = _dataWidthI;
    return dataWidthI;
}
void Inputs::SetDataWidthI(unsigned int dataWidthI) {
    _dataWidthI = dataWidthI;
}

string Inputs::GetVariableI() const {
    string variableI = _variableI;
    return variableI;
}

void Inputs::SetVariableI(string variableI) {
    _variableI = variableI;
}
