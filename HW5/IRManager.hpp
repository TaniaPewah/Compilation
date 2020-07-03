#ifndef IR_MANAGER_HPP
#define IR_MANAGER_HPP

using namespace std;
#include <iostream>
#include <string>
#include <unordered_map> 
#include "bp.hpp"
#include "hw3_output.hpp"

class VarNode;
class ExpNode;
class LabelNode;
class RelopNode;
class BrNode;
class IdNode;
class ExpList;
class FuncNode;

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
    string label_false;
    string label_true;
};


class IRManager {
public:
   static IRManager *instance;
    int register_index = 0;
    int global_register_index = 0;
    int stack_offset_pointer = 0;
    CodeBuffer& codeBuffer = CodeBuffer::instance();
    unordered_map<int, vector<pair<int,BranchLabelIndex>>> list_of_labels;
    int label_list_key_gen = 0;
    int loop_counter = 0;
    vector<vector<pair<int,BranchLabelIndex>>> break_list;
	vector<vector<pair<int,BranchLabelIndex>>> continue_list;

   // Private constructor so that no objects can be created.
   IRManager() { 
       list_of_labels = unordered_map<int, vector<pair<int,BranchLabelIndex>>>();
    }

    IRManager(IRManager const&)       = delete;
    IRManager& operator=(IRManager const&)  = delete;
    static IRManager *getInstance() {
        if (!instance)
        instance = new IRManager;
        return instance;
    }

    int addPointerToRegisterInStack(string llvm_reg);
    void assignExpNodeToVar(VarNode* variable, ExpNode* exp_node);
    Register* getFreshReg();
    Register* getGlobalFreshReg();
    int emitToBuffer(string command);
    void emitGlobalToBuffer(string command);
    void endProgram();
    string fromI8RegisterToI32Register(string type, string original_register);
    void loadID(string type, string reg, string id_name);

    void zeroError();
    BackpatchInfo handlerDivZero(string exp_b_type, string exp_b_reg);
    void handlePatching(BackpatchInfo patching_info);
    string createLabel();
    void andPatching( ExpNode* node_a, ExpNode* node_b, LabelNode* MAlabel, ExpNode* resultExp);
    void orPatching( ExpNode* node_a, ExpNode* node_b, LabelNode* MOlabel, ExpNode* resultExp);
    void createFalseListAndTrueList(ExpNode* bool_node, string bool_sign);
    void expPassListNotRule(ExpNode* old_node, ExpNode* new_node);
    void expRelopExpCreateBr(ExpNode* compare, ExpNode* exp1, ExpNode* exp2, RelopNode* compare_sign);
    
    void patchIf( ExpNode* if_cond_exp , LabelNode* marker, BrNode* br_node );
    void patchIfElse( ExpNode* if_cond_exp, 
                      LabelNode* marker_if_true, 
                      BrNode* go_to_end_from_if, 
                      LabelNode* marker_else, 
                      BrNode* go_to_end_from_else );
    int goToNext( );

    void patchWhileNoElse( BrNode* go_to_before_exp, LabelNode* before_exp_marker,
                     ExpNode* exp_node,  LabelNode* after_exp_marker, BrNode* go_to_check_exp );
    
    void patchWhileElse ( BrNode* go_to_before_exp, LabelNode* before_exp_marker,
                     ExpNode* exp_node,  LabelNode* after_exp_marker, BrNode* go_to_check_exp, 
                     LabelNode* else_marker, BrNode* end_else);

    void handleBreak();
    void enterLoop();
    void handleContinue();
    void definePrintAndPrinti();
    void defineNewFunction(IdNode* id_node, string type, vector<VarNode*> params);

    void startBoolJump(ExpNode* exp_node);
    void handleCallFunction(FuncNode* func_node, ExpList* params_list, ExpNode* returned_value);
    void callToFunctionBackPatch(ExpNode* function_returned_value);

    void returnFromNonVoidFunction(string func_type, ExpNode* return_value);
    void newFuncScope ();
    void bpatchBool(VarNode* variable, ExpNode* exp_node);
    void getExpNodeValueAndBranch(VarNode* var, ExpNode* exp_node);
    string getFreshVarReg();
};

#endif //IR_MANAGER_HPP