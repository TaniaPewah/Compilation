#ifndef PARSER_ADAPTER_HPP
#define PARSER_ADAPTER_HPP
#include "parser.hpp"
#include "symbolTable.hpp"
#include <string>

SymbolTable symbolTable;
string current_func = "";
bool call_print = false;
IRManager* regManager = IRManager::getInstance();

VarNode* ruleFormalDecl(TypeNode* type, IdNode* id){
    VarNode* new_var = new VarNode(type->lineno, id->name, type->type_name);
    new_var->llvm_reg = regManager->getFreshVarReg();
    delete(type);
    delete(id);
    return new_var;
}

void ruleAllowString(IdNode* id){
    if(id->name == "print") {
        call_print = true;
    }
}

void enterWhile(){
    symbolTable.newScope();

    regManager->enterLoop();
    
}

void exitWhile(){
    regManager->loop_counter--;
    symbolTable.closeScope();
}


ExpNode* ruleHandleString(StringNode* string_node){
    // cout<<"called function not print"<<endl;
    if(!call_print){
        output::errorMismatch(string_node->lineno);
        exit(0);
    }

    string command = string_node->llvm_reg + " = constant [" + string_node->size + " x i8] c\""+ string_node->value + "\\00\"";
    regManager->emitGlobalToBuffer(command);
    return (ExpNode*)string_node;
}

void ruleContinueCheck(Node* continue_sign) {
    if(regManager->loop_counter <= 0){
        output::errorUnexpectedContinue(continue_sign->lineno);
        exit(0);
    }

    regManager->handleContinue();
    delete(continue_sign);
}


void ruleBreakCheck(Node* break_sign){
    if(regManager->loop_counter <= 0){
        output::errorUnexpectedBreak(break_sign->lineno);
        exit(0);
    }

    regManager->handleBreak();

    delete(break_sign);
}


void ruleReturnNonVoid(Node* return_sign, ExpNode* return_value) {

    string func_type = symbolTable.getFuncType(return_sign->lineno, current_func);
    if((func_type != return_value->type) && !(return_value->type == "byte" && func_type == "int")){
        // cout<<"function not int byte or bool"<<endl;
        output::errorMismatch(return_sign->lineno);
        exit(0);
    }

    regManager->returnFromNonVoidFunction(func_type, return_value);

}

