//
// Created by hilak on 1/11/2020.
//

#ifndef INC_236360_HW5_LLVM_ASSEMBLY_HPP
#define INC_236360_HW5_LLVM_ASSEMBLY_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include "bp.hpp"
#include "Table.hpp"
#include "parser_actions.h"


using namespace std;
extern CodeBuffer& codeBuffer;
extern Table * symbols_table;


extern vector<string> stam;//just a vector for inputing var
extern vector<int> stam_lineno;//just a vector for inputing var
extern vector<E_TYPE> empty;
extern int while_ind ;
extern string current_func ;

void emit_safe_alloc_Type_ID_ASSIGN_Exp_Setup(Node_Az * $$ , Node_Az *$1, Node_Az *$2, string new_var);

//extern int id_next_available_for_temp_var;

int global_reg_index = 0;
int global_num_index = 0;
int fresh_global_init_index = 0;

string freshRegister(){
    std::stringstream reg;
    reg << "%";
    reg << ".t";
    reg << global_reg_index;
    global_reg_index++;
    return reg.str();
}

string freshGlobalNumber(){
    std::stringstream num;
    num << global_num_index;
    global_num_index++;
    return num.str();
}

string freshGlobalInitNumber(){
    std::stringstream num;
    num << fresh_global_init_index;
    fresh_global_init_index++;
    return num.str();
}

string numToString(int num){
    std::stringstream str_num;
    str_num << num;
    return str_num.str();
}

int emitCommand(const string &command, const string &comment) {
     stringstream ss;
    auto actualComment = comment.empty() ? "" : "; " + comment;
    if (!command.empty()) {
        ss << left << setw(50) << command;
    }
    ss << actualComment;
   return codeBuffer.emit(ss.str());
}

void emit_printIConstant(){
    codeBuffer.emitGlobal("@.forPrinti = internal constant [4 x i8] c\"%d\\0A\\00\"");
}

void emit_printConstant(){
//    codeBuffer.emit("@.forPrintString = internal constant [4 x i8] c\"%s\\0A\\00\"");
    codeBuffer.emitGlobal("@.forPrintString = internal constant [4 x i8] c\"%s\\0A\\00\"");
}

void emit_func_declartion(string dec){
    codeBuffer.emitGlobal(dec);
}

