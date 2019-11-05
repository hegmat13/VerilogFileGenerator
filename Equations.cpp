//
//  Equations.cpp
//  dpgen
//
//  Created by Ian Hooks on 10/29/19.
//  Copyright © 2019 Ian Hooks. All rights reserved.
//

#include "Equations.hpp"

Equations::Equations(string out, char operation, string first, string second, string muxSel) {
    
    _out = out;
    _operation = operation;
    _first = first;
    _second = second;
    _muxSel = muxSel;
}

Equations::Equations(){
    
    SetOut("0");
    SetOperation('0');
    SetFirst("0");
    SetSecond("0");
    SetMuxSel("0");
    
    _out = "0";
    _operation = '0';
    _first = "0";
    _second = "0";
    _muxSel = "0";
}

string Equations::GetOut() const {
    string out = _out;
    return out;
}

void Equations::SetOut(string out) {
    _out = out;
}

char Equations::GetOperation() const {
    char operation = _operation;
    return operation;
}

void Equations::SetOperation(char operation) {
    _operation = operation;
}

string Equations::GetFirst() const {
    string first = _first;
    return first;
}

void Equations::SetFirst(string first) {
    _first = first;
}

string Equations::GetSecond() const {
    string second = _second;
    return second;
}

void Equations::SetSecond(string second) {
    _second = second;
}

string Equations::GetMuxSel() const {
    string muxSel = _muxSel;
    return muxSel;
}

void Equations::SetMuxSel(string muxSel) {
    _muxSel = muxSel;
}


