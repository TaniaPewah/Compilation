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
    VarNode* new_var = new VarNode(type->lineno, id->name, type->type_name, false);
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
}


ExpNode* ruleHandleString(StringNode* string_node){
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
        output::errorMismatch(return_sign->lineno);
        exit(0);
    }

    regManager->returnFromNonVoidFunction(func_type, return_value);

}

void ruleReturnVoid(Node* return_sign){

    // check what is the return type of the current function
    string func_type = symbolTable.getFuncType(return_sign->lineno, current_func);
    if(func_type != "void"){
        output::errorMismatch(return_sign->lineno);
        exit(0);
    }

    regManager->emitToBuffer(" ret void ");

}

void addPrintAndPrinti(){

    vector<VarNode*> vector_for_print_func;
    vector_for_print_func.push_back(new VarNode(NA, "", "string", false));
    FuncNode* print_function = new FuncNode(NA, "print", "void", vector_for_print_func);
    symbolTable.addSymbolFunc(print_function);

    vector<VarNode*> vector_for_printi_func;
    vector_for_printi_func.push_back(new VarNode(NA, "", "int", false));
    FuncNode* printi_function = new FuncNode(NA, "printi", "void", vector_for_printi_func);
    symbolTable.addSymbolFunc(printi_function);

}

ExpNode* ruleAndExp(ExpNode* node_a, ExpNode* node_b, LabelNode* label ){
   
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
    if(node->type != "bool") {
        output::errorMismatch(node->lineno);
        exit(0);
    }

    ExpNode* new_exp_node = new ExpNode(node->lineno, "bool");
    regManager->emitToBuffer("%" + new_exp_node->llvm_reg + " = add i1 1, %" + node->llvm_reg);
    
    regManager->expPassListNotRule(node, new_exp_node);

    return new_exp_node;
}

void ruleFuncDeclEndFunc(TypeNode* type_node){
    if(type_node->type_name == "void"){
        regManager->emitToBuffer("ret void }");
    }else{
        regManager->emitToBuffer("ret i32 }");
    }
   
    current_func = "";
    symbolTable.closeScope();
}

void ruleNewFunc( IdNode* func_id, string ret_type ){
    
    string name = func_id->name;

    if( symbolTable.ifExists(name) ){
        output::errorDef(func_id->lineno, name);
        exit(0);
    }
    
	regManager->newFuncScope();
}

FuncNode* ruleFuncDecl(IdNode* id_node, string type, vector<VarNode*> params) {

    string name = id_node->name;

    reverse(params.begin(), params.end());

	FuncNode* current_node = new FuncNode(id_node->lineno, name, type, params); 
	symbolTable.addSymbolFunc( current_node );
    symbolTable.newScope();
    for( int i = 0 ; i < params.size(); i++){
        symbolTable.addSymbolVarForFunction(params[i], 0 - i - 1 );
    }
    current_func = name;

    regManager->defineNewFunction(id_node, type, params);

    delete(id_node);
    return current_node;

}

void ruleVarDecl( string type_name, IdNode* id_node) {
    string name = id_node->name;

    if( symbolTable.ifExists(name) ){
        output::errorDef(id_node->lineno, name);
        exit(0);
    }
	VarNode* current_node = new VarNode(id_node->lineno, name, type_name, true); 
	symbolTable.addSymbolVar( current_node );

    delete(id_node);

}

void ruleVarDeclAssign(IdNode* id_node, string var_type, ExpNode* exp_node) {
    string name = id_node->name;
    VarNode* current_node = NULL;

    if((var_type != exp_node->type) && !(var_type == "int" && exp_node->type == "byte")){
        output::errorMismatch(id_node->lineno);
        exit(0);

    } 

    current_node = new VarNode(id_node->lineno, name, var_type, true); 
    symbolTable.addSymbolVar( current_node );

    regManager->assignExpNodeToVar(current_node->llvm_reg, exp_node->llvm_reg, exp_node->type);

    delete(id_node);
}


void ruleInit(){
    addPrintAndPrinti();
    regManager->definePrintAndPrinti();
}


