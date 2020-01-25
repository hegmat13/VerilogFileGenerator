//
//  Inputs.hpp
//  dpgen
//
//  Created by Ian Hooks on 10/29/19.
//  Copyright © 2019 Ian Hooks. All rights reserved.
//

#ifndef inputs_hpp
#define inputs_hpp

#include <stdio.h>
#include <string.h>
#include <string>

using namespace std;

class Inputs{
private:
    char _dataTypeI;
    unsigned int _dataWidthI;
    string _variableI;
	unsigned int _timeSlotASAPI; 
	unsigned int _timeSlotALAPI;
	unsigned int _timeSlotListRI;
public:
    Inputs(char dataTypeI, unsigned int dataWidthI, string variableI);
    Inputs();
    
    char GetDataTypeI() const;
    void SetDataTypeI(char dataTypeI);
    unsigned int GetDataWidthI() const;
    void SetDataWidthI(unsigned int dataWidthI);
    string GetVariableI() const;
    void SetVariableI(string variableI);
	unsigned int GetTimeSlotASAPI() const;
	void SetTimeSlotASAPI(unsigned int timeSlotASAPI);
	unsigned int GetTimeSlotALAPI() const;
	void SetTimeSlotALAPI(unsigned int timeSlotListRI);
	unsigned int GetTimeSlotListRI() const;
	void SetTimeSlotListRI(unsigned int timeSlotListRI);

};

#endif /* Inputs_hpp */
