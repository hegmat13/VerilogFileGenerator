//
//  Inputs.hpp
//  dpgen
//
//  Created by Ian Hooks on 10/29/19.
//  Copyright © 2019 Ian Hooks. All rights reserved.
//

#ifndef Inputs_hpp
#define Inputs_hpp

#include <stdio.h>
#include <string.h>
#include <string>

using namespace std;

class Inputs{
private:
    char _dataTypeI;
    unsigned int _dataWidthI;
    string _variableI;
public:
    Inputs(char dataTypeI, unsigned int dataWidthI, string variableI);
    Inputs();
    
    char GetDataTypeI() const;
    void SetDataTypeI(char dataTypeI);
    unsigned int GetDataWidthI() const;
    void SetDataWidthI(unsigned int dataWidthI);
    string GetVariableI() const;
    void SetVariableI(char variableI);
};

#endif /* Inputs_hpp */
