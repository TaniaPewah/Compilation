#include "IRManager.hpp"
#include "parser.hpp"

IRManager* IRManager::instance = 0;

int IRManager::addPointerToRegisterInStack(string llvm_reg){
        // adds a variable to the stack, and creates a register that is a pointer to that location.
        // this register will always be a pointer to the stack

        emitToBuffer("%p" + llvm_reg + " = getelementptr inbounds [50 x i32], [50 x i32]* %stack, i32 0, i32 " + to_string(stack_offset_pointer));
        stack_offset_pointer++;
        emitToBuffer("store i32 0, i32* %p" + llvm_reg);
        return stack_offset_pointer - 1;
    }

void IRManager::assignExpNodeToVar(string variable_reg, string exp_node_reg, string exp_node_type){
    // the variable is a register which points to a location in the string. we just neet to update the value in this location.
    if(exp_node_type == "int"){
        emitToBuffer("store i32 %" + exp_node_reg + ", i32* %p" + variable_reg);
    }else if(exp_node_type == "byte"){
        Register* fresh_register = getFreshReg();
        emitToBuffer("%" + fresh_register->getName() + " = zext i8 %" + exp_node_reg + " to i32");
        emitToBuffer("store i32 %" + fresh_register->getName() + ", i32* %p" + variable_reg);
    }else{
        Register* fresh_register = getFreshReg();
        emitToBuffer("%" + fresh_register->getName() + " = zext i1 %" + exp_node_reg + " to i32");
        emitToBuffer("store i32 %" + fresh_register->getName() + ", i32* %p" + variable_reg);
    }
    return;
}

Register* IRManager::getFreshReg(){
    // TODO: add "t" after %
    
    Register* ret = new Register(register_index , "reg" + to_string(register_index));
   register_index++;
    return ret;
}

Register* IRManager::getGlobalFreshReg(){
    // TODO: add "t" after %
    
    Register* ret = new Register(global_register_index, "@g_reg" + to_string(global_register_index));
    global_register_index++;
    return ret;
}

int IRManager::emitToBuffer(string command){
    
    int emit_result = codeBuffer.emit(" " + command + " ");
    //cout << "command is: " << command << endl;
    return emit_result;
}

void IRManager::emitGlobalToBuffer(string command){
    
    codeBuffer.emitGlobal(command);
    //cout << "global command is: " << command << endl;
}

void IRManager::endProgram(){
    
    codeBuffer.emitGlobal("\n \n; -----------------------  Program ------------------------ ");
    codeBuffer.printGlobalBuffer();
    codeBuffer.printCodeBuffer();
}

string IRManager::fromI8RegisterToI32Register(string type, string original_register){
    // This function recives an exp node. if it's int -> returns the register name. Else->
    // returns a register name, with the same value, but i32

    if(type == "byte"){
        Register* exp_i32 = getFreshReg();
        emitToBuffer("%" + exp_i32->getName() + " = zext i8 %" + original_register + " to i32");
        return exp_i32->getName();
    }
    
    return original_register;
}


void IRManager::loadID(string type, string reg, string id_name) {
    
    emitToBuffer("%" + reg + " = load i32, i32* " + "%p" + id_name);
    
}


void IRManager::zeroError(){
    
    emitGlobalToBuffer("@.zero_error  = constant [23 x i8] c\"Error division by zero\\00\"");

    emitToBuffer("call void @print(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.zero_error, i32 0, i32 0))");
    emitToBuffer("call void @exit(i32 0)");
    emitToBuffer("SHOULD NOT BE PRINTED");

}


BackpatchInfo IRManager::handlerDivZero(string exp_b_type, string exp_b_reg) {
    
    Register* zero_devision = getFreshReg();

    if(exp_b_type == "byte"){
        emitToBuffer("%" + zero_devision->getName() + " = icmp eq i8 %" + exp_b_reg + ", 0");
    }
    else{
        emitToBuffer("%" + zero_devision->getName() + " = icmp eq i32 %" + exp_b_reg + ", 0");
    }

    BackpatchInfo patching_info;
    patching_info.branch_location = emitToBuffer("br i1 %" + zero_devision->getName() + ", label @, label @");
    

    patching_info.label_true = codeBuffer.genLabel();
    zeroError();
    
    patching_info.label_false = codeBuffer.genLabel();

    return patching_info;
}

