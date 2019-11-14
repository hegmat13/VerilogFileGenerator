//
//  Registers.hpp
//  dpgen
//
//  Created by Ian Hooks on 11/14/19.
//  Copyright © 2019 Ian Hooks. All rights reserved.
//

#ifndef Registers_hpp
#define Registers_hpp

#include <stdio.h>
#include <string>

using namespace std;

class Registers{
private:
    char _dataTypeR;
    unsigned int _dataWidthR;
    string _variableR;
public:
    Registers(char dataTypeR, unsigned int dataWidthR, string variableR);
    Registers();
    
    char GetDataTypeR() const;
    void SetDataTypeR(char dataTypeR);
    unsigned int GetDataWidthR() const;
    void SetDataWidthR(unsigned int dataWidthR);
    string GetVariableR() const;
    void SetVariableR(string variableR);
};

#endif /* Registers_hpp */

