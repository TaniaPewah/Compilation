
#ifndef PARSER_ADAPTER_HPP
#define PARSER_ADAPTER_HPP

#include "parser.hpp"
#include "symbolTable.h"
#include <string>

SymbolTable symbolTable;

VarNode* ruleFuncDecl(IdNode* id_node, string type, vector<VarNode*> params) {


    cout << "ruleFuncDecl" << endl;
    string name = id_node->name;

    if( symbolTable.ifExists(id_node->lineno, name) ){
        output::errorDef(id_node->lineno, name);
        exit(0);
    }

    // TODO params show be of type vector<VarNode*> in the calling function
	FuncNode* current_node = new FuncNode(id_node->lineno, name, type, params); 
	symbolTable.addSymbolFunc( current_node );
    delete(id_node);

     cout << "ruleFuncDecl END----------------------------------------------" << endl;
    return current_node;
}

VarNode* ruleVarDecl( string type_name, IdNode* id_node) {
    string name = id_node->name;

    if( symbolTable.ifExists(id_node->lineno, name) ){
        output::errorDef(id_node->lineno, name);
        exit(0);
    }
	VarNode* current_node = new VarNode(id_node->lineno, name, type_name); 
	symbolTable.addSymbolVar( current_node );
    
    delete(id_node);
    return current_node;
}

VarNode* ruleVarDeclAssign(IdNode* id_node, string var_type, string assign_type) {
    string name = id_node->name;
    VarNode* current_node = NULL;

    if(var_type != assign_type ){

        cout << " bad type !! in line: "<< id_node->lineno << "type is: " << var_type << " and assign type is: "<< assign_type << endl;

    } else {
	    current_node = new VarNode(id_node->lineno, name, var_type); 
	    symbolTable.addSymbolVar( current_node );
    }

    delete(id_node);
    return current_node;
}

ExpNode* ruleExpNum(NumNode* num_node){ 

    return (new ExpNode(num_node->lineno, num_node->type_name));
}


ExpNode* ruleExpBinopExp(ExpNode* exp_a,  BinopNode* binop, ExpNode* exp_b) {
    // Check exp_a & exp_b are num types. If not- raise exaption. Else, do binop and return higher num type

    if((exp_a->type != "int" && exp_a->type != "byte") || (exp_b->type != "int" && exp_b->type != "byte")){
        cout<< " ERROR in line num: "<< binop->lineno << " not numaric type using binop!"<< endl;
        exit(0);
    }
    else{
        if(exp_a->type == "int" || exp_b->type == "int"){
            return new ExpNode(binop->lineno, "int");
        }
        return new ExpNode(binop->lineno, "byte");
    }
    
}

ExpNode* ruleExpNumB(NumNode* num) {
    return new ExpNode(num->lineno, "byte");
}

TypeNode* ruleCallFunc(IdNode* id_node) {

    cout << "ruleCallFunc" << endl;

    // search ID in symboltable, and get it's type
    string returned_type = symbolTable.getIdType(id_node->lineno, id_node->name);

    // TODO: get function arg types from symbol table
    // check whether the Exp list types are correct for this func else raise exception
    
    return (new TypeNode(id_node->lineno, returned_type )); 
}

FormalsList* ruleCreateFormals(int lineno){

    return new FormalsList();
}

FormalsList* ruleAddFormals( FormalsList* params_list ,VarNode* param_to_add){
    params_list->addParam(param_to_add);
    return params_list;
}



// int stringToBinop(ExpNode* var_a, ExpNode* var_b, string op )
// {
//     int result = 0;
//     int a,b = 0;
//     if (var_a->type == "int" ){
//         a = stoi(var_a->value);
//     } else {
//         cout<< var_a->value << "  " << (var_a->value)[0] << endl;
//         a = var_a->value[0];
//         cout<<"secound variable is of type byte and value is: " << b << endl;
//     }
//      if (var_b->type == "int" ){
//         b = stoi(var_b->value);
//     } else {
//         b = var_b->value[0];
//         cout<<"secound variable is of type byte and value is: " << b << endl;
//     }

//     if(op == "+") result = a + b;
//     else if(op == "-") result = a - b;
//     else if(op == "*") result = a * b;
//     else if(op == "/") result = a / b;

//     return result;
// }


// TODO: take care of type conversions


#endif //PARSER_ADAPTER_HPP