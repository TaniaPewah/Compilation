
#ifndef PARSER_ADAPTER_HPP
#define PARSER_ADAPTER_HPP

#include "parser.hpp"
#include "symbolTable.h"
#include <string>

SymbolTable symbolTable;
string current_func = "";
bool in_while = false, call_print = false;


void ruleAllowString(IdNode* id){
    if(id->name == "print") {
        call_print = true;
    }
}


ExpNode* ruleHandleString(ExpNode* string_value){
    if(!call_print){
        output::errorMismatch(string_value->lineno);
        exit(0);
    }
    return string_value;
}

void ruleContinueCheck(Node* continue_sign) {
    if(!in_while){
        output::errorUnexpectedContinue(continue_sign->lineno);
        exit(0);
    }
    delete(continue_sign);
}


void ruleBreakCheck(Node* break_sign){
    if(!in_while){
        output::errorUnexpectedBreak(break_sign->lineno);
        exit(0);
    }
    delete(break_sign);
}


void ruleReturnFromNonVoidFunc(Node* return_sign, ExpNode* return_value) {
    string func_type = symbolTable.getIdType(return_sign->lineno, current_func);
    if((func_type != return_value->type) && !(return_value->type == "byte" && func_type == "int")){
        output::errorMismatch(return_sign->lineno);
        exit(0);
    }
}


void returnFromVoidFunction(Node* return_sign){
    string func_type = symbolTable.getIdType(return_sign->lineno, current_func);
    if(func_type != "void"){
        output::errorMismatch(return_sign->lineno);
        exit(0);
    }
}

void addPrintAndPrinti(){

    vector<VarNode*> vector_for_print_func;
    vector_for_print_func.push_back(new VarNode(NA, "", "string"));
    FuncNode* print_function = new FuncNode(NA, "print", "void", vector_for_print_func);
    symbolTable.addSymbolFunc(print_function);

    vector<VarNode*> vector_for_printi_func;
    vector_for_printi_func.push_back(new VarNode(NA, "", "int"));
    FuncNode* printi_function = new FuncNode(NA, "printi", "void", vector_for_printi_func);
    symbolTable.addSymbolFunc(printi_function);

}

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

void ruleFuncDeclEndFunc(){
    current_func = "";
    symbolTable.closeScope();
}

VarNode* ruleFuncDeclStartFunc(IdNode* id_node, string type, vector<VarNode*> params) {

    string name = id_node->name;

    if( symbolTable.ifExists(name) ){
        output::errorDef(id_node->lineno, name);
        exit(0);
    }

    // TODO params show be of type vector<VarNode*> in the calling function
	FuncNode* current_node = new FuncNode(id_node->lineno, name, type, params); 
	symbolTable.addSymbolFunc( current_node );
    symbolTable.newScope();
    current_func = name;
    delete(id_node);

    for(int i=0; i< params.size(); i++) {
        symbolTable.addSymbolVar(params[i]);
    }
    return current_node;
}

VarNode* ruleVarDecl( string type_name, IdNode* id_node) {
    string name = id_node->name;

    if( symbolTable.ifExists(name) ){
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

        // TODO raise proper error
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
    if(num->value > 255) {
        output::errorByteTooLarge(num->lineno, to_string(num->value));
        exit(0);
    }
    return new ExpNode(num->lineno, "byte");
}

TypeNode* ruleCallFunc(IdNode* id_node, ExpList* params_list) {

    call_print = false;
    // search ID in symboltable, and get it's type
    string returned_type = symbolTable.getIdType(id_node->lineno, id_node->name);

    // TODO: get function arg types from symbol table
    // check whether the Exp list types are correct for this func else raise exception
    FuncNode* func = (FuncNode*)symbolTable.findSymbolInStack(id_node->name);

    cout << " ~~~~~~~~~ruleCallFunc params: " << func->params.front()->name << endl;

    if(!params_list->compareParams(func->params)){

        // TODO add error
        cout << " ~~~~~~~~~ruleCallFunc params: WRONG TYPE" << endl;
    }

    return (new TypeNode(id_node->lineno, returned_type )); 
}

TypeNode* ruleCallEmptyFunc(IdNode* id_node) {
     // search ID in symboltable, and get it's type
    string returned_type = symbolTable.getIdType(id_node->lineno, id_node->name);
    return (new TypeNode(id_node->lineno, returned_type )); 
}


FormalsList* ruleCreateFormals(int lineno){

    return new FormalsList();
}

FormalsList* ruleAddFormals( FormalsList* params_list ,VarNode* param_to_add){
    params_list->addParam(param_to_add);
    return params_list;
}

ExpList* ruleAddExp(ExpList* exp_list, ExpNode* exp){

    exp_list->addParam(exp);
    return exp_list;
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

//TODO: didn't saw it. done another one
// void addPrintandPrinti(){

//     // print
//     vector<VarNode*> params_print;
//     params_print.push_back(new VarNode(NA, "string_to_print", "string"));

//     FuncNode* print = new FuncNode(NA, "print", "void", params_print); 

//     vector<VarNode*> params_printi;
//     params_printi.push_back(new VarNode(NA, "int_to_print", "int"));

//     FuncNode* printi = new FuncNode(NA, "printi", "void", params_printi); 

// 	  symbolTable.addSymbolFunc( print );
//     symbolTable.addSymbolFunc( printi );
// }

void checkBoolExp( ExpNode* if_cond_exp ){

    if(if_cond_exp->type != "bool"){
       output::errorMismatch(if_cond_exp->lineno);
       exit(0); 
    }
}


#endif //PARSER_ADAPTER_HPP