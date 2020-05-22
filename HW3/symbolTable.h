//
// Created by tt on 5/22/2020.
//

#ifndef HW3_SYMBOLTABLE_H
#define HW3_SYMBOLTABLE_H
#include "parser.hpp"

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;


class SymbolTable {
public:
    vector <Scope*> stack;

    SymbolTable() {}

    void newScope(){

        Scope* scopeToAdd;
        if (stack.empty()){
            scopeToAdd = new Scope(0);
        }
        else{
            scopeToAdd = new Scope(stack.back()->offset);
        }
        stack.push_back(scopeToAdd);
    }
    void endScope(){}

    void addSymbolVar( Node symbolToAdd ){
        stack.back()->addSymbolVar( symbolToAdd );
    }
    void addSymbolFunc( Node symbolToAdd ){
        stack.back()->addSymbolFunc( symbolToAdd );
    }
};

class Scope{
public:
    map<string, TableEntry> entries;
    int offset;

    Scope(int offset) : offset(offset) {}

    void addSymbolVar( Node symbolToAdd ){
        TableEntry* entryToAdd = new TableEntry( symbolToAdd );
        entries.emplace ( symbolToAdd.name, entryToAdd );
    }
    void addSymbolFunc( Node funcToAdd ){
        TableEntry* entryToAdd = new TableEntry( funcToAdd );
        entries.emplace ( funcToAdd.name, entryToAdd );
    }
};

class TableEntry{
public:

    string name;
    TableEntry( Node newNode ){
        name = newNode.name;
        cout<<"new table entry added: "<<name<<endl;
    }

};

#endif //HW3_SYMBOLTABLE_H
