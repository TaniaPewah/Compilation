//
// Created by tt on 5/22/2020.
//

#ifndef HW3_SYMBOLTABLE_H
#define HW3_SYMBOLTABLE_H

#include <iostream>
#include <map>
#include <string>

using namespace std;

class TableEntry
{};



class SymbolTable {
public:
    vector <Scope> stack;

    void newScope(){}
    void endScope(){}
};

class Scope{

    map<string, TableEntry> entries;
    Scope(){}
}


#endif //HW3_SYMBOLTABLE_H
