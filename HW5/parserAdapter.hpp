
#ifndef PARSER_ADAPTER_HPP
#define PARSER_ADAPTER_HPP
#include "parser.hpp"
#include "symbolTable.hpp"
#include <string>

SymbolTable symbolTable;
string current_func = "";
bool call_print = false;
int in_while = 0;
IRManager* regManager = IRManager::getInstance();

VarNode* ruleFormalDecl(TypeNode* type, IdNode* id){
    VarNode* new_var = new VarNode(type->lineno, id->name, type->type_name);
    delete(type);
    delete(id);
    return new_var;
}

void ruleAllowString(IdNode* id){
    if(id->name == "print") {
        call_print = true;
    }
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
    if(in_while <= 0){
        output::errorUnexpectedContinue(continue_sign->lineno);
        exit(0);
    }
    delete(continue_sign);
}


void ruleBreakCheck(Node* break_sign){
    if(in_while <= 0){
        output::errorUnexpectedBreak(break_sign->lineno);
        exit(0);
    }
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
    vector_for_print_func.push_back(new VarNode(NA, "", "string"));
    FuncNode* print_function = new FuncNode(NA, "print", "void", vector_for_print_func);
    symbolTable.addSymbolFunc(print_function);

    vector<VarNode*> vector_for_printi_func;
    vector_for_printi_func.push_back(new VarNode(NA, "", "int"));
    FuncNode* printi_function = new FuncNode(NA, "printi", "void", vector_for_printi_func);
    symbolTable.addSymbolFunc(printi_function);

}

ExpNode* ruleAndExp(ExpNode* node_a, ExpNode* node_b){
   
    if(node_a->type != "bool" || node_b->type != "bool") {
        output::errorMismatch(node_a->lineno);
        exit(0);
    }

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

    ExpNode* new_exp_node = new ExpNode(node->lineno, "bool");
    regManager->emitToBuffer(new_exp_node->llvm_reg + " = add i1 1, " + node->llvm_reg);
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
        output::errorMismatch(id_node->lineno);
        exit(0);

    } else {
	    current_node = new VarNode(id_node->lineno, name, var_type); 
	    symbolTable.addSymbolVar( current_node );
    }

    delete(id_node);
    return current_node;
}


ExpNode* handlerDivZero(ExpNode* exp_a,  BinopNode* binop, ExpNode* exp_b) {
    Register* zero_devision = regManager->getFreshReg();

    if(exp_b->type == "byte"){
        regManager->emitToBuffer(zero_devision->getName() + " = icmp eq i8 " + exp_b->llvm_reg + ", 0");
    }
    else{
        regManager->emitToBuffer(zero_devision->getName() + " = icmp eq i32 " + exp_b->llvm_reg + ", 0");
    }
    
    
    

    

}


ExpNode* ruleExpBinopExp(ExpNode* exp_a,  BinopNode* binop, ExpNode* exp_b) {
    // Check exp_a & exp_b are num types. If not- raise exeption. Else, do binop and return higher num type


    if((exp_a->type != "int" && exp_a->type != "byte") || (exp_b->type != "int" && exp_b->type != "byte")){

        output::errorMismatch(exp_a->lineno);
        exit(0);
    }
    

    // TODO 1. division by zero
    // 2. result of MUL can be up to 64 bits

    // zero_division_register = fresh_register;

    // codeBuffer.emit(zero_division_register + " = icmp eq i8 " + $3->llvm_register + ", 0");
    // int location = codeBuffer.emit("br i1 " + zero_division_register + ", label @, label @");

    if(binop->binop == "div"){
        handlerDivZero(exp_a, binop, exp_b);
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
        cout << "ruleExpBinopExp command : " << to_emit << endl;
        regManager->emitToBuffer(to_emit);
        
        return expNode;
    }

    // Both are byte
    ExpNode* expNode = new ExpNode(binop->lineno, "byte");
    string to_emit = expNode->llvm_reg + " = " + binop->binop + " i8 " + exp_a->llvm_reg + ", " + exp_b->llvm_reg;
    cout << "ruleExpBinopExp command : " << to_emit << endl;
    regManager->emitToBuffer(to_emit);
    return expNode;
      
}


ExpNode* ruleExpNum(NumNode* num_node){

    //TODO: Add constructor, to get existing reister
    ExpNode* expNode = new ExpNode(num_node->lineno, num_node->type_name);
    string command = expNode->llvm_reg + " = add i32 0, " + to_string(num_node->value); 
    cout << "ruleExpNum command: " << command << endl;
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
    cout << "ruleExpNumB command: " << command << endl;
    regManager->emitToBuffer(command);
    return (expNode);
}


ExpNode* ruleBool(ExpNode* bool_node, string bool_sign){
    string command = bool_node->llvm_reg + " = add i1 0, " + bool_sign;
    cout << "ruleBool command: " << command << endl;
    
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

    string id_type = symbolTable.getIdType(id_node->lineno, id_node->name);

    if( id_type != exp->type ){
        
        output::errorMismatch(id_node->lineno);
        exit(0);
    }

    //codeBuffer.emit("load i32, i32* " + exp->llvm_reg);
    // %x = alloca i32 // I now have an x
    // store i32 3, i32* %x // write to x
    // %temp = load i32, i32* %x  // temp = x = 3
}

ExpNode* ruleRelop(ExpNode* exp1, RelopNode* compare_sign, ExpNode* exp2){

    if((exp1->type != "int" && exp1->type != "byte") || (exp2->type != "int" && exp2->type != "byte")){

        output::errorMismatch(exp1->lineno);
        exit(0);
    }

    ExpNode* compare = new ExpNode(exp1->lineno, "bool");

    string exp1_i32_register = regManager->fromI8RegisterToI32Register(exp1->type, exp1->llvm_reg);
    string exp2_i32_register = regManager->fromI8RegisterToI32Register(exp2->type, exp2->llvm_reg);
    
    regManager->emitToBuffer(compare->llvm_reg + " = icmp " + compare_sign->relop_sign + " i32 " + 
    exp1_i32_register + ", " + exp2_i32_register);
    
    return compare;
}


void checkBoolExp( ExpNode* if_cond_exp ){
    if(if_cond_exp->type != "bool"){
       output::errorMismatch(if_cond_exp->lineno);
       exit(0); 
    }
}

void checkMain(){
    if (!symbolTable.hasMain()) {
		output::errorMainMissing();
		exit(0);
	}
}

ExpNode* ruleIDToExp (IdNode* id_node){
    string type = symbolTable.getIdType(id_node->lineno, id_node->name);
    int line = id_node->lineno;
    delete id_node;
    return new ExpNode(line, type);
}

ExpNode* ruleCallToExp ( TypeNode* callNode ){
    ExpNode* expNode = new ExpNode(callNode->lineno, callNode->type_name);
    delete(callNode);
    return expNode;
}

void endProgram(){

    regManager->endProgram();
}


#endif //PARSER_ADAPTER_HPP
