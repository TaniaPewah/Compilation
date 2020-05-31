//
// Created by tt on 5/22/2020.
//

#ifndef HW3_SYMBOLTABLE_H
#define HW3_SYMBOLTABLE_H
#include "parser.hpp"
#include "hw3_output.hpp"
#include <map>
#include <vector>
#include <cctype> 

using namespace std;


class TableEntry{
public:

    VarNode* node;
    bool is_var;
    int offset;

    TableEntry( VarNode* newNode, bool is_var, int offset=0 ) : node(newNode), is_var(is_var), offset(offset) {
    }
};

class Scope{
public:
    map<string, TableEntry*> entries;
    int offset;
    string func_name;
    bool is_func_scope;

    Scope(int offset) : offset(offset), func_name(""), is_func_scope(false) {}
    Scope(int offset, string func_name) : offset(offset), func_name(func_name), is_func_scope(true) {}

    void addSymbolVar( VarNode* symbolToAdd ){
        
        TableEntry* entryToAdd = new TableEntry( symbolToAdd, true, offset );
        offset++;
        
        entries.insert(make_pair(symbolToAdd->name, entryToAdd));
    }

    void addSymbolVarForFunction( VarNode* symbolToAdd , int wanted_offset){
        TableEntry* entryToAdd = new TableEntry( symbolToAdd, true, wanted_offset );
        entries.insert(make_pair(symbolToAdd->name, entryToAdd));
    }

    void addSymbolFunc( FuncNode* funcToAdd ){
        TableEntry* entryToAdd = new TableEntry( funcToAdd, false );
        entries.insert(make_pair(funcToAdd->name, entryToAdd)); 
    }

    VarNode* findSymbolInScope(string key) {
        //cout << "entries in current scope are: " << endl;

        for(auto key = entries.begin(); key != entries.end(); key++) {
           //cout << key->first << " : " << key->second->node->name << ", " << key->second->node->type << "        ";
        }
        //cout << endl;

        map<string, TableEntry*>::iterator wanted_entry = entries.find(key);
        if (wanted_entry != entries.end()){
            return wanted_entry->second->node;
        }
        return NULL;
    }

    string toUpper(string lower_type){

        string returned;
        
        // convert string to upper case
        for(char lower_char : lower_type){
            returned.push_back(toupper(lower_char));
        }
        return returned;
    }

    void endScope(){
        output::endScope();
        //cout << "++++++ Trying to end scope: "<< func_name << "++++++" << endl;
      
        for (pair<string, TableEntry*> entry : entries) {
 
            string id = entry.first;
            TableEntry* entry_value = entry.second;

            output::printID( id, entry_value->offset, toUpper(entry_value->node->type));

            // TODO is its a func print name, type, params, offset
            // if its a var print name type offset
        }
    }
};

class SymbolTable {
public:
    vector <Scope*> stack;
    bool has_main = false;

    SymbolTable() {}

    void newScope(){
         

        Scope* scopeToAdd;
        if (stack.empty()){
            scopeToAdd = new Scope(0);
            //cout << "--First scope was created--" << endl;
        }
        else{
            scopeToAdd = new Scope(stack.back()->offset);
            //cout << "--New scope was created--" << endl;
        }
        stack.push_back(scopeToAdd);
    }
    void closeScope(){
        stack.back()->endScope();
        stack.pop_back();
    }

    void addSymbolVar( VarNode* symbolToAdd){
        if(!ifExists(symbolToAdd->name)){
            stack.back()->addSymbolVar( symbolToAdd);
            //cout << "~~~~~~~~~~~~~~~~~~~~~ added var to symbol table " << symbolToAdd->name << endl;
        }
        else{
            output::errorDef(symbolToAdd->lineno, symbolToAdd->name);
            exit(0);
        }
    }

    void addSymbolVarForFunction( VarNode* symbolToAdd , int wanted_offset) {
        if(!ifExists(symbolToAdd->name)){
            stack.back()->addSymbolVarForFunction( symbolToAdd, wanted_offset );
            //cout << "~~~~~~~~~~~~~~~~~~~~~ added var to symbol table " << symbolToAdd->name << endl;
        }
        else{
            output::errorDef(symbolToAdd->lineno, symbolToAdd->name);
            exit(0);
        }
    }
    
    void addSymbolFunc( FuncNode* func_to_add ){
        if(!ifExists(func_to_add->name)){
            stack.front()->addSymbolFunc( func_to_add );
            
            if (func_to_add->name == "main" && 
                func_to_add->type == "void" && 
                func_to_add->params.size() == 0) {

                this->has_main = true;
            }  
            //cout << "~~~~~~~~~~~~~~~~~~~~~ added func to symbol table " << func_to_add->name << endl;
        }
        else{

            output::errorDef(func_to_add->lineno, func_to_add->name);
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


    bool hasMain(){
        return has_main;
    }
};

#endif //HW3_SYMBOLTABLE_H
