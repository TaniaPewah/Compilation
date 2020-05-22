//
// Created by tt on 5/22/2020.
//

#ifndef HW3_SYMBOLTABLE_H
#define HW3_SYMBOLTABLE_H
using namespace std;

class SymbolTable {
public:
    vector <Scope> stack;

    void newScope(){}
    void endScope(){}
}

class Scope{

    map <string, TableEntry> entries;
    Scope(){}
}


#endif //HW3_SYMBOLTABLE_H
