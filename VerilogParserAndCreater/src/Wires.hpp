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
public:
    Wires(char dataTypeW, unsigned int dataWidthW, string variableW);
    Wires();
    
    char GetDataTypeW() const;
    void SetDataTypeW(char dataTypeW);
    unsigned int GetDataWidthW() const;
    void SetDataWidthW(unsigned int dataWidthW);
    string GetVariableW() const;
    void SetVariableW(string variableW);
};

#endif /* Wires_hpp */