void IRManager::handlePatching(BackpatchInfo patching_info){
    
    pair<int, BranchLabelIndex>* p = new pair<int, BranchLabelIndex>({patching_info.branch_location, FIRST});
    codeBuffer.bpatch( codeBuffer.makelist(*p), patching_info.label_true);
    p = new pair<int, BranchLabelIndex>({patching_info.branch_location, SECOND});
    codeBuffer.bpatch( codeBuffer.makelist(*p), patching_info.label_false);
}

string IRManager::createLabel(){
    
    return codeBuffer.genLabel();
}

void IRManager::andPatching( ExpNode* node_a, ExpNode* node_b, LabelNode* MAlabel, ExpNode* resultExp){

    // if node_a true go to MAlabel
    codeBuffer.bpatch(list_of_labels[node_a->true_list_id], MAlabel->label);

    // erase the bpached list
    list_of_labels.erase(node_a->true_list_id);

    // if node_a false go to FalseList label
    // if node_b false go to FalseList label

    // merge false lists of node_a node_b
    list_of_labels[label_list_key_gen] = codeBuffer.merge(list_of_labels[node_a->false_list_id], 
                                                            list_of_labels[node_b->false_list_id]);
    resultExp->false_list_id = label_list_key_gen++;

    // erase merged lists
    list_of_labels.erase(node_a->false_list_id);         // erasing by key
    list_of_labels.erase(node_b->false_list_id);        // erasing by key


    // if node_b true go to TrueList label

    // the new truelist id is the right exp truelist
    // TODO check where node_b->true_list_id is initiallized
    resultExp->true_list_id = node_b->true_list_id;
    // TODO : not sure to use :
    // codeBuffer.emit(or_register + " = phi i1 [ 0 , %" + label_false + "], [ 1 , %" + label_true + "]");
}

void IRManager::orPatching( ExpNode* node_a, ExpNode* node_b, LabelNode* MOlabel, ExpNode* resultExp){
    
    // if node_a false go to MOlabel label
    codeBuffer.bpatch(list_of_labels[node_a->false_list_id], MOlabel->label);
    
    // erase the bpached list
    list_of_labels.erase(node_a->false_list_id);


    // if node_a true go to TrueList label
    // if node_b true go to TrueList label
    list_of_labels[label_list_key_gen] = codeBuffer.merge(list_of_labels[node_a->true_list_id], 
                                                            list_of_labels[node_b->true_list_id]);
    resultExp->true_list_id = label_list_key_gen++;

    // erase merged lists
    list_of_labels.erase(node_a->true_list_id);         // erasing by key
    list_of_labels.erase(node_b->true_list_id);        // erasing by key
    
    // if node_b false go to FalseList label
    // the new falselist id is the right exp falselist
    resultExp->false_list_id = node_b->false_list_id;
    // TODO : not sure to use :
    // codeBuffer.emit(or_register + " = phi i1 [ 0 , %" + label_false + "], [ 1 , %" + label_true + "]");

    }


void IRManager::createFalseListAndTrueList(ExpNode* bool_node, string bool_sign){
        
    int br_command_location = emitToBuffer("br label @");
	vector<pair<int,BranchLabelIndex>> false_list;
    vector<pair<int,BranchLabelIndex>> true_list;

    if(bool_sign == "1"){
        // we don't really need false_list
        false_list = vector<pair<int,BranchLabelIndex>>();
        true_list = codeBuffer.makelist({br_command_location,FIRST});
    }else{
        // we don't really need true_list
        true_list = vector<pair<int,BranchLabelIndex>>();
        false_list = codeBuffer.makelist({br_command_location,FIRST}); 
    }

    list_of_labels[label_list_key_gen] = true_list;
    bool_node->true_list_id = label_list_key_gen;
    label_list_key_gen++;

    list_of_labels[label_list_key_gen] = false_list;
    bool_node->false_list_id = label_list_key_gen;
    label_list_key_gen++;
	
}

void IRManager::expPassListNotRule(ExpNode* old_node, ExpNode* new_node){
    new_node->false_list_id = old_node->true_list_id;
    new_node->true_list_id = old_node ->false_list_id;
    return;
}

