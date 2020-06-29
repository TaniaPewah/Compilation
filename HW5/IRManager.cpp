#include "IRManager.hpp"
#include "parser.hpp"

IRManager* IRManager::instance = 0;

int IRManager::addPointerToRegisterInStack(string llvm_reg){
        // adds a variable to the stack, and creates a register that is a pointer to that location.
        // this register will always be a pointer to the stack
        cout << "REMEMBER TO INITIATE %stack LATER!!!" <<endl;
        //TODO: initiate %stack. refer: line 104, parser_hw5.hpp
        emitToBuffer(llvm_reg + " = getelementptr [50 x i32], [50 x i32]* %stack, i32 0, i32 " + to_string(stack_offset_pointer));
        stack_offset_pointer++;
        emitToBuffer("store i32 0, i32* " + llvm_reg);
        return stack_offset_pointer - 1;
    }

void IRManager::assignExpNodeToVar(string variable_reg, string exp_node_reg, string exp_node_type){
    // the variable is a register which points to a location in the string. we just neet to update the value in this location.
    if(exp_node_type == "int"){
        emitToBuffer("store i32 " + exp_node_reg + ", i32* " + variable_reg);
    }else if(exp_node_type == "byte"){
        Register* fresh_register = getFreshReg();
        emitToBuffer(fresh_register->getName() + " = zext i8 " + exp_node_reg + " to i32");
        emitToBuffer("store i32 " + fresh_register->getName() + ", i32* " + variable_reg);
    }else{
        Register* fresh_register = getFreshReg();
        emitToBuffer(fresh_register->getName() + " = zext i1 " + exp_node_reg + " to i32");
        emitToBuffer("store i32 " + fresh_register->getName() + ", i32* " + variable_reg);
    }
    return;
}

Register* IRManager::getFreshReg(){
    // TODO: add "t" after %
    
    Register* ret = new Register(register_index , "%.reg" + to_string(register_index));
   register_index++;
    return ret;
}

Register* IRManager::getGlobalFreshReg(){
    // TODO: add "t" after %
    
    Register* ret = new Register(global_register_index, "@.g_reg" + to_string(global_register_index));
    global_register_index++;
    return ret;
}

int IRManager::emitToBuffer(string command){
    
    int emit_result = codeBuffer.emit(command);
    cout << "command is: " << command << endl;
    return emit_result;
}

void IRManager::emitGlobalToBuffer(string command){
    
    codeBuffer.emitGlobal(command);
    cout << "global command is: " << command << endl;
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
        emitToBuffer(exp_i32->getName() + " = zext i8 " + original_register + " to i32");
        return exp_i32->getName();
    }
    
    return original_register;
}


void IRManager::loadID(string type, string reg, string id_name) {
    if(type =="int"){
        emitToBuffer(reg + " = load i32, i32* " + id_name);
    }
    else if(type=="byte"){
        emitToBuffer(reg + " = load i1, i1* " + id_name);
    }
    else if(type=="bool"){
        emitToBuffer(reg + " = load i8, i8* " + id_name);
    }
    else {
        cout<<"%%% WHAT TYPE IS THAT"<< endl;
    }
}

void IRManager::exitFunction(){
    
    string s;
    s = "  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.forPrintString, i32 0, i32 0), i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.1, i32 0, i32 0))\n"
    "  call void @exit(i32 0) \n"
    "  unreachable\n";
    codeBuffer.emit(s);

}

void IRManager::zeroExit(){
    
    string s = "@.str.1 = private unnamed_addr constant [23 x i8] c\"Error division by zero\\00\"\n"
            "\n"
            "\n"
            "define void @print_error_exit()  {\n"
            "print_error_exit_entry:\n"
            "  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.forPrintString, i32 0, i32 0), i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.1, i32 0, i32 0))\n"
            "  call void @exit(i32 0) \n"
            "  unreachable\n"
            "                                                  ; No predecessors!\n"
            "  ret void\n"
            "}";
    codeBuffer.emitGlobal(s);
}

void IRManager::zeroError(){
    
    codeBuffer.emit("call void @print_error_exit()");
    codeBuffer.emit("unreachable");
}


BackpatchInfo IRManager::handlerDivZero(string exp_b_type, string exp_b_reg) {
    
    Register* zero_devision = getFreshReg();

    if(exp_b_type == "byte"){
        emitToBuffer(zero_devision->getName() + " = icmp eq i8 " + exp_b_reg + ", 0");
    }
    else{
        emitToBuffer(zero_devision->getName() + " = icmp eq i32 " + exp_b_reg + ", 0");
    }

    BackpatchInfo patching_info;
    patching_info.branch_location = emitToBuffer("br i1 " + zero_devision->getName() + ", label @, label @");
    

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

    cout<< "merging AND false lists" << endl;

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

    cout << "merging OR true lists:" << endl;
    cout <<" node_a truelist: " << node_a->true_list_id << endl;
    cout <<" node_b truelist: " << node_b->true_list_id << endl;

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

    cout <<" node_a truelist: " << bool_node->true_list_id << endl;
	
}