void emit__print_zeroConstant(){
    codeBuffer.emitGlobal("@.str.1 = private unnamed_addr constant [23 x i8] c\"Error division by zero\\00\", align 1");
}
//prints to global!
void emit_func_zero_exit(){
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

void emit_exit(){
    string s;
    s= "  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.forPrintString, i32 0, i32 0), i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.1, i32 0, i32 0))\n"
       "  call void @exit(i32 0) \n"
       "  unreachable\n";
    codeBuffer.emit(s);

}

void test_emit_void_function_to_codeBuffer(string func_name, string ret_type, string input_parameters_types, string internal_code){
    codeBuffer.emitGlobal("define "+ ret_type+ " @"+func_name+"("+input_parameters_types+") {");
    codeBuffer.emitGlobal(func_name+"_entry:");
    codeBuffer.emitGlobal(internal_code);
    codeBuffer.emitGlobal("ret "+ ret_type);
    codeBuffer.emitGlobal("}");
    codeBuffer.emitGlobal("");
    codeBuffer.emitGlobal("");

}

void test_emit_ret_type_function_to_codeBuffer(string func_name, string ret_type, string input_parameters_types, string internal_code){
    codeBuffer.emit("define "+ ret_type+ " @"+func_name+"("+input_parameters_types+") {");
    codeBuffer.emit(func_name+"_entry:");
    codeBuffer.emit(internal_code);
//    codeBuffer.emit("ret "+ ret_type);
    codeBuffer.emit("}");
    codeBuffer.emit("");
    codeBuffer.emit("");
}

void emit_printi_function_buffer(){
    test_emit_void_function_to_codeBuffer("printi", "void", "i32*",
                                          " %1 = load i32, i32* %0\n %2 = call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8],[4 x i8]* @.forPrinti, i32 0, i32 0), i32 %1)");
}

void emit_print_function_buffer(){
    test_emit_void_function_to_codeBuffer("print", "void", "i8*",
                                          " %1 = call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8],[4 x i8]* @.forPrintString, i32 0, i32 0), i8* %0)");
}

void test_emit_code_buffer_main_begin(){
    string main_begin = "define void @main() {\n"
                   "main_entry:";

    codeBuffer.emit(main_begin);
}
//
//void test_emit_code_buffer_main_end(){
////    string main_end = ""
//                      //"ret void\n"
////                      "}";
//    codeBuffer.emit("}");
//}

void emit_exp_main_function_buffer(){
    string internal_code = "%0 = alloca i32\n"
                           "  store i32 333333333, i32* %0\n"
                          // "  call void @print(i32* %0)\n"
                           "  call void @printi(i32* %0)\n"
                           "\n"
                           "  ret i32 0";

    test_emit_ret_type_function_to_codeBuffer("main", "i32", "", internal_code);
}

//
//void emit_codeBuffer_tem_register_num(int val, string action, string op_register){
//    stringstream ss;
//    stringstream ss_comment;
//    if(action == "add"){
//       // ss <<"%.t"<<id_next_available_for_temp_var<<" = "<<action << " i32 0, " << val;
//    }
////    ss_comment << "init temp reg .t"<<id_next_available_for_temp_var;
//
//    ss_comment << " with literal "<<val;
//    emitCommand(ss.str(),ss_comment.str());
//    //id_next_available_for_temp_var++;
//}


void test(){
//    emit_exp_main_function_buffer();
//    codeBuffer.printGlobalBuffer();
//    codeBuffer.printCodeBuffer();
}


void emit_zero_error_and_exit(){
    codeBuffer.emit("call void @print_error_exit()");
    codeBuffer.emit("unreachable");
}

//input: temp_reg_id is in form %<reg>
void emit_and_print_temp_reg(string llvm_type,string temp_reg_id){
    string fresh_reg_on_buffer = freshRegister();
    emitCommand(fresh_reg_on_buffer+" = alloca i32","emiting so we can  print in emit_and_print_temp_reg");
    emitCommand("store "+llvm_type+" "+temp_reg_id+", i32 * "+fresh_reg_on_buffer,"emiting so we can  print in emit_and_print_temp_reg");
    string str_printi_val = "call void @printi(i32* "+fresh_reg_on_buffer+")";
    //codeBuffer.emit(str_printi_val);
    emitCommand(str_printi_val,"print for test");
    //sing str_printi_val = "call void @printi(i32 "+temp_reg_id+")";
    //codeBuffer.emit(str_printi_val);

}

void emit_and_print_variable(string llvm_type, string reg_variable){
    string fresh_reg_on_buffer ;
    string reg ;
    string reg_zext ;
    string str_printi_val;
    codeBuffer.emit("");

    if(llvm_type != "i32"){
         fresh_reg_on_buffer = freshRegister();
         reg = freshRegister();
         reg_zext = freshRegister();

        emitCommand(fresh_reg_on_buffer+" = alloca i32","emiting so we can  print in emit_and_print_temp_reg");
        emitCommand(reg + " = load " + llvm_type + ", " + llvm_type + "* " + reg_variable, llvm_type + " to i32");
        emitCommand(reg_zext+" = zext "+ llvm_type+" "+reg+" to i32",llvm_type+" to i32");
        emitCommand("store i32 "+reg_zext+", i32 * "+fresh_reg_on_buffer,"");
        str_printi_val = "call void @printi(i32* "+fresh_reg_on_buffer+")";
        emitCommand(str_printi_val,"print for test");
    }else {
//        emitCommand(fresh_reg_on_buffer+" = alloca i32","emiting so we can  print in emit_and_print_temp_reg");
//        emitCommand(reg + " = load " + llvm_type + ", " + llvm_type + "* " + reg_variable, llvm_type + " to i32");
        str_printi_val = "call void @printi(i32* "+reg_variable+")";
        emitCommand(str_printi_val,"print for test");
    }

    codeBuffer.emit("");
}

void emit_and_print_ID_saved( string default_val_type,Node_Az *$1){


    if(default_val_type== "INT"){
        default_val_type = "i32";

    } else if(default_val_type == "BYTE"){
        default_val_type = "i8";

    } else if (default_val_type== "BOOL"){
        default_val_type = "i1";
    }
//    cout << default_val_type <<endl;
    emit_and_print_variable(default_val_type, "%"+$1->name+symbols_table->getGlobalName($1->name));
    emitCommand(freshRegister()+" = add i32 1 , 1","dummy");
}

void aid_func_Type_ID_ASSIGN_Exp_SC (Node_Az * $$ ,Node_Az *$1,Node_Az *$2,Node_Az *$3,Node_Az *$4){
//    $$->llvm_register = "%"+$2->name;
    string new_var = $$->llvm_register;
    string default_val_type ;
    string reg_from_Exp = $4->llvm_register;
    string init_val;

    if($1->type == "INT"){
        new_var += " = alloca i32";
        default_val_type = "i32";

    } else if($1->type == "BYTE"){
        new_var += " = alloca i8";
        default_val_type = "i8";

    } else if ($1->type == "BOOL"){
        new_var += " = alloca i1";
        default_val_type = "i1";
    }

//    emit_safe_alloc_Type_ID_ASSIGN_Exp_Setup($$,$1,$2,new_var);
    emitCommand(new_var,"declaring var "+ $1->type+" "+$2->name+" with register "+$$->llvm_register );

    if($4->type == "BYTE" && $1->type == "INT"){
        string zext_reg = freshRegister();

        emitCommand(zext_reg + " = zext i8 " + $4->llvm_register + " to i32","aid_func_Type_ID_ASSIGN_Exp_SC func");
        init_val = "store i32 "+zext_reg+", "+default_val_type+"* "+$$->llvm_register;

        emitCommand(init_val,"primitive init "+$$->llvm_register+" from reg "+reg_from_Exp );
    }else {
//        emitCommand("hi","hello");
        init_val = "store "+default_val_type+" "+reg_from_Exp+", "+default_val_type+"* "+$$->llvm_register;
        emitCommand(init_val,"primitive init "+$$->llvm_register+" from reg "+reg_from_Exp );
    }
    codeBuffer.emit("");
}

void aid_func_ID_ASSIGN_Exp_SC_Setup (Node_Az *$1,Node_Az *$2,Node_Az *$3){

  frameEntry * ptr =  symbols_table->returnEntryByName($1->name);
  $1->type = ptr->type_name;

  string id_llvm_register =  "%"+$1->name +symbols_table->getGlobalName($1->name);
  string default_val_type ;
  string reg_from_Exp = $3->llvm_register;
  string init_val;

    if($1->type == "INT"){
        default_val_type = "i32";
    } else if($1->type == "BYTE"){
        default_val_type = "i8";
    } else if ($1->type == "BOOL"){
        default_val_type = "i1";
    }else{
        default_val_type = "i32";
    }

        if($3->type == "BYTE" && $1->type == "INT"){
        string zext_reg = freshRegister();
        codeBuffer.emit(zext_reg + " = zext i8 " + $3->llvm_register + " to i32");

        init_val = "store i32 "+zext_reg+", "+default_val_type+"* "+id_llvm_register;
        emitCommand(init_val,"primitive init "+id_llvm_register+" from reg "+reg_from_Exp );
        } else {
            init_val = "store "+default_val_type+" "+reg_from_Exp+", "+default_val_type+"* "+id_llvm_register;
            emitCommand(init_val,"primitive init "+id_llvm_register+" from reg "+reg_from_Exp );
        }
    codeBuffer.emit("");
}

void aid_id_func_ID_ASSIGN_Exp_SC_Setup_ENUM(Node_Az *$1, Node_Az *$2, Node_Az *$3){


    string id_llvm_register =  "%"+$1->name +symbols_table->getGlobalName($1->name);
    string default_val_type ;
    string reg_from_Exp = $3->llvm_register;
    string init_val;

    default_val_type = "i32";//the enum type is i32;

    $1->type = default_val_type;

    init_val = "store "+default_val_type+" "+reg_from_Exp+", "+default_val_type+"* "+id_llvm_register;

    emitCommand(""," -------------------  in aid_id_func_ID_ASSIGN_Exp_SC_Setup_enum");
    codeBuffer.emit("");

    emitCommand(init_val,"primitive init "+id_llvm_register+" from reg "+reg_from_Exp );
    codeBuffer.emit("");
    emitCommand(""," ------------------- END in aid_id_func_ID_ASSIGN_Exp_SC_Setup_enum");

}

string Init_name_id_to_llvm_register(Node_Az *$2){
    frameEntry * ptr_variable = symbols_table->returnEntryByName($2->name);
    string fresh_id_num = freshGlobalNumber();
    stringstream llvm_register_variable_name;

    llvm_register_variable_name << "%"+$2->name << fresh_id_num;
    symbols_table->setGlobalNameForEntry($2->name, fresh_id_num);

    return llvm_register_variable_name.str();
}

string Init_name_param_to_llvm_register(string param_name){
//    frameEntry * ptr_variable = symbols_table->returnEntryByName($2->name);
    string fresh_id_num = freshGlobalNumber();
    stringstream llvm_register_variable_name;

    llvm_register_variable_name << "%"+param_name << fresh_id_num;
    symbols_table->setGlobalNameForEntry(param_name, fresh_id_num);

    return llvm_register_variable_name.str();
}

void emit_ret_void(){
    emitCommand("ret void","RETURN SC Rule");
}

string TypeToLLVM(string type){
    if(type == "i1" || type == "i32" || type == "i8" || type == "void") return type;

    if(type == "VOID"){
        return "void";
    }
    if(type == "INT"){
        return "i32";
    } else if(type == "BYTE"){
        return "i8";
    } else if(type == "BOOL"){
        return "i1";
    } else {
        return "i32";
    }
}

void emit_dummy_ret_type(string ret_type_llvm ){
    codeBuffer.emit("");

//    emitCommand("","This is ret_type_llvm : "+ret_type_llvm);

    ret_type_llvm = TypeToLLVM(ret_type_llvm);
    string ret;
    if(ret_type_llvm != "void"){
        ret = "%..res = add " + ret_type_llvm + " 0, 0\n" +
              +"ret " + ret_type_llvm + " %..res";
    } else {
        ret ="ret void";
    }


    emitCommand(ret,"Dummy RETURN ");
}



void emit_ret_Type(string ret_type, string ptr_type , string reg_llvm_ptr_load_res_from){
    emitCommand("","this is the the register:"+reg_llvm_ptr_load_res_from);

//    emitCommand("","this is the $2->type:"+ptr_type);
//    emitCommand("","this is the  func type:"+ret_type);


    ret_type = TypeToLLVM(ret_type);
    ptr_type = TypeToLLVM(ptr_type);
    emitCommand("","this is the $2->type:"+ptr_type);
    emitCommand("","this is the  func type:"+ret_type);

    string fresh_num = freshGlobalNumber();
    string reg_zext = freshRegister();
    if(ret_type == "i32" && ptr_type == "i8"){
        emitCommand(reg_zext+" = zext i8 "+reg_llvm_ptr_load_res_from+" to i32","emit_ret_Type -- i8 to i32");
        reg_llvm_ptr_load_res_from = reg_zext;
    }

    string ret = "%...res"+fresh_num+"= add "+ret_type+" "+reg_llvm_ptr_load_res_from+", 0\n"+
//                "%res = load " + ret_type + ", " + ptr_type + "* " + reg_llvm_ptr_load_res_from +"\n"
                                         //%b = load i32, i32* %a
                +"ret "+ret_type+" %...res"+fresh_num;
    emitCommand(ret,"RETURN Exp SC");
}
/*

void emit_safe_alloc_Type_ID_SC_Setup(Node_Az * $$ , Node_Az *$1, Node_Az *$2, string new_var){

    string initNumber_0 = freshGlobalInitNumber();
    string initNumber_1 = freshGlobalInitNumber();
    codeBuffer.emit("");
    emitCommand("","|||||||||||||||||||||||||||||||||||||||||| start init var"+ $1->type+" "+$2->name+" |||||||");
    codeBuffer.emit("");
    codeBuffer.emitGlobal("@init_local_var"+initNumber_0+" = global i1 1, align 4");
    emitCommand("%..t" + initNumber_0 + " = load i1, i1* @init_local_var"+initNumber_0, "|||||||");
    emitCommand("%..t" + initNumber_1 + " = icmp eq i1 %..t" + initNumber_0 +", 1", "|||||||");
    int br_init =  emitCommand("br i1 %..t" + initNumber_1 +", label @, label @","|||||||");
    string init_start_label = codeBuffer.genLabel();
    codeBuffer.emit("");
    // ------------------------------ the alloc of the memeber! ------------------------------

    emitCommand(new_var,"declaring var "+ $1->type+" "+$2->name+" with register "+$$->llvm_register );

    // ------------------------------ the alloc of the memeber! ------------------------------
    codeBuffer.emit("");
    emitCommand("store i1 0, i1*  @init_local_var"+initNumber_0+", align 4","|||||||");
    string init_end_br_label = "br label @";
    int loc_init_else_label = codeBuffer.emit(init_end_br_label);
    string init_end_label = codeBuffer.genLabel();
    codeBuffer.bpatch(codeBuffer.makelist({loc_init_else_label, FIRST}), init_end_label);
    codeBuffer.emit("");
    codeBuffer.bpatch(codeBuffer.makelist({br_init,SECOND}), init_end_label);
    codeBuffer.bpatch(codeBuffer.makelist({br_init,FIRST}), init_start_label);
    emitCommand("", "|||||||||||||||||||||||||||||||||||||||||| end init var "+ $1->type+" "+$2->name+" |||||||");
    codeBuffer.emit("");





}

void emit_safe_alloc_Type_ID_ASSIGN_Exp_Setup(Node_Az * $$ , Node_Az *$1, Node_Az *$2, string new_var){

    string initNumber_0 = freshGlobalInitNumber();
    string initNumber_1 = freshGlobalInitNumber();
    codeBuffer.emit("");
    emitCommand("","|||||||||||||||||||||||||||||||||||||||||| start init var"+ $1->type+" "+$2->name+" |||||||");
    codeBuffer.emit("");
    codeBuffer.emitGlobal("@init_local_var"+initNumber_0+" = global i1 1, align 4");
    emitCommand("%..t" + initNumber_0 + " = load i1, i1* @init_local_var"+initNumber_0, "|||||||");
    emitCommand("%..t" + initNumber_1 + " = icmp eq i1 %..t" + initNumber_0 +", 1", "|||||||");
    int br_init =  emitCommand("br i1 %..t" + initNumber_1 +", label @, label @","|||||||");
    string init_start_label = codeBuffer.genLabel();
    codeBuffer.emit("");
    // ------------------------------ the alloc of the memeber! ------------------------------

    emitCommand(new_var,"declaring var "+ $1->type+" "+$2->name+" with register "+$$->llvm_register );

    // ------------------------------ the alloc of the memeber! ------------------------------
    codeBuffer.emit("");
    emitCommand("store i1 0, i1*  @init_local_var"+initNumber_0+", align 4","|||||||");
    string init_end_br_label = "br label @";
    int loc_init_else_label = codeBuffer.emit(init_end_br_label);
    string init_end_label = codeBuffer.genLabel();
    codeBuffer.bpatch(codeBuffer.makelist({loc_init_else_label, FIRST}), init_end_label);
    codeBuffer.emit("");
    codeBuffer.bpatch(codeBuffer.makelist({br_init,SECOND}), init_end_label);
    codeBuffer.bpatch(codeBuffer.makelist({br_init,FIRST}), init_start_label);
    emitCommand("", "|||||||||||||||||||||||||||||||||||||||||| end init var "+ $1->type+" "+$2->name+" |||||||");
    codeBuffer.emit("");
}
*/

void add_$op_list_to_$$_list(Node_Az * $$, Node_Az * $op,  ListLabelType list_type ){
    if(list_type == TRUE_LIST){
        $$->true_list = codeBuffer.merge($$->true_list,$op->true_list);
    } else if(list_type == FALSE_LIST){
        $$->false_list = codeBuffer.merge($$->false_list,$op->false_list);
    } else if(list_type == BREAK_LIST){
        $$->break_list = codeBuffer.merge($$->break_list,$op->break_list);

    } else if(list_type == CONTINUE_LIST){
        $$->continue_list = codeBuffer.merge($$->continue_list,$op->continue_list);
    } else if(list_type == END_IF_LIST){
        $$->end_if_list = codeBuffer.merge($$->end_if_list,$op->end_if_list);
    }
}

void add_all_$op_lists_to_$$_list(Node_Az * $$, Node_Az * $op){
    add_$op_list_to_$$_list($$,$op,TRUE_LIST);
    add_$op_list_to_$$_list($$,$op,FALSE_LIST);
    add_$op_list_to_$$_list($$,$op,BREAK_LIST);
    add_$op_list_to_$$_list($$,$op,CONTINUE_LIST);
    add_$op_list_to_$$_list($$,$op,END_IF_LIST);
}


void add_to_$$_label_list(Node_Az * $$, Node_Az * $op, int loc_br , BranchLabelIndex branchLabelIndex, ListLabelType list_type ){
    pair<int,BranchLabelIndex> *p =  new pair<int, BranchLabelIndex>({loc_br,branchLabelIndex});;
    if(list_type == TRUE_LIST){
//        p = new pair<int, BranchLabelIndex>({loc_br,branchLabelIndex});
        $$->true_list = codeBuffer.merge(codeBuffer.merge($op->true_list,codeBuffer.makelist(p)), $$->true_list);
    } else if(list_type == FALSE_LIST){
//        p = new pair<int, BranchLabelIndex>({loc_br,branchLabelIndex});
        $$->false_list = codeBuffer.merge(codeBuffer.merge($op->false_list,codeBuffer.makelist(p)),$$->false_list);
    } else if(list_type == BREAK_LIST){

        $$->break_list = codeBuffer.merge(codeBuffer.merge($op->break_list,codeBuffer.makelist(p)),$$->break_list);

    } else if(list_type == CONTINUE_LIST){

        $$->continue_list = codeBuffer.merge(codeBuffer.merge($op->continue_list,codeBuffer.makelist(p)),$$->continue_list);
    } else if(list_type == END_IF_LIST){

        $$->end_if_list = codeBuffer.merge(codeBuffer.merge($op->end_if_list,codeBuffer.makelist(p)),$$->end_if_list);
    }
}

string RetTypeToLLVM(string retType){
    if(retType == "INT" || retType == "BYTE" || retType == "BOOL"){
        return TypeToLLVM(retType);
    }else if(retType == "VOID"){
        return "void";
    } else {
        return "i32";
    }
}

string getRegisterOfVariableFromTable(string var_name){

    frameEntry * ptr =  symbols_table->returnEntryByName(var_name);
    if(ptr == nullptr){
        return " Not in Symbols Table! ";
    }
    else return "%"+var_name+ptr->globalNum;
}

string is_var(string var_name){
    frameEntry * ptr =  symbols_table->returnEntryByName(var_name);
    if(ptr == nullptr){
        return "false";
    }
    else return "true";
}


string emit_for_function_call_and_return_list_registers(Node_Az * $$){

    if($$->params_types_or_enum_values.size() == 0) return "";

    std::stringstream size;
    std::stringstream list;

    string register_ptr_variable_in_table ;
    string func_name = $$->name;

    std::stringstream load_cmd;

    codeBuffer.emit("");

    int s = $$->params_types_or_enum_values.size();
    string temp_register_for_load;
    int i = s-1;
    for(; i > 0 ;i--){
        emitCommand("","This is the register of the exp: : "+ $$->list_of_registers[i]);
        emitCommand("","This is the parameter of name the exp:  "+ $$->params_names[i]);
        emitCommand("","This is the register of the variable: "+getRegisterOfVariableFromTable($$->params_types_or_enum_values[i]));
        emitCommand("","This is the type of the register : "+TypeToLLVM($$->params_types_or_enum_values[i]));
        emitCommand("","");
        emitCommand("","is it a var? -"+is_var($$->params_types_or_enum_values[i]));
        list << TypeToLLVM($$->params_types_or_enum_values[i]) <<" " << $$->list_of_registers[i] ;
        list <<", ";
        emitCommand("","");

    }
    list << TypeToLLVM($$->params_types_or_enum_values[i]) <<" " << $$->list_of_registers[i] ;

    emitCommand("","This is the register of the exp: : "+ $$->list_of_registers[i]);
    emitCommand("","This is the parameter of name the exp:  "+ $$->params_names[i]);
    emitCommand("","This is the register of the variable: "+getRegisterOfVariableFromTable($$->params_types_or_enum_values[i]));
    emitCommand("","This is the type of the register : "+TypeToLLVM($$->params_types_or_enum_values[i]));
    emitCommand("","");
    emitCommand("","is it a var? -"+is_var($$->params_types_or_enum_values[i]));
    emitCommand("","");

    codeBuffer.emit("");


    return list.str();
}

string emit_for_function_call_and_return_list_registersA(Node_Az * $$, vector<string> exp_params){

    if($$->params_types_or_enum_values.size() == 0) return "";

    std::stringstream size;
    std::stringstream list;

    string register_ptr_variable_in_table ;
    string func_name = $$->name;

    std::stringstream load_cmd;

    codeBuffer.emit("");

    int s = $$->params_types_or_enum_values.size();
    string temp_register_for_load;
    int i = s-1;
    for(int j = 0; i >= 0, j < s ;i--, j++){
        emitCommand("","This is the register of the exp: : "+ $$->list_of_registers[i]);
        emitCommand("","This is the parameter of name the exp:  "+ $$->params_names[i]);
        emitCommand("","This is the register of the variable: "+getRegisterOfVariableFromTable($$->params_types_or_enum_values[i]));
        emitCommand("","This is the type of the register : "+TypeToLLVM($$->params_types_or_enum_values[i]));
        emitCommand("","");
        emitCommand("","is it a var? -"+is_var($$->params_types_or_enum_values[i]));
        //cout << "wow_before: " << TypeToLLVM(exp_params[j]) << "\n";
        if(TypeToLLVM($$->params_types_or_enum_values[i]) == "i8" && TypeToLLVM(exp_params[j]) == "i32"){
            string fresh_r = freshRegister();
            //cout << "wow_after: " << TypeToLLVM(exp_params[i]) << "\n";
            codeBuffer.emit(fresh_r + " = zext i8 " + $$->list_of_registers[i] + " to i32");
            list << "i32" << " " << fresh_r ;
        }else{
            list << TypeToLLVM($$->params_types_or_enum_values[i]) <<" " << $$->list_of_registers[i];
        }
        if(i != 0){
            list <<", ";
        }
        emitCommand("","");
    }
    /*
    list << TypeToLLVM($$->params_types_or_enum_values[i]) <<" " << $$->list_of_registers[i] ;

    emitCommand("","This is the register of the exp: : "+ $$->list_of_registers[i]);
    emitCommand("","This is the parameter of name the exp:  "+ $$->params_names[i]);
    emitCommand("","This is the register of the variable: "+getRegisterOfVariableFromTable($$->params_types_or_enum_values[i]));
    emitCommand("","This is the type of the register : "+TypeToLLVM($$->params_types_or_enum_values[i]));
    emitCommand("","");
    emitCommand("","is it a var? -"+is_var($$->params_types_or_enum_values[i]));
    emitCommand("","");
    */
    codeBuffer.emit("");


    return list.str();
}

void emit_input_parameters(string func_name){
    string emit_command="";

    stringstream alloc_command ;
    stringstream store_command ;
    vector<string> param_types = symbols_table->get_func_parameters_type_in_reverse_order(func_name);

    string new_var;
    string param_name;
    string llvm_type;
    int i_reverse = param_types.size() - 1;

    for(int i = 0 ; i < param_types.size(); i_reverse-- ,i++){
        param_name = symbols_table->getParamName(i);
        llvm_type = TypeToLLVM(param_types[i]);
        emitCommand("","This is the param type: "+(llvm_type)+ " name: "+param_name);
        new_var = Init_name_param_to_llvm_register(param_name);
        alloc_command << new_var << " = alloca " << llvm_type <<"" << endl;
        store_command << "store "<<llvm_type<<" %"<<i<<", "<<llvm_type <<"* "<<new_var <<endl;
    }
    emit_command += alloc_command.str();
    emit_command += store_command.str();

    emitCommand(emit_command,"---------------------- alloc all  parameters ----------------------");
}

#endif //INC_236360_HW5_LLVM_ASSEMBLY_HPP
