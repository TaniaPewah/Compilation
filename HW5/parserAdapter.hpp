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
    regManager->loop_counter++;
    symbolTable.newScope();
    // add break & countinue;
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
    delete(continue_sign);
}


void ruleBreakCheck(Node* break_sign){
    if(regManager->loop_counter <= 0){
        output::errorUnexpectedBreak(break_sign->lineno);
        exit(0);
    }

    regManager->handleBreake();

    delete(break_sign);
}


void ruleReturnNonVoid(Node* return_sign, ExpNode* return_value) {

    string func_type = symbolTable.getFuncType(return_sign->lineno, current_func);
    if((func_type != return_value->type) && !(return_value->type == "byte" && func_type == "int")){
        output::errorMismatch(return_sign->lineno);
        exit(0);
    }
}

void ruleReturnVoid(Node* return_sign){

    // check what is the return type of the current function
    string func_type = symbolTable.getFuncType(return_sign->lineno, current_func);
    if(func_type != "void"){
        output::errorMismatch(return_sign->lineno);
        exit(0);
    }
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
    cout << "handling ruleOrExp function" << endl;
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
    regManager->emitToBuffer(new_exp_node->llvm_reg + " = add i1 1, " + node->llvm_reg);
    
    regManager->expPassListNotRule(node, new_exp_node);

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

    reverse(params.begin(), params.end());

	FuncNode* current_node = new FuncNode(id_node->lineno, name, type, params); 
	symbolTable.addSymbolFunc( current_node );
    symbolTable.newScope();
    for( int i = 0 ; i < params.size(); i++){
        symbolTable.addSymbolVarForFunction(params[i], 0 - i - 1 );
    }
    current_func = name;
    delete(id_node);
    return current_node;
}

StatementNode* ruleVarDecl( string type_name, IdNode* id_node) {
    string name = id_node->name;

    if( symbolTable.ifExists(name) ){
        output::errorDef(id_node->lineno, name);
        exit(0);
    }
	VarNode* current_node = new VarNode(id_node->lineno, name, type_name, false); 
	symbolTable.addSymbolVar( current_node );

    delete(id_node);
    StatementNode* returned = new StatementNode();
    return returned;
}

StatementNode* ruleVarDeclAssign(IdNode* id_node, string var_type, ExpNode* exp_node) {
    string name = id_node->name;
    VarNode* current_node = NULL;

    if((var_type != exp_node->type) && !(var_type == "int" && exp_node->type == "byte")){
        output::errorMismatch(id_node->lineno);
        exit(0);

    } 

    current_node = new VarNode(id_node->lineno, name, var_type, false); 
    symbolTable.addSymbolVar( current_node );

    regManager->assignExpNodeToVar(current_node->llvm_reg, exp_node->llvm_reg, exp_node->type);

    delete(id_node);
    StatementNode* returned = new StatementNode();
    return returned;
}


void ruleInit(){
    addPrintAndPrinti();
    regManager->emitGlobalToBuffer("@.forPrintString = internal constant [4 x i8] c\"%s\\0A\\00\"");
    regManager->emitGlobalToBuffer("@.forPrinti = internal constant [4 x i8] c\"%d\\0A\\00\"");
    
    regManager->emitGlobalToBuffer("declare void @exit(i32) ");
    regManager->emitGlobalToBuffer("declare i32 @printf(i8*, ...)");
    
    regManager->exitFunction();
    regManager->zeroExit();
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
                command = expNode->llvm_reg + " = zext i8 " + exp_a->llvm_reg + " to i32";
            }
            else{
                command = expNode->llvm_reg + " = zext i8 " + exp_b->llvm_reg + " to i32";
            }
            cout << "command binup is: " << command << endl;
            regManager->emitToBuffer(command);
        }
        string to_emit = expNode->llvm_reg + " = " + binop->binop + " i32 " + exp_a->llvm_reg + ", " + exp_b->llvm_reg;
        regManager->emitToBuffer(to_emit);

        if(binop->binop == "sdiv"){
            regManager->handlePatching(patching_info);
        }
        
        return expNode;
    }

    // Both are byte
    ExpNode* expNode = new ExpNode(binop->lineno, "byte");
    string to_emit = expNode->llvm_reg + " = " + binop->binop + " i8 " + exp_a->llvm_reg + ", " + exp_b->llvm_reg;

    regManager->emitToBuffer(to_emit);
    return expNode;
      
}


