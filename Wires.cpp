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

unsigned int Wires::GetTimeSlotASAPW() const {
	int timeSlotASAPW = _timeSlotASAPW;
	return timeSlotASAPW;
}

void Wires::SetTimeSlotASAPW(unsigned int timeSlotASAPW) {
	_timeSlotASAPW = timeSlotASAPW;
}

unsigned int Wires::GetTimeSlotALAPW() const {
	int timeSlotALAPW = _timeSlotALAPW;
	return timeSlotALAPW;
}

void Wires::SetTimeSlotALAPW(unsigned int timeSlotALAPW) {
	_timeSlotALAPW = timeSlotALAPW;
}

unsigned int Wires::GetTimeSlotListRW() const {
	int timeSlotListRW = _timeSlotListRW;
	return timeSlotListRW;
}

void Wires::SetTimeSlotListRW(unsigned int timeSlotListRW) {
	_timeSlotListRW = timeSlotListRW;
}