void IRManager::expRelopExpCreateBr(ExpNode* compare, ExpNode* exp1, ExpNode* exp2, RelopNode* compare_sign){
    string exp1_i32_register = fromI8RegisterToI32Register(exp1->type, exp1->llvm_reg);
    string exp2_i32_register = fromI8RegisterToI32Register(exp2->type, exp2->llvm_reg);
    
    emitToBuffer("%" + compare->llvm_reg + " = icmp %" + compare_sign->relop_sign + " i32 " + 
    exp1_i32_register + ", " + exp2_i32_register);

    int branch_location = emitToBuffer("br i1 %" + compare->llvm_reg + ", label @, label @");
	
	// make a new truelist
	vector<pair<int,BranchLabelIndex>> true_list = codeBuffer.makelist({branch_location,FIRST});
	list_of_labels[label_list_key_gen] = true_list;
	compare->true_list_id = label_list_key_gen;
    label_list_key_gen++;
	
	// make a new falselist
	vector<pair<int,BranchLabelIndex>> false_list = codeBuffer.makelist({branch_location,SECOND});
	list_of_labels[label_list_key_gen] = false_list;
	compare->false_list_id = label_list_key_gen;
    label_list_key_gen++;
    return;
}


void IRManager::patchIf( ExpNode* if_cond_exp , LabelNode* marker, 
                        BrNode* br_node ){

    string end_if_label = codeBuffer.genLabel();

    // if if_cond_exp true go to marker label
    codeBuffer.bpatch(list_of_labels[if_cond_exp->true_list_id], marker->label);
    // erase the bpached list
    list_of_labels.erase(if_cond_exp->true_list_id);
    // Still in true-> We've ended the IF statment, go to end of if
    codeBuffer.bpatch(codeBuffer.makelist({br_node->br_location,FIRST}), end_if_label);

    // if cond is false jump to end_if_label
	codeBuffer.bpatch(list_of_labels[if_cond_exp->false_list_id], end_if_label);
	list_of_labels.erase(if_cond_exp->false_list_id);
   
}

void IRManager::patchIfElse(ExpNode* if_cond_exp, 
                           LabelNode* marker_if_true, 
                           BrNode* go_to_end_from_if, 
                           LabelNode* marker_else, 
                           BrNode* go_to_end_from_else ){
    
    string end_ifelse_label = codeBuffer.genLabel();
     
    // if if_cond_exp true go to marker label
    codeBuffer.bpatch(list_of_labels[if_cond_exp->true_list_id], marker_if_true->label);
    // erase the bpached list
    list_of_labels.erase(if_cond_exp->true_list_id);

    codeBuffer.bpatch(list_of_labels[if_cond_exp->false_list_id], marker_else->label);
    // erase the bpached list
    list_of_labels.erase(if_cond_exp->false_list_id);

    // false_finish_branch & true_finish_branch should go to finish_label
	codeBuffer.bpatch(codeBuffer.makelist({go_to_end_from_if->br_location,FIRST}), end_ifelse_label);
	codeBuffer.bpatch(codeBuffer.makelist({go_to_end_from_else->br_location,FIRST}), end_ifelse_label);
                                                     
}


void IRManager::patchWhileNoElse( BrNode* go_to_before_exp, LabelNode* before_exp_marker,
                      ExpNode* exp_node, LabelNode* after_exp_marker, BrNode* go_to_check_exp ){

    string end_while_label = codeBuffer.genLabel();

    // if exp == true
    codeBuffer.bpatch(list_of_labels[exp_node->true_list_id], after_exp_marker->label);
    list_of_labels.erase(exp_node->true_list_id);

    codeBuffer.bpatch(list_of_labels[exp_node->false_list_id], end_while_label);
    list_of_labels.erase(exp_node->false_list_id);

    // check exp condition
    codeBuffer.bpatch(codeBuffer.makelist({go_to_before_exp->br_location,FIRST}), before_exp_marker->label);
    codeBuffer.bpatch(codeBuffer.makelist({go_to_check_exp->br_location,FIRST}), before_exp_marker->label);

    // break branches should go to loop_end_label
	codeBuffer.bpatch(break_list[break_list.size() - 1], end_while_label);
	break_list.pop_back();

	// continue branches should go to cond_label
	codeBuffer.bpatch(continue_list[continue_list.size() -1], before_exp_marker->label);
	continue_list.pop_back();

    return;
}

