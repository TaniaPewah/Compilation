//
// Created by tt on 5/22/2020.
//

#ifndef HW3_SYMBOLTABLE_H
#define HW3_SYMBOLTABLE_H
#include "parser.hpp"
#include <map>
#include <vector>

using namespace std;


class TableEntry{
public:

    string name;
    TableEntry( Node* newNode ){
        name = newNode->name;
        cout<<"--------new table entry added: "<<name<<endl;
    }

};

class Scope{
public:
    map<string, TableEntry*> entries;
    int offset;

    Scope(int offset) : offset(offset) {}

    void addSymbolVar( Node* symbolToAdd ){
        TableEntry* entryToAdd = new TableEntry( symbolToAdd );
        entries.insert(make_pair(symbolToAdd->name, entryToAdd));
    }
    void addSymbolFunc( Node* funcToAdd ){
        TableEntry* entryToAdd = new TableEntry( funcToAdd );
        entries.insert(make_pair(funcToAdd->name, entryToAdd)); 
    }
};

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

    void addSymbolVar( Node* symbolToAdd ){
        stack.back()->addSymbolVar( symbolToAdd );
        cout << "~~~~~~~~~~~~~~~~~~~~~ added var to symbol table " << symbolToAdd->name << endl;
    }
    void addSymbolFunc( Node* symbolToAdd ){
        stack.back()->addSymbolFunc( symbolToAdd );
        cout << "~~~~~~~~~~~~~~~~~~~~~ added func to symbol table " << symbolToAdd->name << endl;
    }
};

#endif //HW3_SYMBOLTABLE_H