void ruleReturnVoid(Node* return_sign){

    // check what is the return type of the current function
    string func_type = symbolTable.getFuncType(return_sign->lineno, current_func);
    // cout<<"funvtion not void"<<endl;
    if(func_type != "void"){
        output::errorMismatch(return_sign->lineno);
        exit(0);
    }

    regManager->emitToBuffer(" ret void ");

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

ExpNode* ruleAndExp(ExpNode* node_a, ExpNode* node_b, LabelNode* label ){
   
   // cout<<"And with no bool" <<endl;
    if(node_a->type != "bool" || node_b->type != "bool") {
        output::errorMismatch(node_a->lineno);
        exit(0);
    }

    ExpNode* new_exp_node = new ExpNode(node_a->lineno, "bool");

    // get short circuit label label_if_true_continue
    // TODO why need regsize and substring of nodea_llvmreg -1??

    regManager->andPatching(node_a, node_b, label, new_exp_node);
    
    delete(node_a);
    delete(node_b);
    return new_exp_node;
}

ExpNode* ruleOrExp(ExpNode* node_a, ExpNode* node_b,  LabelNode* label){
    // cout<<"OR with no bool"<<endl;
    if(node_a->type != "bool" || node_b->type != "bool") {
        output::errorMismatch(node_a->lineno);
        exit(0);
    }

    ExpNode* new_exp_node = new ExpNode(node_a->lineno, "bool");

    regManager->orPatching(node_a, node_b, label, new_exp_node);
    delete(node_a);
    delete(node_b);
    
    return new_exp_node;
}

ExpNode* ruleNotExp(ExpNode* node) {
    // cout<<"not with no bool"<<endl;
    if(node->type != "bool") {
        output::errorMismatch(node->lineno);
        exit(0);
    }

    ExpNode* new_exp_node = new ExpNode(node->lineno, "bool");
    
    regManager->expPassListNotRule(node, new_exp_node);

    return new_exp_node;
}

void ruleFuncDeclEndFunc(TypeNode* type_node){
    if(type_node->type_name == "void"){
        regManager->emitToBuffer("ret void }");
    }else{
        regManager->emitToBuffer("ret i32 0 }");
    }
   
    current_func = "";
    symbolTable.closeScope();
}

void ruleNewFunc( IdNode* func_id, string ret_type ){

    vector<VarNode*> temp_params;
    FuncNode* current_node = new FuncNode(func_id->lineno, func_id->name, ret_type, temp_params); 
	symbolTable.addSymbolFunc( current_node );
    symbolTable.newScope();
	regManager->newFuncScope();
}

FuncNode* ruleFuncDecl(IdNode* id_node, string type, vector<VarNode*> params) {

    reverse(params.begin(), params.end());

    // if (id_node->name == "main"){
    //     if (params.size() != 0){
    //         output::errorMainMissing();
    //     }
    // }

    FuncNode* current_node = symbolTable.getFuncNode(id_node->lineno, id_node->name);
    current_node->setParams(params);

    for( int i = 0 ; i < params.size(); i++){
        symbolTable.addSymbolVarForFunction(params[i], 0 - i - 1 );
    }
    current_func = id_node->name;
    
    // ruleNewFunc(id_node, type);
    regManager->defineNewFunction(id_node, type, params);

    delete(id_node);
    return current_node;

}

VarNode* createVarNode(IdNode* id_node, string var_type){
    VarNode* current_node =  regManager->createNewVar(id_node, var_type);
   
    symbolTable.addSymbolVar( current_node );

    delete(id_node);
    return current_node;
}

void ruleVarDeclAssign(IdNode* id_node, VarNode* var_node, ExpNode* exp_node,  string var_type) {
   
   // cout<<"Type ID = exp, but id type != exp type" <<endl;
    if((var_type != exp_node->type) && !(var_type == "int" && exp_node->type == "byte")){
        output::errorMismatch(id_node->lineno);
        exit(0);

    } 
    regManager->assignExpNodeToVar(var_node, exp_node);
}


void ruleInit(){
    addPrintAndPrinti();
    regManager->defineDivZeroError();
    regManager->definePrintAndPrinti();
    
}


ExpNode* ruleExpBinopExp(ExpNode* exp_a,  BinopNode* binop, ExpNode* exp_b) {
    // Check exp_a & exp_b are num types. If not- raise exeption. Else, do binop and return higher num type

    // VarNode* var_a = getVarNode(exp_a-)

    // cout<<"binop between two vars, with diffrent types"<<endl;
    if((exp_a->type != "int" && exp_a->type != "byte") || (exp_b->type != "int" && exp_b->type != "byte")){
        output::errorMismatch(exp_a->lineno);
        exit(0);
    }
    
    BackpatchInfo patching_info;
    if(binop->binop == "sdiv"){
        patching_info = regManager->handlerDivZero(exp_b->type, exp_b->llvm_reg);
    }

    string type = (exp_a->type == "int" || exp_b->type == "int") ? "int" : "byte";
    
    ExpNode* expNode = new ExpNode(binop->lineno, type);
    expNode->llvm_reg =  regManager->getFreshReg()->getName();

    regManager->emitToBuffer(";if multipale define-> check binop");
    string to_emit = "%p" + expNode->llvm_reg + " = " + binop->binop + " i32 %p" + exp_a->llvm_reg + ", %p" + exp_b->llvm_reg;
    regManager->emitToBuffer(to_emit );

    if(binop->binop == "sdiv"){
        regManager->handlePatching(patching_info);
    }

    if(type == "byte"){
        string new_reg_name = regManager->getFreshReg()->getName();
        regManager->emitToBuffer( "%p" + new_reg_name + " = and i32 %p" + expNode->llvm_reg + ", 255");
        expNode->llvm_reg = new_reg_name;
    }
    
    return expNode;
    
}


ExpNode* ruleExpNum(NumNode* num_node){

    //TODO: Add constructor, to get existing reister
    ExpNode* expNode = new ExpNode(num_node->lineno, num_node->type_name);
    expNode->llvm_reg = regManager->getFreshReg()->getName();

    string command = "%p" + expNode->llvm_reg + " = add i32 0, " + to_string(num_node->value); 
    regManager->emitToBuffer(command);
    return (expNode);
}

ExpNode* ruleExpNumB(NumNode* num) {
    if(num->value > 255) {
        output::errorByteTooLarge(num->lineno, to_string(num->value));
        exit(0);
    }
    //TODO: Add constructor, to get existing reister
    ExpNode* expNode = new ExpNode(num->lineno, "byte");
    expNode->llvm_reg = regManager->getFreshReg()->getName();

    string command = "%p" + expNode->llvm_reg + " = add i32 0, " + to_string(num->value); 

    regManager->emitToBuffer(command);
    return (expNode);
}


ExpNode* ruleBool(ExpNode* bool_node, string bool_sign){

    regManager->createFalseListAndTrueList(bool_node, bool_sign);
    
    return (bool_node);
}


ExpNode* ruleCallFunc(IdNode* id_node, ExpList* params_list) {
    

    // search ID in symboltable, and get it's type
    string returned_type = symbolTable.getFuncType(id_node->lineno, id_node->name);

    // get function arg types from symbol table
    FuncNode* func = (FuncNode*)(symbolTable.findSymbolInStack(id_node->name)->node);

    // check whether the Exp list types are correct for this func else raise exception
    params_list->compareParams(func, func->params);

    reverse(params_list->params.begin(), params_list->params.end());
    ExpNode* returned_value = new ExpNode(id_node->lineno, returned_type);

    regManager->handleCallFunction(func, params_list, returned_value);

    return returned_value; 
}

ExpNode* ruleCallEmptyFunc(IdNode* id_node) {
     // search ID in symboltable, and get it's type
     //TODO: Add check for empty called function!!
    string returned_type = symbolTable.getFuncType(id_node->lineno, id_node->name);

    FuncNode* func = (FuncNode*)(symbolTable.findSymbolInStack(id_node->name)->node);

    ExpNode* returned_value = new ExpNode(id_node->lineno, returned_type);

    regManager->handleCallFunction(func, NULL, returned_value);

    return returned_value; 
}


FormalsList* ruleCreateFormals(int lineno){

    return new FormalsList();
}

FormalsList* ruleAddFormals( FormalsList* params_list ,VarNode* param_to_add){
    params_list->addParam(param_to_add);
    return params_list;
}

ExpList* ruleAddExp(ExpList* exp_list, ExpNode* exp_node_to_add){

    if(exp_node_to_add->type == "bool") {
        regManager->startBoolJump(exp_node_to_add);
    }

    exp_list->addParam(exp_node_to_add);
    return exp_list;
}

void ruleIdAssign( IdNode* id_node, ExpNode* exp){

    VarNode* variable = symbolTable.getVarNode(id_node->lineno, id_node->name);

    string id_type = variable->type;
    // cout<<"ID = exp, but id type != exp type"<<endl;
    if( id_type != exp->type && !(id_type == "int" && exp->type == "byte")){
        
        output::errorMismatch(id_node->lineno);
        exit(0);
    }

    regManager->assignExpNodeToVar(variable, exp);
    return;
}

ExpNode* ruleRelop(ExpNode* exp1, RelopNode* compare_sign, ExpNode* exp2){

    // cout<<"var 1 relop var 2, but tow diffrent types"<<endl;
    if((exp1->type != "int" && exp1->type != "byte") || (exp2->type != "int" && exp2->type != "byte")){

        output::errorMismatch(exp1->lineno);
        exit(0);
    }

    ExpNode* compare = new ExpNode(exp1->lineno, "bool");

    regManager->expRelopExpCreateBr(compare, exp1, exp2, compare_sign);
    
    return compare;
}


void ruleIfNoElse( ExpNode* if_cond_exp , LabelNode* marker, BrNode* br_node ){
    // cout<<"if no else condition type not bool"<<endl;
    if(if_cond_exp->type != "bool"){
       output::errorMismatch(if_cond_exp->lineno);
       exit(0); 
    }

    regManager->patchIf(if_cond_exp, marker, br_node);
}

void ruleIfElse( ExpNode* if_cond_exp, 
                           LabelNode* marker_if_true, 
                           BrNode* go_to_end_from_if, 
                           LabelNode* marker_else, 
                           BrNode* go_to_end_from_else ){
    // cout<<"if condition with else not bool"<<endl;
    if(if_cond_exp->type != "bool"){
       output::errorMismatch(if_cond_exp->lineno);
       exit(0); 
    }

    regManager->patchIfElse(if_cond_exp, marker_if_true, go_to_end_from_if, 
                            marker_else, go_to_end_from_else );

}


void checkMain(){
    if (!symbolTable.hasMain()) {
		output::errorMainMissing();
		exit(0);
	}
}

ExpNode* ruleIDToExp (IdNode* id_node){
    VarNode* var = symbolTable.getVarNode(id_node->lineno, id_node->name);
    string type = var->type;
    int line = id_node->lineno;

    ExpNode* exp_node = new ExpNode(line, type, regManager->getFreshReg()->getName());

    regManager->getExpNodeValueAndBranch(var, exp_node);

    return exp_node;
}

void endProgram(){

    regManager->endProgram();
}

LabelNode* ruleBranchLabel(){
    return new LabelNode();
}

BrNode* ruleNextJump(){
    BrNode* returned = new BrNode(regManager->goToNext());
    return returned;
}

void ruleWhileNoElse( BrNode* go_to_before_exp, LabelNode* before_exp_marker,
                     ExpNode* exp_node,  LabelNode* after_exp_marker, BrNode* go_to_check_exp ){

    // cout<<"While condition no else not bool"<<endl;
    if(exp_node->type != "bool") {
        output::errorMismatch(exp_node->lineno);
        exit(0);
    }
    regManager->patchWhileNoElse(go_to_before_exp, before_exp_marker,  exp_node, after_exp_marker, go_to_check_exp );
}

void ruleWhileElse ( BrNode* go_to_before_exp, LabelNode* before_exp_marker,
                     ExpNode* exp_node,  LabelNode* after_exp_marker, BrNode* go_to_check_exp,
                    LabelNode* else_marker, BrNode* end_else){
    
    // cout<<"While condition with else not bool"<<endl;
    if(exp_node->type != "bool") {
        output::errorMismatch(exp_node->lineno);
        exit(0);
    }
     
    regManager->patchWhileElse(go_to_before_exp, before_exp_marker,  exp_node, after_exp_marker, go_to_check_exp,
                                 else_marker, end_else);                                   
}


void ruleCallStatment(ExpNode* function_returned_value){
    if(function_returned_value->type == "bool"){
        regManager->callToFunctionBackPatch(function_returned_value);
    }
    call_print = false;
} 

void debugWindow(string s){
    regManager->emitToBuffer(";" + s);
}

#endif //PARSER_ADAPTER_HPP