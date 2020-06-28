
#ifndef IR_MANAGER_HPP
#define IR_MANAGER_HPP

using namespace std;
#include <iostream>
#include <string>
#include "bp.hpp"


class Register {
	int num;
    string name;
	bool used;
    
public: 
    Register() : num(0), name("invalid"), used(false) {}
    Register( int num, string name) : num(num), name(name), used(false){}
    bool isFree() {
		return !used;
	}
	void free() {
		used = false;
	}
	void setUsed() {
		used = true;
	}
	int getNum() {
		return num;
	}
	string getName() {
		return name;
	}
};

struct BackpatchInfo
{
    /* data */
    int branch_location;
    string lable_false;
    string lable_true;
};


class IRManager {
private:
   static IRManager *instance;
    int register_index = 0;
    int global_register_index = 0;
    int stack_offset_pointer = 0;
    CodeBuffer& codeBuffer = CodeBuffer::instance();

   // Private constructor so that no objects can be created.
   IRManager() {  }

   public:
    IRManager(IRManager const&)       = delete;
    IRManager& operator=(IRManager const&)  = delete;
    static IRManager *getInstance() {
        if (!instance)
        instance = new IRManager;
        return instance;
    }

    int addPointerToRegisterInStack(string llvm_reg){
        // adds a variable to the stack, and creates a register that is a pointer to that location.
        // this register will always be a pointer to the stack
        cout << "REMEMBER TO INITIATE %stack LATER!!!" <<endl;
        //TODO: initiate %stack. refer: line 104, parser_hw5.hpp
        emitToBuffer(llvm_reg + " = getelementptr [50 x i32], [50 x i32]* %stack, i32 0, i32 " + to_string(stack_offset_pointer));
        stack_offset_pointer++;
        emitToBuffer("store i32 0, i32* " + llvm_reg);
        return stack_offset_pointer - 1;
    }

    void assignExpNodeToVar(string variable_reg, string exp_node_reg, string exp_node_type){
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

    Register* getFreshReg(){
        // TODO: add "t" after %
        Register* ret = new Register(register_index, "%.reg" + to_string(register_index));
        register_index++;
        return ret;
    }

    Register* getGlobalFreshReg(){
        // TODO: add "t" after %
        Register* ret = new Register(register_index, "@.g_reg" + to_string(global_register_index));
        global_register_index++;
        return ret;
    }

    int emitToBuffer(string command){
        int emit_result = codeBuffer.emit(command);
        cout << "command is: " << command << endl;
        return emit_result;
    }

    void emitGlobalToBuffer(string command){
        codeBuffer.emitGlobal(command);
        cout << "global command is: " << command << endl;
    }

    void endProgram(){
        codeBuffer.emitGlobal("\n \n; -----------------------  Program ------------------------ ");
        codeBuffer.printGlobalBuffer();
        codeBuffer.printCodeBuffer();
    }

    string fromI8RegisterToI32Register(string type, string original_register){
        // This function recives an exp node. if it's int -> returns the register name. Else->
        // returns a register name, with the same value, but i32
    
        if(type == "byte"){
            Register* exp_i32 = getFreshReg();
            emitToBuffer(exp_i32->getName() + " = zext i8 " + original_register + " to i32");
            return exp_i32->getName();
        }
        
        return original_register;
    }
    
 
    void loadID(string type, string reg, string id_name) {
        if(type =="int"){
            this->emitToBuffer(reg + " = load i32, i32* " + id_name);
        }
        else if(type=="byte"){
            this->emitToBuffer(reg + " = load i1, i1* " + id_name);
        }
        else if(type=="bool"){
            this->emitToBuffer(reg + " = load i8, i8* " + id_name);
        }
        else {
            cout<<"%%% WHAT TYPE IS THAT"<< endl;
        }
    }

    void exitFunction(){
        string s;
        s = "  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.forPrintString, i32 0, i32 0), i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.1, i32 0, i32 0))\n"
        "  call void @exit(i32 0) \n"
        "  unreachable\n";
        codeBuffer.emit(s);

}

    void zeroExit(){
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

    void zeroError(){
        codeBuffer.emit("call void @print_error_exit()");
        codeBuffer.emit("unreachable");
    }

    BackpatchInfo handlerDivZero(string exp_b_type, string exp_b_reg) {
        Register* zero_devision = getFreshReg();

        if(exp_b_type == "byte"){
            emitToBuffer(zero_devision->getName() + " = icmp eq i8 " + exp_b_reg + ", 0");
        }
        else{
            emitToBuffer(zero_devision->getName() + " = icmp eq i32 " + exp_b_reg + ", 0");
        }

        BackpatchInfo patching_info;
        patching_info.branch_location = emitToBuffer("br i1 " + zero_devision->getName() + ", label @, label @");
        

        patching_info.lable_true = codeBuffer.genLabel();
        zeroError();
        
        patching_info.lable_false = codeBuffer.genLabel();

        return patching_info;
    }

    void handlePatching(BackpatchInfo patching_info){
        pair<int, BranchLabelIndex>* p = new pair<int, BranchLabelIndex>({patching_info.branch_location, FIRST});
        codeBuffer.bpatch(codeBuffer.makelist(*p), patching_info.lable_true);
        p = new pair<int, BranchLabelIndex>({patching_info.branch_location, SECOND});
        codeBuffer.bpatch(codeBuffer.makelist(*p), patching_info.lable_false);
    }

    string createLabel(){
        return codeBuffer.genLabel();
    }

    void andPatching( ExpNode* node_a, ExpNode* node_b, LabelNode* label ){
        pair<int, BranchLabelIndex> *p = new pair<int, BranchLabelIndex>({label->location, FIRST});
    }
};

#endif //IR_MANAGER_HPP