ExpNode* ruleExpNum(NumNode* num_node){

    //TODO: Add constructor, to get existing reister
    ExpNode* expNode = new ExpNode(num_node->lineno, num_node->type_name);
    string command = expNode->llvm_reg + " = add i32 0, " + to_string(num_node->value); 
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
    string command = expNode->llvm_reg + " = add i8 0, " + to_string(num->value); 

    regManager->emitToBuffer(command);
    return (expNode);
}


ExpNode* ruleBool(ExpNode* bool_node, string bool_sign){
    if(bool_sign == "1"){
        cout << "now handling true with register " << bool_node->llvm_reg << endl;
    }
    else{
        cout << "now handling false with register " << bool_node->llvm_reg << endl;
    }
    regManager->emitToBuffer(bool_node->llvm_reg + " = add i1 0, " + bool_sign);

    regManager->createFalseListAndTrueList(bool_node, bool_sign);
    
    return (bool_node);
}


TypeNode* ruleCallFunc(IdNode* id_node, ExpList* params_list) {

    call_print = false;
    // search ID in symboltable, and get it's type
    string returned_type = symbolTable.getFuncType(id_node->lineno, id_node->name);

    // get function arg types from symbol table
    FuncNode* func = (FuncNode*)(symbolTable.findSymbolInStack(id_node->name)->node);

    // check whether the Exp list types are correct for this func else raise exception
    params_list->compareParams(func, func->params);

    return (new TypeNode(id_node->lineno, returned_type )); 
}

TypeNode* ruleCallEmptyFunc(IdNode* id_node) {
     // search ID in symboltable, and get it's type
    string returned_type = symbolTable.getFuncType(id_node->lineno, id_node->name);
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


StatementNode* ruleIfNoElse( ExpNode* if_cond_exp , LabelNode* marker, StatementNode* statement ){
    if(if_cond_exp->type != "bool"){
       output::errorMismatch(if_cond_exp->lineno);
       exit(0); 
    }

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ruleIfNoElse"<< endl;

    StatementNode* result =  new StatementNode();
    regManager->patchIf(if_cond_exp, marker, statement, result);

    return result;
}

StatementNode* ruleIfElse( ExpNode* if_cond_exp , LabelNode* marker_if_st, 
                           StatementNode* if_statement, StatementNode* go_to_else, 
                           LabelNode* marker_else_st, StatementNode* else_statement ){
    if(if_cond_exp->type != "bool"){
       output::errorMismatch(if_cond_exp->lineno);
       exit(0); 
    }

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ruleIfNoElse"<< endl;

    StatementNode* result =  new StatementNode();
    regManager->patchIfElse(if_cond_exp, marker_if_st, if_statement, go_to_else, 
                            marker_else_st, else_statement,result);

    return result;
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

ExpNode* ruleCallToExp ( TypeNode* callNode ){
    ExpNode* expNode = new ExpNode(callNode->lineno, callNode->type_name);
    delete(callNode);
    return expNode;
}

void endProgram(){

    regManager->endProgram();
}

LabelNode* ruleBranchLabel(){
    cout << "in OR marker" << endl;
    return new LabelNode();
}

StatementNode* ruleNextJump(){
    StatementNode* returned = new StatementNode();
    regManager->goToNext(returned);
    return returned;
}

StatementNode* ruleWhileNoElse( StatementNode* statment_node, LabelNode* before_exp_marker,
                     LabelNode* after_exp_marker, ExpNode* exp_node, LabelNode* end_lable ){

    StatementNode* returned = new StatementNode();

    exitWhile();
    regManager->patchWhileNoElse(statment_node, before_exp_marker, after_exp_marker, exp_node, returned, end_lable);

    return returned;
}

#endif //PARSER_ADAPTER_HPP