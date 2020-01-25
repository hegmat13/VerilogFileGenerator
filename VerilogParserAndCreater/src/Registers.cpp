//
//  Registers.cpp
//  dpgen
//
//  Created by Ian Hooks on 11/14/19.
//  Copyright © 2019 Ian Hooks. All rights reserved.
//

#include "Registers.hpp"

Registers::Registers(char dataTypeR, unsigned int dataWidthR, string variableR) {
    
    _dataTypeR = dataTypeR;
    _dataWidthR = dataWidthR;
    _variableR = variableR;
    
}

Registers::Registers(){
    
    SetDataTypeR('0');
    SetDataWidthR(0.0);
    SetVariableR("0");
    
    _dataTypeR = '0';
    _dataWidthR = 0;
    _variableR = "0";
    
}

char Registers::GetDataTypeR() const {
    char dataTypeR = _dataTypeR;
    return dataTypeR;
}

void Registers::SetDataTypeR(char dataTypeR) {
    _dataTypeR = dataTypeR;
}

unsigned int Registers::GetDataWidthR() const {
    unsigned int dataWidthR = _dataWidthR;
    return dataWidthR;
}
void Registers::SetDataWidthR(unsigned int dataWidthR) {
    _dataWidthR = dataWidthR;
}

string Registers::GetVariableR() const {
    string variableR = _variableR;
    return variableR;
}

void Registers::SetVariableR(string variableR) {
    _variableR = variableR;
}