void IRManager::patchWhileElse ( BrNode* go_to_before_exp, LabelNode* before_exp_marker,
                     ExpNode* exp_node,  LabelNode* after_exp_marker, BrNode* go_to_check_exp,
                     LabelNode* else_marker, BrNode* end_else){

    string end_while_label = codeBuffer.genLabel();

    // if exp == true
    codeBuffer.bpatch(list_of_labels[exp_node->true_list_id], after_exp_marker->label);
    list_of_labels.erase(exp_node->true_list_id);

    codeBuffer.bpatch(list_of_labels[exp_node->false_list_id], else_marker->label);
    list_of_labels.erase(exp_node->false_list_id);

    // check exp condition
    codeBuffer.bpatch(codeBuffer.makelist({go_to_before_exp->br_location,FIRST}), before_exp_marker->label);
    codeBuffer.bpatch(codeBuffer.makelist({go_to_check_exp->br_location,FIRST}), before_exp_marker->label);

    codeBuffer.bpatch(codeBuffer.makelist({end_else->br_location, FIRST}), end_while_label);

    // break branches should go to loop_end_label
	codeBuffer.bpatch(break_list[break_list.size() - 1], end_while_label);
	break_list.pop_back();

	// continue branches should go to cond_label
	codeBuffer.bpatch(continue_list[continue_list.size() -1], before_exp_marker->label);
	continue_list.pop_back();

    return;
}

void IRManager::handleBreak(){
    int br_location = emitToBuffer("br label @");
    break_list[break_list.size() - 1].push_back({br_location, FIRST});
}

void IRManager::handleContinue(){
    int br_location = emitToBuffer("br label @");
    continue_list[break_list.size() - 1].push_back({br_location, FIRST});
}

int IRManager::goToNext(){
    // cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ IRManager::goToNext"<< endl;
    return emitToBuffer("br label @");
}

void IRManager::enterLoop(){
    loop_counter++;
    break_list.push_back(vector<pair<int,BranchLabelIndex>>());
	continue_list.push_back(vector<pair<int,BranchLabelIndex>>());
}

void IRManager::definePrintAndPrinti() {
    emitGlobalToBuffer("@.str_specifier = constant [4 x i8] c\"%s\\0A\\00\"");
    emitGlobalToBuffer("@.int_specifier = constant [4 x i8] c\"%d\\0A\\00\"");
    
    emitGlobalToBuffer("declare void @exit(i32) ");
    emitGlobalToBuffer("declare i32 @printf(i8*, ...)");

    emitToBuffer("define void @printi(i32) { call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0), i32 %0) ret void}");
    emitToBuffer("define void @print(i8*) { call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0), i8* %0) ret void }");
}

void IRManager::defineNewFunction(IdNode* id_node, string type, vector<VarNode*> params){
    string llvm_function_type = "";
    if(type == "void"){
        llvm_function_type = "void";
    } else{
        llvm_function_type = "i32";
    }
    
    string params_list = "";
    if(params.size() > 0){
        params_list += "i32";
    
        for(int i = 0; i < params.size() - 1; i++){
            params_list += ", i32";
        }
    }

    //TODO: check how to add the parameters correctly!!
    //cout << "NOT SURE HOW TO ADD VARIABLES TO FUNCTION!!!" << endl;

    emitToBuffer("define " + llvm_function_type + " @" + id_node->name + "(" + params_list + ") { ");	

    emitToBuffer("	; ======================= input arguments definition =======================");

    for(int i = 0; i < params.size() ; i++){

        // allocate empty space in stack and save on the param.llvm_reg
        emitToBuffer ( "%p" + params[i]->llvm_reg + " = alloca i32");

        // copy the value back to function stack and params[i]->llvm_reg points to it
        emitToBuffer("store i32 %"+ params[i]->llvm_reg + ", i32* %p" + params[i]->llvm_reg );
    }

    emitToBuffer("	; =========================== stack definition =============================");

    emitToBuffer("%stack = alloca [50 x i32]");

    emitToBuffer("	; ============================= function body ==============================");
    
}

