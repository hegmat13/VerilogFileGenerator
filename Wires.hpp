//
//  Wires.hpp
//  dpgen
//
//  Created by Ian Hooks on 10/29/19.
//  Copyright © 2019 Ian Hooks. All rights reserved.
//

#ifndef Wires_hpp
#define Wires_hpp

#include <stdio.h>
#include <string>

using namespace std;

class Wires{
private:
    char _dataTypeW;
    unsigned int _dataWidthW;
    string _variableW;
	unsigned int _timeSlotASAPW;
	unsigned int _timeSlotALAPW;
	unsigned int _timeSlotListRW;

public:
    Wires(char dataTypeW, unsigned int dataWidthW, string variableW);
    Wires();
    
    char GetDataTypeW() const;
    void SetDataTypeW(char dataTypeW);
    unsigned int GetDataWidthW() const;
    void SetDataWidthW(unsigned int dataWidthW);
    string GetVariableW() const;
    void SetVariableW(string variableW);
	unsigned int GetTimeSlotASAPW() const;
	void SetTimeSlotASAPW(unsigned int timeSlotASAPW);
	unsigned int GetTimeSlotALAPW() const;
	void SetTimeSlotALAPW(unsigned int timeSlotALAPW);
	unsigned int GetTimeSlotListRW() const;
	void SetTimeSlotListRW(unsigned int timeSlotListRW);

};

#endif /* Wires_hpp */
