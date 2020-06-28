//
// Created by tt on 5/22/2020.
//

#ifndef HW3_PARSER_HPP
#define HW3_PARSER_HPP
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
#include "IRManager.hpp"

#ifndef YYINITEPTH
# define YYINITDEPTH 10000
#endif

#define NA -1

static string toUpper(string lower_type){

    string returned;
    
    // convert string to upper case
    for(char lower_char : lower_type){
        returned.push_back(toupper(lower_char));
    }
    return returned;
}

class Node{
    /* This class is the basic node with line num. which all other types inherite from */
    public:
    int lineno;
    Node(int lineno) : lineno(lineno) {};

    string getFreshReg(){
        IRManager* regManager = IRManager::getInstance();
        Register* regID = regManager->getFreshReg();
        cout <<" new reg created name : " << regID->getName() << endl;
        return regID->getName();
    }

    string getGlobalFreshReg(){
        IRManager* regManager = IRManager::getInstance();
        Register* regID = regManager->getGlobalFreshReg();
        cout <<" new global reg created name : " << regID->getName() << endl;
        return regID->getName();
    }
};

class IdNode: public Node{
    /* This class gives basic support all ID's (variable name) */
    public:
    string llvm_reg;
    string name;
    IdNode( int lineno, string name ) : Node(lineno), name(name) {
        this->llvm_reg = this->getFreshReg();
    };
};

class TypeNode : public Node{
    /* This class gives basic support all types */
    public:
    string type_name;
    TypeNode( int lineno, string type ) : Node(lineno), type_name(type) {
    };
};

class NumNode: public TypeNode{
    /* This class will support int variable without a name*/
    public:
    string llvm_reg;
    int value;
    NumNode( int lineno, int value, string type ) : TypeNode(lineno, type), value(value) {
        this->llvm_reg = this->getFreshReg();
    };
};


class ExpNode: public Node{
    /* This class supports the rule expressions, need further checks on CALL methods*/
    public: 
    string type;
    string llvm_reg;
    int true_list_id;
    int false_list_id;

    ExpNode( int lineno, string type ) : Node(lineno), type(type)  {
        cout << " Expnode "<< endl;
        string regt = this->getFreshReg();
        cout << " Expnode, calling freshreg: " << regt << endl;
        llvm_reg = regt;
        cout<< "llvm reg is: "<< llvm_reg << endl;
    };
    
    ExpNode(int lineno, string type, string llvm_reg) : Node(lineno), type(type), llvm_reg(llvm_reg) {
        cout << "created new exp node, with givven register " << llvm_reg << endl;
    }
};


class StringNode: public ExpNode{
    /*This class supports string values without variable*/
    public:
    string value;
    string size;

    StringNode(int lineno, string type, string raw_value) : ExpNode(lineno, type, getGlobalFreshReg()){
        value = raw_value.substr(1, raw_value.size() - 2);
        size = to_string(value.size() + 1);
     }
};


class VarNode: public IdNode{
    /* This class supports variable types, such as int, bool... */
    public:
    string type;
    int stack_offset;
    bool is_function;
    VarNode( int lineno, string name, string type , bool is_function) : IdNode(lineno, name), type(type) , is_function(is_function){
        if(is_function){
            stack_offset = -1;
        }
        else{
            IRManager* regManager = IRManager::getInstance();
            stack_offset = regManager->addPointerToRegisterInStack(llvm_reg);
        }    
    };
};

class FuncNode: public VarNode{
    /* This class supports variable types, such as int, bool... */
    public:
    vector<VarNode*> params;
    FuncNode( int lineno, string name, string type, vector<VarNode*> params ) : VarNode(lineno, name, type, true), params(params) {
    };
};

class ExpList: public Node{
    public:
    vector<ExpNode*> params;
    ExpList() : Node(NA){ }

    void addParam( ExpNode* param_to_add){
        params.push_back(param_to_add);
    }

    // for every param of the func check whether the param in params list is same.
    void compareParams(FuncNode* func, vector<VarNode*> func_params){
        reverse(params.begin(), params.end()); 
        
        if (params.size() != func_params.size()) {
            vector<string> types = varNodeToVectString(func_params);
            output::errorPrototypeMismatch( (*params.begin())->lineno, func->name, types );
            exit(0);
        }

        auto it_my = params.begin();
        for (auto it_func = func_params.begin(); 
            it_func != func_params.end(); ++it_func, ++it_my) {
            if ((*it_func)->type != (*it_my)->type && !((*it_func)->type == "int" && (*it_my)->type == "byte")) {
                vector<string> types = varNodeToVectString(func_params);
                output::errorPrototypeMismatch( (*it_my)->lineno, func->name, types);
                exit(0);
            }
        }
        reverse(params.begin(), params.end());
    }

    vector <string> varNodeToVectString(vector<VarNode*> func_params){
        vector<string> params;
        for( auto it_param = func_params.begin(); it_param != func_params.end(); ++it_param){
            params.push_back(toUpper((*it_param)->type));
        }
        return params;
    }
};


class BinopNode: public Node{
    /* This class supports Binop operataros , such as +, - ... */
    public:
    string binop;
    BinopNode( int lineno, string binop ) : Node(lineno){

        if (binop =="+")
            this->binop = "add";
        else  if (binop =="-")
            this->binop = "sub";
        else  if (binop =="*")
            this->binop = "mul";
        else  if (binop =="/")
            this->binop = "sdiv";
    };
};


class RelopNode: public Node{
    /* This class supports relop operataros , such as ==, <= ... */
    public:
    string relop_sign;
    RelopNode( int lineno, string relop_sign ) : Node(lineno){

        if (relop_sign == "==")
            this->relop_sign = "eq";
        else  if (relop_sign == "!=")
            this->relop_sign = "ne";
        else  if (relop_sign == "<=")
            this->relop_sign = "sle";
        else  if (relop_sign == "<")
            this->relop_sign = "slt";
        else  if (relop_sign == ">=")
            this->relop_sign = "sge";
        else  if (relop_sign == ">")
            this->relop_sign = "sgt";
    };
};

class FormalsList: public Node {
    /* this class saves the function givven variables for every function */
    public:
    vector<VarNode*> functionParams;

    FormalsList() : Node(NA){ }

    void addParam( VarNode* param_to_add){
        functionParams.push_back(param_to_add);
    }
};

class Formals: public Node {
    public:
    FormalsList* params;

    Formals(): Node(NA){ 
        params = new FormalsList();
    }
    Formals( FormalsList* params): Node(NA), params(params){ }
};

class LabelNode: public Node {
    public:
    string label;
    int location;

    LabelNode(): Node(NA){
        location = regManager->emitToBuffer("br i1 @ , label @ , label @");
        label = regManager->createLabel();
    }
}



#define YYSTYPE Node*
#endif //HW3_PARSER_HPP
