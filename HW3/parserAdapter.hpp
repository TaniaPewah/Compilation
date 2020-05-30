
#ifndef PARSER_ADAPTER_HPP
#define PARSER_ADAPTER_HPP

#include "parser.hpp"
#include "symbolTable.h"
#include <string>

SymbolTable symbolTable;

ExpNode* ruleAndExp(ExpNode* node_a, ExpNode* node_b){
    cout<< "node a atype is: "<< node_a->type <<endl;
    cout<< "node b atype is: "<< node_a->type <<endl;

    if(node_a->type != "bool" || node_b->type != "bool") {
        output::errorMismatch(node_a->lineno);
        exit(0);
    }
    cout<< "is boll!!!"<<endl;

    ExpNode* new_exp_node = new ExpNode(node_a->lineno, "bool");
    delete(node_a);
    delete(node_b);
    return new_exp_node;
}

ExpNode* ruleOrExp(ExpNode* node_a, ExpNode* node_b){
    return ruleAndExp(node_a, node_b);
}

ExpNode* ruleNotExp(ExpNode* node) {
    if(node->type != "bool") {
        output::errorMismatch(node->lineno);
        exit(0);
    }
    cout<< "is boll!!!"<<endl;

    ExpNode* new_exp_node = new ExpNode(node->lineno, "bool");
    delete(node);
    return new_exp_node;
}

VarNode* ruleFuncDecl(IdNode* id_node, string type, vector<VarNode*> params) {

    string name = id_node->name;

    if( symbolTable.ifExists(id_node->lineno, name) ){
        output::errorDef(id_node->lineno, name);
        exit(0);
    }

    // TODO params show be of type vector<VarNode*> in the calling function
	FuncNode* current_node = new FuncNode(id_node->lineno, name, type, params); 
	symbolTable.addSymbolFunc( current_node );
    delete(id_node);
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

    if((var_type != assign_type) && !(var_type == "int" && assign_type == "byte")){

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
    // Check exp_a & exp_b are num types. If not- raise exeption. Else, do binop and return higher num type

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

void ruleIdAssign( IdNode* id_node, ExpNode* exp){

    string id_type = symbolTable.getIdType(id_node->lineno, id_node->name);

    if( id_type != exp->type ){
        output::errorMismatch(id_node->lineno);
        exit(0);
    }
    // TODO update value in symbol table
}

ExpNode* ruleRelop(ExpNode* exp1, ExpNode* exp2){

    if((exp1->type != "int" && exp1->type != "byte") || (exp2->type != "int" && exp2->type != "byte")){

        output::errorMismatch(exp1->lineno);
        exit(0);
    }
      
    return new ExpNode(exp1->lineno, "bool");
}

void addPrintandPrinti(){

    // print
    vector<VarNode*> params_print;
    params_print.push_back(new VarNode(NA, "string_to_print", "string"));

    FuncNode* print = new FuncNode(NA, "print", "void", params_print); 

    vector<VarNode*> params_printi;
    params_printi.push_back(new VarNode(NA, "int_to_print", "int"));

    FuncNode* printi = new FuncNode(NA, "printi", "void", params_printi); 

	symbolTable.addSymbolFunc( print );
    symbolTable.addSymbolFunc( printi );
}


#endif //PARSER_ADAPTER_HPP