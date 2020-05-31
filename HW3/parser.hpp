//
// Created by tt on 5/22/2020.
//

#ifndef HW3_PARSER_HPP
#define HW3_PARSER_HPP
#include <iostream>
#include <algorithm>
using namespace std;
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
};

class IdNode: public Node{
    /* This class gives basic support all ID's (variable name) */
    public:
    string name;
    IdNode( int lineno, string name ) : Node(lineno), name(name) {
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
    int value;
    NumNode( int lineno, int value, string type ) : TypeNode(lineno, type), value(value) {
    };
};

class ExpNode: public Node{
    /* This class supports the rule expressions, need further checks on CALL methods*/
    public:
    string type;
    ExpNode( int lineno, string type ) : Node(lineno), type(type) {
    };
};

class VarNode: public IdNode{
    /* This class supports variable types, such as int, bool... */
    public:
    string type;
    VarNode( int lineno, string name, string type ) : IdNode(lineno, name), type(type) {
    };
};

class FuncNode: public VarNode{
    /* This class supports variable types, such as int, bool... */
    public:
    vector<VarNode*> params;
    FuncNode( int lineno, string name, string type, vector<VarNode*> params ) : VarNode(lineno, name, type), params(params) {
    };
};

class ExpList: public Node{
    public:
    vector<ExpNode*> params;
    ExpList() : Node(NA){ }

    // TODO use it
    void addParam( ExpNode* param_to_add){
        params.push_back(param_to_add);
    }

    // for every param of the func check whether the param in params list is same.
    bool compareParams(FuncNode* func, vector<VarNode*> func_params){
        if (params.size() != func_params.size()) {
            cout << " ~~~~~~~~~ruleCallFunc params: WRONG NUMBER OF PARAMS" << params.size() << " " << func_params.size()<< endl;
            return false;
        }

        reverse(params.begin(), params.end());

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
        return true;
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
    BinopNode( int lineno, string binop ) : Node(lineno), binop(binop){
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



#define YYSTYPE Node*
#endif //HW3_PARSER_HPP
