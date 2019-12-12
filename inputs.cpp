//
//  inputs.cpp
//  dpgen
//
//  Created by Ian Hooks on 10/29/19.
//  Copyright © 2019 Ian Hooks. All rights reserved.
//

#include "inputs.hpp"


inputs::inputs(char dataTypeI, unsigned int dataWidthI, string variableI) {
    
    _dataTypeI = dataTypeI;
    _dataWidthI = dataWidthI;
    _variableI = variableI;
    
}

inputs::inputs(){
    
    SetDataTypeI('0');
    SetDataWidthI(0.0);
    SetVariableI("0");
    
    _dataTypeI = '0';
    _dataWidthI = 0;
    _variableI = "0";
    
}

char inputs::GetDataTypeI() const {
    char dataTypeI = _dataTypeI;
    return dataTypeI;
}

void inputs::SetDataTypeI(char dataTypeI) {
    _dataTypeI = dataTypeI;
}

unsigned int inputs::GetDataWidthI() const {
    unsigned int dataWidthI = _dataWidthI;
    return dataWidthI;
}
void inputs::SetDataWidthI(unsigned int dataWidthI) {
    _dataWidthI = dataWidthI;
}

string inputs::GetVariableI() const {
    string variableI = _variableI;
    return variableI;
}

void inputs::SetVariableI(string variableI) {
    _variableI = variableI;
}

unsigned int inputs::GetTimeSlotASAPI() const {
	int timeSlotASAPI = _timeSlotASAPI;
	return timeSlotASAPI;
}

void inputs::SetTimeSlotASAPI(unsigned int timeSlotASAPI) {
	_timeSlotASAPI = timeSlotASAPI;
}

unsigned int inputs::GetTimeSlotALAPI() const {
	int timeSlotALAPI = _timeSlotALAPI;
	return timeSlotALAPI;
}

void inputs::SetTimeSlotALAPI(unsigned int timeSlotALAPI) {
	_timeSlotALAPI = timeSlotALAPI;
}

unsigned int inputs::GetTimeSlotListRI() const {
	int timeSlotListRI = _timeSlotListRI;
	return timeSlotListRI;
}

void inputs::SetTimeSlotListRI(unsigned int timeSlotListRI) {
	_timeSlotListRI = timeSlotListRI;
}
