//
// Created by tt on 5/22/2020.
//

#ifndef HW3_SYMBOLTABLE_H
#define HW3_SYMBOLTABLE_H
#include "parser.hpp"
#include "hw3_output.hpp"
#include <map>
#include <vector>

using namespace std;


class TableEntry{
public:

    string name;
    TableEntry( IdNode* newNode ){
        name = newNode->name;
        cout<<"--------new table entry added: "<<name<<endl;
    }
    //TODO: add FunctionTableEntry

};

class Scope{
public:
    map<string, TableEntry*> entries;
    int offset;

    Scope(int offset) : offset(offset) {}

    void addSymbolVar( IdNode* symbolToAdd ){
        TableEntry* entryToAdd = new TableEntry( symbolToAdd );
        entries.insert(make_pair(symbolToAdd->name, entryToAdd));
        offset++;
    }
    void addSymbolFunc( IdNode* funcToAdd ){
        TableEntry* entryToAdd = new TableEntry( funcToAdd );
        entries.insert(make_pair(funcToAdd->name, entryToAdd)); 
        offset++;
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
            cout << "--First scope was created--" << endl;
        }
        else{
            scopeToAdd = new Scope(stack.back()->offset);
            cout << "--New scope was created--" << endl;
        }
        stack.push_back(scopeToAdd);
    }
    void closeScope(){
        stack.pop_back();
        output::endScope();
    }

    void addSymbolVar( IdNode* symbolToAdd ){
        stack.back()->addSymbolVar( symbolToAdd );
        cout << "~~~~~~~~~~~~~~~~~~~~~ added var to symbol table " << symbolToAdd->name << endl;
    }
    void addSymbolFunc( IdNode* symbolToAdd ){
        stack.back()->addSymbolFunc( symbolToAdd );
        cout << "~~~~~~~~~~~~~~~~~~~~~ added func to symbol table " << symbolToAdd->name << endl;
    }
};

#endif //HW3_SYMBOLTABLE_H