void IRManager::startBoolJump(ExpNode* exp_node){

    // if true
    string true_lable = codeBuffer.genLabel();
    codeBuffer.bpatch(list_of_labels[exp_node->true_list_id], true_lable);
    list_of_labels.erase(exp_node->true_list_id);
    int true_branch_location = emitToBuffer("br label @");

    //if false
    string false_lable = codeBuffer.genLabel();
    codeBuffer.bpatch(list_of_labels[exp_node->false_list_id], true_lable);
    list_of_labels.erase(exp_node->false_list_id);
    int false_branch_location = emitToBuffer("br label @");

    //finally
    string end_lable = codeBuffer.genLabel();
    codeBuffer.bpatch(codeBuffer.makelist({true_branch_location, FIRST}), end_lable);
    codeBuffer.bpatch(codeBuffer.makelist({false_branch_location, FIRST}), end_lable);

    emitToBuffer("%" + exp_node->llvm_reg + " = phi i32 [1, %" + true_lable + "], [0, %" + false_lable + "]");
}

string _addFunctionSingleVar(ExpNode* current_node){
    if(current_node->type == "string"){
        return "i8* getelementptr inbounds ([" + ((StringNode*)current_node)->size + \
         " x i8], [" + ((StringNode*)current_node)->size + " x i8]* " + current_node->llvm_reg + ", i32 0, i32 0)";
    }
    else{
        return "i32 " + current_node->llvm_reg;
    }
}

void IRManager::handleCallFunction(FuncNode* func_node, ExpList* params_list, ExpNode* returned_value){

    //TODO: check this call function- not sure if sending variables value currectly

    string call_start = "";
    if(func_node->type == "void"){
        call_start = "call void @";
    }else{
        call_start = returned_value->llvm_reg + " = call i32 @";
    }

    string call_func_variables = "";

    
    if(params_list != NULL && params_list->params.size() > 0){
        call_func_variables = _addFunctionSingleVar(params_list->params[0]);
        for(int i = 1; i < params_list->params.size(); i++){
            call_func_variables += ", " + _addFunctionSingleVar(params_list->params[i]);
        }
    }

    emitToBuffer(call_start + func_node->name + "(" + call_func_variables + ")");

    // Check if the function is bool- and patc
    if(func_node->type == "bool"){
        Register* fresh_reg = getFreshReg();

        emitToBuffer("%" + fresh_reg->getName() + " = icmp eq i32 %" + returned_value->llvm_reg + ", 1");

       	int branch_location = emitToBuffer( "br i1 %" + fresh_reg->getName() + ", label @, label @");
		
		list_of_labels[label_list_key_gen] = codeBuffer.makelist({branch_location,FIRST});
		returned_value->true_list_id = label_list_key_gen;
        label_list_key_gen++;
		
        list_of_labels[label_list_key_gen] = codeBuffer.makelist({branch_location,SECOND});
		returned_value->false_list_id = label_list_key_gen;
		label_list_key_gen++;
    }
}

void IRManager::callToFunctionBackPatch(ExpNode* function_returned_value){

    string end_lable = codeBuffer.genLabel();
    codeBuffer.bpatch(list_of_labels[function_returned_value->true_list_id], end_lable);
    codeBuffer.bpatch(list_of_labels[function_returned_value->false_list_id], end_lable);
    list_of_labels.erase(function_returned_value->true_list_id);
    list_of_labels.erase(function_returned_value->false_list_id);

}

void IRManager::returnFromNonVoidFunction(string func_type, ExpNode* return_value){

    if(func_type == "bool"){
        string true_lable = codeBuffer.genLabel();
        codeBuffer.bpatch(list_of_labels[return_value->true_list_id], true_lable);
        list_of_labels.erase(return_value->true_list_id);
        emitToBuffer("ret i32 1");

        string false_lable = codeBuffer.genLabel();
        codeBuffer.bpatch(list_of_labels[return_value->false_list_id], false_lable);
        list_of_labels.erase(return_value->false_list_id);
        emitToBuffer("ret i32 0");
    }else{
        emitToBuffer("ret i32" + return_value->llvm_reg);
    }

}

void IRManager::newFuncScope (){
    register_index = 0;
}

                        