void IRManager::expPassListNotRule(ExpNode* old_node, ExpNode* new_node){
    new_node->false_list_id = old_node->true_list_id;
    new_node->true_list_id = old_node ->false_list_id;
    return;
}

void IRManager::expRelopExpCreateBr(ExpNode* compare, ExpNode* exp1, ExpNode* exp2, RelopNode* compare_sign){
    string exp1_i32_register = fromI8RegisterToI32Register(exp1->type, exp1->llvm_reg);
    string exp2_i32_register = fromI8RegisterToI32Register(exp2->type, exp2->llvm_reg);
    
    emitToBuffer(compare->llvm_reg + " = icmp " + compare_sign->relop_sign + " i32 " + 
    exp1_i32_register + ", " + exp2_i32_register);

    int branch_location = emitToBuffer("br i1 " + compare->llvm_reg + ", label @, label @");
	
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
                        StatementNode* statement, StatementNode* result_state ){

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ IRManager::patchIf"<< endl;
    // if if_cond_exp true go to marker label
    codeBuffer.bpatch(list_of_labels[if_cond_exp->true_list_id], marker->label);
    // erase the bpached list
    list_of_labels.erase(if_cond_exp->true_list_id);

    // if node_a true go to TrueList label
    // if node_b true go to TrueList label
    list_of_labels[label_list_key_gen] = codeBuffer.merge(list_of_labels[if_cond_exp->false_list_id], 
                                                            list_of_labels[statement->next_list_id]);
    result_state->next_list_id = label_list_key_gen++;

    // erase merged lists
    list_of_labels.erase(if_cond_exp->true_list_id);         // erasing by key
    list_of_labels.erase(statement->next_list_id);        // erasing by key
}

void IRManager::patchIfElse( ExpNode* if_cond_exp , LabelNode* marker_if_st, 
                           StatementNode* if_statement, StatementNode* go_to_else, 
                           LabelNode* marker_else_st, StatementNode* else_statement, StatementNode* result_state ){
    
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ IRManager::patchIfElse"<< endl;
    // if if_cond_exp true go to marker label
    codeBuffer.bpatch(list_of_labels[if_cond_exp->true_list_id], marker_if_st->label);
    // erase the bpached list
    list_of_labels.erase(if_cond_exp->true_list_id);

    codeBuffer.bpatch(list_of_labels[if_cond_exp->false_list_id], marker_else_st->label);
    // erase the bpached list
    list_of_labels.erase(if_cond_exp->false_list_id);

    vector<pair<int,BranchLabelIndex>> temp = codeBuffer.merge(list_of_labels[if_statement->next_list_id], 
                                                               list_of_labels[go_to_else->next_list_id]);

    list_of_labels[label_list_key_gen] = codeBuffer.merge(temp, list_of_labels[else_statement->next_list_id]);                                                           
               
    result_state->next_list_id = label_list_key_gen++;

    // erase merged lists
    list_of_labels.erase(if_statement->next_list_id);         // erasing by key
    list_of_labels.erase(go_to_else->next_list_id);        // erasing by key
    list_of_labels.erase(else_statement->next_list_id);
}


void IRManager::patchWhileNoElse( StatementNode* statment_node, LabelNode* before_exp_marker,LabelNode* after_exp_marker,
 ExpNode* exp_node , StatementNode* returned_statment){

    codeBuffer.bpatch(list_of_labels[statment_node->next_list_id], before_exp_marker->label);
    // erase the bpached list
    list_of_labels.erase(statment_node->next_list_id);

    codeBuffer.bpatch(list_of_labels[exp_node->true_list_id], after_exp_marker->label);
    // erase the bpached list
    list_of_labels.erase(exp_node->true_list_id);

    returned_statment->next_list_id = exp_node->false_list_id;

    emitToBuffer("br label " + before_exp_marker->label);

    return;
}

void IRManager::goToNext( StatementNode* returned ){
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ IRManager::goToNext"<< endl;
    int branch_location = emitToBuffer("br label @");
    vector<pair<int,BranchLabelIndex>> next_list = codeBuffer.makelist({branch_location, FIRST});
	list_of_labels[label_list_key_gen] = next_list;
	returned->next_list_id = label_list_key_gen;
    label_list_key_gen++;
}

void IRManager::patchStatements( StatementNode* statments_node, LabelNode* before_statement_marker, 
                                   StatementNode* statment_node, StatementNode* returned){
    codeBuffer.bpatch(list_of_labels[statments_node->next_list_id], before_statement_marker->label);
    // erase the bpached list
    list_of_labels.erase(statments_node->next_list_id);
    returned->next_list_id = statment_node->next_list_id;
}

                        