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

    VarNode* node;
    bool is_var;

    TableEntry( VarNode* newNode, bool is_var ) : node(newNode), is_var(is_var) {

        if(is_var){
            cout<<"--------new table Var added: "<< node->name <<endl;
        }
        else{
            cout<<"--------new Function Var added: "<< node->name <<endl;
        }
    }
    //TODO: add FunctionTableEntry

};

class Scope{
public:
    map<string, TableEntry*> entries;
    int offset;

    Scope(int offset) : offset(offset) {}

    void addSymbolVar( VarNode* symbolToAdd ){
        TableEntry* entryToAdd = new TableEntry( symbolToAdd, true );
        entries.insert(make_pair(symbolToAdd->name, entryToAdd));
        offset++;
    }
    void addSymbolFunc( FuncNode* funcToAdd ){
        TableEntry* entryToAdd = new TableEntry( funcToAdd, false );
        entries.insert(make_pair(funcToAdd->name, entryToAdd)); 
        offset++;
    }

    VarNode* findSymbolInScope(string key) {
        cout << "entries in current scope are: " << endl;

        for(auto key = entries.begin(); key != entries.end(); key++) {
           cout << key->first << " : " << key->second->node->name << ", " << key->second->node->type << "        ";
        }
        cout << endl;

        map<string, TableEntry*>::iterator wanted_entry = entries.find(key);
        if (wanted_entry != entries.end()){
            return wanted_entry->second->node;
        }
        return NULL;
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

    void addSymbolVar( VarNode* symbolToAdd ){
        if(!ifExists(symbolToAdd->name)){
            stack.back()->addSymbolVar( symbolToAdd );
            cout << "~~~~~~~~~~~~~~~~~~~~~ added var to symbol table " << symbolToAdd->name << endl;
        }
        else{
            output::errorDef(symbolToAdd->lineno, symbolToAdd->name);
            exit(0);
        }
    }
    void addSymbolFunc( FuncNode* func_to_add ){
        if(!ifExists(func_to_add->name)){
            stack.front()->addSymbolFunc( func_to_add );
            cout << "~~~~~~~~~~~~~~~~~~~~~ added func to symbol table " << func_to_add->name << endl;
        }
        else{
            output::errorUndefFunc(func_to_add->lineno, func_to_add->name);
            exit(0);
        }
    }

    VarNode* findSymbolInStack(string symbol_name){
       
        for(int i = 0; i < stack.size(); i++){
            VarNode* entry = stack[i]->findSymbolInScope(symbol_name);
            if(entry != NULL) {
                return entry;
            }
        }
        return NULL;
    }

    string getIdType(int lineno, string id){

        VarNode* wanted_entry = findSymbolInStack(id);
        if(!wanted_entry) {
            output::errorUndef(lineno, id);
            exit(0);
       }
       return wanted_entry->type;
    }

    bool ifExists(string name){
        return findSymbolInStack(name) ? true : false;
    }
};

#endif //HW3_SYMBOLTABLE_H
