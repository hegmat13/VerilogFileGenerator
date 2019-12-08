//
//  Outputs.hpp
//  dpgen
//
//  Created by Ian Hooks on 10/29/19.
//  Copyright © 2019 Ian Hooks. All rights reserved.
//

#ifndef Outputs_hpp
#define Outputs_hpp

#include <stdio.h>
#include <string>

using namespace std;

class Outputs{
private:
    char _dataTypeO;
    unsigned int _dataWidthO;
    string _variableO;
	unsigned int _timeSlotASAPO;
	unsigned int _timeSlotALAPO;
	unsigned int _timeSlotListRO;
public:
    Outputs(char dataTypeO, unsigned int dataWidthO, string variableO);
    Outputs();
    
    char GetDataTypeO() const;
    void SetDataTypeO(char dataTypeO);
    unsigned int GetDataWidthO() const;
    void SetDataWidthO(unsigned int dataWidthO);
    string GetVariableO() const;
    void SetVariableO(string variableO);
	unsigned int GetTimeSlotASAPO() const;
	void SetTimeSlotASAPO(unsigned int timeSlotASAPO);
	unsigned int GetTimeSlotALAPO() const;
	void SetTimeSlotALAPO(unsigned int timeSlotALAPO);
	unsigned int GetTimeSlotListRO() const;
	void SetTimeSlotListRO(unsigned int timeSlotListRO);
};
#endif /* Outputs_hpp */
