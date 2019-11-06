//
//  Equations.hpp
//  dpgen
//
//  Created by Ian Hooks on 10/29/19.
//  Copyright © 2019 Ian Hooks. All rights reserved.
//

#ifndef Equations_hpp
#define Equations_hpp

#include <stdio.h>
#include <string>

using namespace std;

class Equations{
private:
    string _out;
    string _operation;
    string _first;
    string _second;
    string _muxSel;

public:
    Equations(string out, string operation, string first, string second, string muxSel);
    Equations();
    
    string GetOut() const;
    void SetOut(string out);
    string GetOperation() const;
    void SetOperation(string operation);
    string GetFirst() const;
    void SetFirst(string first);
    string GetSecond() const;
    void SetSecond(string second);
    string GetMuxSel() const;
    void SetMuxSel(string muxSel);
};

#endif /* Equations_hpp */