ExpNode* ruleExpBinopExp(ExpNode* exp_a,  BinopNode* binop, ExpNode* exp_b) {
    // Check exp_a & exp_b are num types. If not- raise exeption. Else, do binop and return higher num type


    if((exp_a->type != "int" && exp_a->type != "byte") || (exp_b->type != "int" && exp_b->type != "byte")){

        output::errorMismatch(exp_a->lineno);
        exit(0);
    }
    
    BackpatchInfo patching_info;
    if(binop->binop == "sdiv"){
        patching_info = regManager->handlerDivZero(exp_b->type, exp_b->llvm_reg);
    }

    if(exp_a->type == "int" || exp_b->type == "int") {  
        ExpNode* expNode = new ExpNode(binop->lineno, "int");

        if(exp_a->type !=  exp_b->type) {
            //one is int, and the other one byte
            string command;
            if(exp_a->type == "byte") {
                command = "%" + expNode->llvm_reg + " = zext i8 %" + exp_a->llvm_reg + " to i32";
            }
            else{
                command = "%" + expNode->llvm_reg + " = zext i8 %" + exp_b->llvm_reg + " to i32";
            }
            regManager->emitToBuffer(command);
        }
        string to_emit = "%" + expNode->llvm_reg + " = " + binop->binop + " i32 %" + exp_a->llvm_reg + ", %" + exp_b->llvm_reg;
        regManager->emitToBuffer(to_emit);

        if(binop->binop == "sdiv"){
            regManager->handlePatching(patching_info);
        }
        
        return expNode;
    }

    // Both are byte
    ExpNode* expNode = new ExpNode(binop->lineno, "byte");
    string to_emit = "%" + expNode->llvm_reg + " = " + binop->binop + " i8 %" + exp_a->llvm_reg + ", %" + exp_b->llvm_reg;

    regManager->emitToBuffer(to_emit);
    return expNode;
      
}


ExpNode* ruleExpNum(NumNode* num_node){

    //TODO: Add constructor, to get existing reister
    ExpNode* expNode = new ExpNode(num_node->lineno, num_node->type_name);
    string command = "%" + expNode->llvm_reg + " = add i32 0, " + to_string(num_node->value); 
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
    string command = "%" + expNode->llvm_reg + " = add i8 0, " + to_string(num->value); 

    regManager->emitToBuffer(command);
    return (expNode);
}


ExpNode* ruleBool(ExpNode* bool_node, string bool_sign){
    
    regManager->emitToBuffer("%" + bool_node->llvm_reg + " = add i1 0, " + bool_sign);

    regManager->createFalseListAndTrueList(bool_node, bool_sign);
    
    return (bool_node);
}


ExpNode* ruleCallFunc(IdNode* id_node, ExpList* params_list) {

    call_print = false;
    // search ID in symboltable, and get it's type
    string returned_type = symbolTable.getFuncType(id_node->lineno, id_node->name);

    // get function arg types from symbol table
    FuncNode* func = (FuncNode*)(symbolTable.findSymbolInStack(id_node->name)->node);

    // check whether the Exp list types are correct for this func else raise exception
    params_list->compareParams(func, func->params);

    ExpNode* returned_value = new ExpNode(id_node->lineno, returned_type);

    regManager->handleCallFunction(func, params_list, returned_value);

    return returned_value; 
}

ExpNode* ruleCallEmptyFunc(IdNode* id_node) {
     // search ID in symboltable, and get it's type
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

    if( id_type != exp->type ){
        
        output::errorMismatch(id_node->lineno);
        exit(0);
    }

    regManager->assignExpNodeToVar(variable->llvm_reg, exp->llvm_reg, exp->type);
    return;
}

ExpNode* ruleRelop(ExpNode* exp1, RelopNode* compare_sign, ExpNode* exp2){

    if((exp1->type != "int" && exp1->type != "byte") || (exp2->type != "int" && exp2->type != "byte")){

        output::errorMismatch(exp1->lineno);
        exit(0);
    }

    ExpNode* compare = new ExpNode(exp1->lineno, "bool");

    regManager->expRelopExpCreateBr(compare, exp1, exp2, compare_sign);
    
    return compare;
}


void ruleIfNoElse( ExpNode* if_cond_exp , LabelNode* marker, BrNode* br_node ){
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

    ExpNode* exp_node = new ExpNode(line, type);
    regManager->loadID(type, exp_node->llvm_reg, var->llvm_reg);
    
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

    exitWhile();
    regManager->patchWhileNoElse(go_to_before_exp, before_exp_marker,  exp_node, after_exp_marker, go_to_check_exp );
}

void ruleWhileElse ( BrNode* go_to_before_exp, LabelNode* before_exp_marker,
                     ExpNode* exp_node,  LabelNode* after_exp_marker, BrNode* go_to_check_exp,
                    LabelNode* else_marker, BrNode* end_else){

    
    regManager->patchWhileElse(go_to_before_exp, before_exp_marker,  exp_node, after_exp_marker, go_to_check_exp,
                                 else_marker, end_else);                                   
}


void ruleCallStatment(ExpNode* function_returned_value){
    if(function_returned_value->type == "bool"){
        regManager->callToFunctionBackPatch(function_returned_value);
    }
} 

#endif //PARSER_ADAPTER_HPP