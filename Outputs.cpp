//
//  Outputs.cpp
//  dpgen
//
//  Created by Ian Hooks on 10/29/19.
//  Copyright © 2019 Ian Hooks. All rights reserved.
//

#include "Outputs.hpp"

Outputs::Outputs(char dataTypeO, unsigned int dataWidthO, string variableO) {
    
    _dataTypeO = dataTypeO;
    _dataWidthO = dataWidthO;
    _variableO = variableO;
    
}

Outputs::Outputs(){
    
    SetDataTypeO('0');
    SetDataWidthO(0.0);
    SetVariableO("0");
    
    _dataTypeO = '0';
    _dataWidthO = 0;
    _variableO = "0";
    
}

char Outputs::GetDataTypeO() const {
    char dataTypeO = _dataTypeO;
    return dataTypeO;
}

void Outputs::SetDataTypeO(char dataTypeO) {
    _dataTypeO = dataTypeO;
}

unsigned int Outputs::GetDataWidthO() const {
    unsigned int dataWidthO = _dataWidthO;
    return dataWidthO;
}
void Outputs::SetDataWidthO(unsigned int dataWidthO) {
    _dataWidthO = dataWidthO;
}

string Outputs::GetVariableO() const {
    string variableO = _variableO;
    return variableO;
}

void Outputs::SetVariableO(string variableO) {
    _variableO = variableO;
}

unsigned int Outputs::GetTimeSlotASAPO() const {
	int timeSlotASAPO = _timeSlotASAPO;
	return timeSlotASAPO;
}

void Outputs::SetTimeSlotASAPO(unsigned int timeSlotASAPO) {
	_timeSlotASAPO = timeSlotASAPO;
}

unsigned int Outputs::GetTimeSlotALAPO() const {
	int timeSlotALAPO = _timeSlotALAPO;
	return timeSlotALAPO;
}

void Outputs::SetTimeSlotALAPO(unsigned int timeSlotALAPO) {
	_timeSlotALAPO = timeSlotALAPO;
}

unsigned int Outputs::GetTimeSlotListRO() const {
	int timeSlotListRO = _timeSlotListRO;
	return timeSlotListRO;
}

void Outputs::SetTimeSlotListRO(unsigned int timeSlotListRO) {
	_timeSlotListRO = timeSlotListRO;
}