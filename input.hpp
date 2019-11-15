//
//
//  input.hpp
//  dpgen
//
//  Created by Ian Hooks on 10/29/19.
//  Copyright © 2019 Ian Hooks. All rights reserved.
//

#ifndef input_hpp
#define input_hpp

#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Inputs.hpp"
#include "Outputs.hpp"
#include "Wires.hpp"
#include "Equations.hpp"
#include "Registers.hpp"

class verilogSim {
private: 
    std::vector<Inputs> _inputs;
    std::vector<Outputs> _outputs;
    std::vector<Wires> _wires;
    std::vector<Equations> _equations;
    std::vector<Registers> _registers;
    char* _inputFileName;
    char* _outputFileName;
    
public:
    void run(char* _inputFileName, char* _outputFileName);
    void ReadCommandsFromFile();
    void TestValid();
    void WriteCommandsToFile();

};

#endif /* input_hpp */
