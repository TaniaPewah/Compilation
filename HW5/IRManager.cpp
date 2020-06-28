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

    cout << "merging OR true lists" << endl;

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
};



}
