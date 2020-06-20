//
// Created by hilak on 1/12/2020.
//
//#include "aidFunction.hpp"

#ifndef INC_236360_HW5_PARSER_HW5_HPP
#define INC_236360_HW5_PARSER_HW5_HPP

#include <sstream>
#include <string>
#include <iomanip>

#include <iostream>
#include <string>
#include "bp.hpp"
#include "Table.hpp"
#include "parser_actions.h"
#include "hw3_output.hpp"
#include "yystype.hpp"
#include "LLVM_Assembly.hpp"
//#include "aidFunction.hpp"

using namespace std;
using namespace output;
extern CodeBuffer& codeBuffer;
extern Table * symbols_table;

extern vector<string> stam;//just a vector for inputing var
extern vector<int> stam_lineno;//just a vector for inputing var
extern vector<E_TYPE> empty;
extern int while_ind ;
extern string current_func ;




void initProgram(){

    symbols_table = InitNewTable();

    //emiting to global buffer:
    //emit__print_zeroConstant();
    emit_printConstant();
    emit_printIConstant();
    emit_func_declartion("declare void @exit(i32) ");
    emit_func_declartion("declare i32 @printf(i8*, ...)");
    emit_func_zero_exit();

    //emiting to code buffer:
    codeBuffer.emit("");
    codeBuffer.emit("");
    emit_printi_function_buffer();
    emit_print_function_buffer();
    codeBuffer.emit("");


}

void endProgram(){
    codeBuffer.emitGlobal("\n \n; -----------------------  Program ------------------------ ");

    codeBuffer.printGlobalBuffer();


//    codeBuffer.printGlobalBuffer();

    codeBuffer.printCodeBuffer();
    closeTableAndVerifyMain(symbols_table);
    DeleteTable(&symbols_table);
}

int CharPointerToInt_2(const char *str) {
        int i = 0;
        int val = 0;
        while (str[i] != '\0') {
            val *= 10;
            val += (int) (str[i] - '0');
            i++;
        }
        return val;
    }

// ------------------------ FuncDecl Rules Setup ------------------------

void RetType_ID_LPAREN_Formals_RPAREN(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3, Node_Az *$4, Node_Az *$5){
    InsertEntryToCurrentFrame(symbols_table, $1->type, $2->name,$4->params_types_or_enum_values,FUNC,$2->lineno);
    current_func = $2->name;
    }

void RetType_ID_LPAREN_Formals_RPAREN_LBRACErBRACE_LBRACE(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3,Node_Az *$4, Node_Az * $5, Node_Az * $6, Node_Az * $7){
    PushNewScope(symbols_table);

    PushParams(symbols_table,$4->params_types_or_enum_values,$4->params_names,$4->params_lineno,$4->etypes);
    string func_name = $2->name;
    frameEntry * ptr = symbols_table->returnEntryByName(func_name);

    string llvm_retType = RetTypeToLLVM(ptr->type_name);
    string llvm_parameter_list =  symbols_table->ParameterListToFuncDecLLVM(func_name);

    string func_define = "define "+llvm_retType+" @"+func_name+"("+llvm_parameter_list+") {\n"
                                                   ""+func_name+"_entry:";
    codeBuffer.emit(func_define);
    emit_input_parameters(func_name);
    codeBuffer.emit("%fp = alloca [50 x i32]");

}

void RetType_ID_LPAREN_Formals_RPAREN_LBRACErBRACE_LBRACE_LBRACErBRACE_Statements_RBRACE(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3,Node_Az *$4, Node_Az * $5, Node_Az * $6, Node_Az * $7, Node_Az * $8, Node_Az * $9){


    PopCurrentScope(symbols_table);

    emit_dummy_ret_type($1->type);
    codeBuffer.emit("}\n \n");
}

// ------------------------ Formals Rules Setup ------------------------

void Formals_epslion_Setup(Node_Az * $$){
    //  output::printProductionRule(10);
    $$->params_types_or_enum_values = stam;
    $$->params_names = stam;
    $$->params_lineno = stam_lineno;
    $$->etypes = empty;
    }

void FormalsList_Setup(Node_Az * $$ , Node_Az *$1){
    // output::printProductionRule(11);
    $$->params_types_or_enum_values = $1->params_types_or_enum_values;
    $$->params_names = $1->params_names;
    $$->params_lineno = $1->params_lineno;
    $$->etypes = $1->etypes;



    }

// ------------------------ FormalsList Rules Setup ------------------------

void FormalDecl_Setup(Node_Az * $$ , Node_Az *$1){
    //	output::printProductionRule(12);
    if( symbols_table->isEntryExists($1->name)){
        errorDef($1->lineno,$1->name);
    }
    $$->params_types_or_enum_values.push_back($1->type);
    $$->params_names.push_back($1->name);
    $$->params_lineno.push_back($1->lineno);
    $$->etypes.push_back($1->eType);
    $$->lineno = $1->lineno;
    }

void FormalDecl_COMMA_FormalsList_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3){
    if( symbols_table->isEntryExists($1->name)){
        errorDef($1->lineno,$1->name);
    }

    $$->params_types_or_enum_values = $3->params_types_or_enum_values;
    $$->params_names = $3->params_names;
    $$->params_lineno = $3->params_lineno;
    $$->etypes = $3->etypes;

    $$->params_types_or_enum_values.push_back($1->type);
    $$->params_names.push_back($1->name);
    $$->params_lineno.push_back($1->lineno);
    $$->etypes.push_back($1->eType);
    //	output::printProductionRule(13);
    }

// ------------------------ FormalDecl Rules Setup ------------------------
void Type_ID_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2){
    // output::printProductionRule(14);
    $$->type = $1->type;
    $$->name = $2->name;
    $$->lineno = $2->lineno;
    $$->eType = VAR;
    }

void EnumType_ID_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2){
    checkEnumTypeUndef(symbols_table,  $1->type, $1->lineno);//checks if enum is in the any of the scopes.
    $$->type =  $1->type;
    $$->name = $2->name;
    $$->lineno = $1->lineno;
    $$->eType = ENUM_PARAM;
    //   output::printProductionRule(15);
    }


// ------------------------ Enumerator Rules Setup ------------------------

void Enumerator_Setup(Node_Az * $$ , Node_Az *$1){
    //output::printProductionRule(16);
    $1->params_types_or_enum_values.push_back($1->name);
    $1->params_names.push_back($1->name);
    $1->params_lineno.push_back($1->lineno);

    $$->params_types_or_enum_values = $1->params_types_or_enum_values;
    $$->params_names = $1->params_names;
    $$->params_lineno = $1->params_lineno;
    $$->lineno = $1->lineno;
    }

void EnumeratorList_COMMA_Enumerator_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3){
    $$->params_types_or_enum_values = $1->params_types_or_enum_values;
    $$->params_names = $1->params_names;
    $$->params_lineno = $1->params_lineno;

    $$->params_types_or_enum_values.push_back($3->name);
    $$->params_names.push_back($3->name);
    $$->params_lineno.push_back($3->lineno);
    //      output::printProductionRule(17);
    }

void Enumerator_ID_Setup(Node_Az * $$ , Node_Az *$1){
    $$->type = $1->name;//Enumerator doesn't have a type.
    $$->name = $1->name;
    $$->lineno = $1->lineno;
    // output::printProductionRule(18);
    }

// ------------------------ Statements Rules Setup ------------------------
void Statement_Setup(Node_Az * $$ , Node_Az *$1){
    add_all_$op_lists_to_$$_list($$,$1);
//    $$->break_list = codeBuffer.merge($$->break_list,$1->break_list);
}

void Statements_Statement_Setup(Node_Az * $$ , Node_Az *$1, Node_Az *$2){
    add_all_$op_lists_to_$$_list($$,$1);
    add_all_$op_lists_to_$$_list($$,$2);
}

// ------------------------ IfRule Rules Setup ------------------------

void IF_LPAREN_Exp_RPAREN_Setup(Node_Az * $$ , Node_Az *$1, Node_Az *$2,Node_Az *$3){
    PushNewScope(symbols_table);//push for if token.
    checkBool($3->type,$3->lineno);//for Exp in if(EXP)
    string exp_bool_register = $3->llvm_register;

    string check_exp = "br i1 "+exp_bool_register+", label @, label @";
    int loc_br_if = emitCommand(check_exp,"IF_LPAREN_Exp_RPAREN_Setup rule");

    string dummy_br_true_label = "br label @";
    int loc_dummy_true_label = emitCommand(dummy_br_true_label, "just dummy label before the true label of if");

    add_to_$$_label_list($$, $3, loc_br_if, FIRST, TRUE_LIST);
    add_to_$$_label_list($$, $3, loc_dummy_true_label, FIRST, TRUE_LIST);

    add_to_$$_label_list($$, $3, loc_br_if, SECOND, FALSE_LIST);
    string label_if_true = codeBuffer.genLabel();
    codeBuffer.bpatch($$->true_list,label_if_true);
}



// ------------------------ Statement Rules Setup ------------------------
void LBRACE_LBRACErBRACE_Statements_RBRACE(Node_Az * $$ , Node_Az *$1,Node_Az *$2, Node_Az *$3, Node_Az *$4){
    PopCurrentScope(symbols_table);
    add_all_$op_lists_to_$$_list($$,$3);
    add_all_$op_lists_to_$$_list($$,$2);
    add_all_$op_lists_to_$$_list($$,$4);
//    emitCommand("","2222");
}

void Type_ID_SC_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2){
    InsertEntryToCurrentFrame(symbols_table,$1->type,$2->name, stam,VAR,$2->lineno);

    int offset_to_fp = symbols_table->returnEntryByName($2->name)->offset;
    std::stringstream off;
    off << offset_to_fp;
    string offset_relative_to_fp = off.str();
    string fresh_reg = freshRegister();
    codeBuffer.emit(fresh_reg + " = getelementptr [50 x i32], [50 x i32]* %fp, i32 0, i32 " + offset_relative_to_fp);
    codeBuffer.emit("store i32 0, i32* " + fresh_reg);
    $$->llvm_register = Init_name_id_to_llvm_register($2);
    /*
    string new_var = $$->llvm_register;
    string default_val_type ;
    string default_val = "0";
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

    init_val = "store "+default_val_type+" "+default_val+", "+default_val_type+"* "+$$->llvm_register;


//    emit_safe_alloc_Type_ID_SC_Setup($$, $1, $2, new_var);
    emitCommand(new_var,"declaring var "+ $1->type+" "+$2->name+" with register "+$$->llvm_register );

    emitCommand(init_val,"primitive init "+$$->llvm_register+" with literal "+default_val );
     */

}

void Type_ID_ASSIGN_Exp_SC(Node_Az * $$ ,Node_Az *$1,Node_Az *$2,Node_Az *$3,Node_Az *$4){
    if( symbols_table->isEntryExists($2->name)){
        errorDef($2->lineno,$2->name);
    }
    InsertEntryToCurrentFrame(symbols_table,$1->type,$2->name, stam,VAR,$2->lineno);
    checkAssign(symbols_table, $1->type, $4->type ,$4->lineno);

    int offset_to_fp = symbols_table->returnEntryByName($2->name)->offset;
    std::stringstream off;
    off << offset_to_fp;
    string offset_relative_to_fp = off.str();
    string fresh_reg = freshRegister();
    codeBuffer.emit(fresh_reg + " = getelementptr [50 x i32], [50 x i32]* %fp, i32 0, i32 " + offset_relative_to_fp);
    if($4->type == "BYTE"){
        string zext_reg = freshRegister();
        emitCommand(zext_reg + " = zext i8 " + $4->llvm_register + " to i32","aid_func_Type_ID_ASSIGN_Exp_SC func");
        codeBuffer.emit("store i32 " + zext_reg + ", i32* " + fresh_reg);
    }else if($4->type == "BOOL"){
        string zext_reg = freshRegister();
        emitCommand(zext_reg + " = zext i1 " + $4->llvm_register + " to i32","aid_func_Type_ID_ASSIGN_Exp_SC func");
        codeBuffer.emit("store i32 " + zext_reg + ", i32* " + fresh_reg);
    }else{
        codeBuffer.emit("store i32 " + $4->llvm_register + ", i32* " + fresh_reg);
    }


    /*
    $$->llvm_register = Init_name_id_to_llvm_register($2);
    $1->llvm_register = $$->llvm_register;

    aid_func_Type_ID_ASSIGN_Exp_SC($$,$1,$2,$3,$4);
    */
}

void EnumType_ID_SC_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2){
    checkEnumTypeUndef(symbols_table, $1->type, $1->lineno);
    InsertEntryToCurrentFrame(symbols_table,$1->type,$2->name, stam,ENUM_VAR,$2->lineno);

    int offset_to_fp = symbols_table->returnEntryByName($2->name)->offset;
    std::stringstream off;
    off << offset_to_fp;
    string offset_relative_to_fp = off.str();
    string fresh_reg = freshRegister();
    codeBuffer.emit(fresh_reg + " = getelementptr [50 x i32], [50 x i32]* %fp, i32 0, i32 " + offset_relative_to_fp);
    codeBuffer.emit("store i32 0, i32* " + fresh_reg);
    /*
    $$->llvm_register = Init_name_id_to_llvm_register($2);

    $$->llvm_register = Init_name_id_to_llvm_register($2);
    $$->llvm_type = "i32";

    string new_var = $$->llvm_register;
    string default_val = "0";
    string init_val;

    new_var += " = alloca i32";

//    emit_safe_alloc_Type_ID_SC_Setup($$, $1, $2 , new_var);
    emitCommand(new_var,"declaring var "+ $1->type+" "+$2->name+" with register "+$$->llvm_register );

    init_val = "store i32 "+default_val+", i32* "+$$->llvm_register;
    emitCommand(init_val,"primitive init "+$$->llvm_register+" with literal "+default_val );
     */
    codeBuffer.emit("");

//    emit_and_print_variable("i32",$$->llvm_register);
}

void EnumTypeIDAssign_Exp_SC(Node_Az * $$ ,Node_Az *$1,Node_Az *$2){
    checkEnumAssign(symbols_table, $1->type,$1->name, $2->type,$2->lineno);

    //cout << "look offset: " << symbols_table->returnEntryByName($1->name)->offset << "\n";
    //cout << "look name: " << $1->name << "\n";
    int offset_to_fp = symbols_table->returnEntryByName($1->name)->offset;
    std::stringstream off;
    off << offset_to_fp;
    string offset_relative_to_fp = off.str();
    string fresh_reg = freshRegister();
    codeBuffer.emit(fresh_reg + " = getelementptr [50 x i32], [50 x i32]* %fp, i32 0, i32 " + offset_relative_to_fp);
    codeBuffer.emit("store i32 " + $2->llvm_register + ", i32* " + fresh_reg);


    /*
    $$->llvm_register = Init_name_id_to_llvm_register($1);
    string new_var = $$->llvm_register;
    string init_val;
    string default_val = $2->llvm_register;
    new_var += " = alloca i32";

//    emit_safe_alloc_Type_ID_SC_Setup($$, $1, $2 , new_var);
    emitCommand(new_var,"declaring var "+ $1->type+" "+$2->name+" with register "+$$->llvm_register );

    init_val = "store i32 "+default_val+", i32* "+$$->llvm_register;
    emitCommand(init_val,"primitive init "+$$->llvm_register+" with exp" );
    */
    codeBuffer.emit("");

//    emit_and_print_variable("i32",$$->llvm_register);

}

void ID_ASSIGN_Exp_SC_Setup(Node_Az *$1,Node_Az *$2,Node_Az *$3){
    checkIdExist(symbols_table, $1->name, $1->lineno);
    //cout << $1.type << "\n";
    if(symbols_table->getEntryEType($1->name) == ENUM_VAR){
        checkEnumAssign(symbols_table,symbols_table->getEntryTypeName($1->name),$1->name, $3->type,$1->lineno);

        //cout << "look offset: " << symbols_table->returnEntryByName($1->name)->offset << "\n";
        //cout << "look name: " << $1->name << "\n";
        int offset_to_fp = symbols_table->returnEntryByName($1->name)->offset;
        std::stringstream off;
        off << offset_to_fp;
        string offset_relative_to_fp = off.str();
        string fresh_reg = freshRegister();
        codeBuffer.emit(fresh_reg + " = getelementptr [50 x i32], [50 x i32]* %fp, i32 0, i32 " + offset_relative_to_fp);
        codeBuffer.emit("store i32 " + $3->llvm_register + ", i32* " + fresh_reg);

       // aid_id_func_ID_ASSIGN_Exp_SC_Setup_ENUM($1, $2, $3);
    } else if(symbols_table->getEntryEType($1->name) == ENUM_PARAM || symbols_table->getEntryEType($1->name) == PARAM){
        checkAssignID(symbols_table, $1->name, $3->type ,$3->lineno);
        aid_func_ID_ASSIGN_Exp_SC_Setup($1,$2,$3);
    }else{
        checkAssignID(symbols_table, $1->name, $3->type ,$3->lineno);
        int offset_to_fp = symbols_table->returnEntryByName($1->name)->offset;
        std::stringstream off;
        off << offset_to_fp;
        string offset_relative_to_fp = off.str();
        string fresh_reg = freshRegister();
        codeBuffer.emit(fresh_reg + " = getelementptr [50 x i32], [50 x i32]* %fp, i32 0, i32 " + offset_relative_to_fp);
        frameEntry* entry_ptr =  symbols_table->returnEntryByName($1->name);
        if($3->type == "INT"){
            codeBuffer.emit("store i32 " + $3->llvm_register + ", i32* " + fresh_reg);
        }else if($3->type == "BYTE"){
            string zext_reg = freshRegister();
            codeBuffer.emit(zext_reg + " = zext i8 " + $3->llvm_register + " to i32");
            codeBuffer.emit("store i32 " + zext_reg + ", i32* " + fresh_reg);
        }else if($3->type == "BOOL"){
            string zext_reg = freshRegister();
            codeBuffer.emit(zext_reg + " = zext i1 " + $3->llvm_register + " to i32");
            codeBuffer.emit("store i32 " + zext_reg + ", i32* " + fresh_reg);
        }
    }
}

void Call_SC(Node_Az * $$ ,Node_Az *$1,Node_Az *$2){
    $$->name = $1->name;

    $$->list_of_registers = $1->list_of_registers;
    $$->params_names = $1->params_names;
    $$->params_types_or_enum_values = $1->params_types_or_enum_values;
    $$->type = $1->type;

}

void RETURN_Exp_SC_Setup(Node_Az *$1,Node_Az *$2){
    if(symbols_table->returnEntryByName(current_func)->type_name == "VOID"){
        errorMismatch($1->lineno);
    }
    verifyFuncReturn(symbols_table,current_func ,$2->type, $1->lineno);

    string ret_type_func = symbols_table->returnEntryByName(current_func)->type_name;

    string llvm_exp_reg = $2->llvm_register;
//    emitCommand("","this is the $2->type:"+$2->llvm_type);
//    emitCommand("","this is the  func type:"+ret_type_func);
    emit_ret_Type(ret_type_func,$2->llvm_type,llvm_exp_reg);
}

void IfRule_Statement_prec_SS_Setup(Node_Az * $$ , Node_Az *$1, Node_Az *$2){
    PopCurrentScope(symbols_table);//pop for if token.

    string dummy_br_false_label = "br label @";

    int loc_dummy_false_label = emitCommand(dummy_br_false_label, "just dummy label before the false label of if");
    string dummy_false_label = codeBuffer.genLabel();
    codeBuffer.emit("");//space in buffer


    add_to_$$_label_list($$, $2, loc_dummy_false_label, FIRST, FALSE_LIST);
    codeBuffer.bpatch($$->false_list,dummy_false_label);

    add_to_$$_label_list($$, $2, loc_dummy_false_label, FIRST, BREAK_LIST);
    add_to_$$_label_list($$, $2, loc_dummy_false_label, FIRST, CONTINUE_LIST);
}

void IfRule_Statement_ELSE_Setup(Node_Az * $$ , Node_Az *$1, Node_Az *$2, Node_Az *$3){
    PopCurrentScope(symbols_table);//pop for if token.
    PushNewScope(symbols_table);//push for else token.

    string end_if_else_br_label = "br label @";
    int loc_end_if_else_label = emitCommand(end_if_else_br_label, "ended satements of  \"if\", br to the end of the statment");
    add_to_$$_label_list($$, $1, loc_end_if_else_label, FIRST, END_IF_LIST);

    //this is for the start of the else
    string dummy_br_else_label = "br label @";
    int loc_dummy_else_label = emitCommand(dummy_br_else_label, "just dummy label before the else label of if");

    string else_label = codeBuffer.genLabel();
    codeBuffer.emit("");//space in buffer


    add_to_$$_label_list($$, $1, loc_dummy_else_label, FIRST, FALSE_LIST);
    add_to_$$_label_list($$, $2, loc_dummy_else_label, FIRST, CONTINUE_LIST);
    add_to_$$_label_list($$, $2, loc_dummy_else_label, FIRST, BREAK_LIST);
    codeBuffer.bpatch($$->false_list, else_label);
}

void IfRule_Statement_ELSE_LBRACErBRACE_Statement_Setup(Node_Az * $$ , Node_Az *$1, Node_Az *$2, Node_Az *$3, Node_Az *$4, Node_Az *$5){
    PopCurrentScope(symbols_table);//pop for else token.

    string dummy_br_end_if_else = "br label @";
    int loc_dummy_br_end_if_else = emitCommand(dummy_br_end_if_else, "just dummy label before the end_if_else label of if");

    add_to_$$_label_list($$, $1, loc_dummy_br_end_if_else, FIRST, END_IF_LIST);

    add_all_$op_lists_to_$$_list($$,$2);
    add_all_$op_lists_to_$$_list($$,$4);

//    add_all_$op_lists_to_$$_list($$, $1, loc_dummy_else_label, FIRST, FALSE_LIST);
    add_all_$op_lists_to_$$_list($$, $5);
//    add_all_$op_lists_to_$$_list($$, $2);


    string end_if_else_label = codeBuffer.genLabel();
    codeBuffer.emit("");//space in buffer
    codeBuffer.bpatch($4->end_if_list, end_if_else_label);
    codeBuffer.bpatch($$->end_if_list, end_if_else_label);

//    string dummy_br_false_label = "br label @";
//
//    int loc_dummy_false_label = emitCommand(dummy_br_false_label, "just dummy label before the false label of if");
//    string dummy_false_label = codeBuffer.genLabel();
//    codeBuffer.emit("");//space in buffer
//
//
//    add_to_$$_label_list($$, $2, loc_dummy_false_label, FIRST, FALSE_LIST);
//    codeBuffer.bpatch($$->false_list,dummy_false_label);
    }

void WHILE_Setup(Node_Az *$$ ,Node_Az *$1){

    string dummy_br_while_start_label = "br label @";
    int loc_dummy_br_while_start_label = emitCommand(dummy_br_while_start_label, "just dummy label before the true label of while, NEXT LABEL IS FOR CONTINUE ");
    $$->start_while_label = codeBuffer.genLabel();

    add_to_$$_label_list($$, $1, loc_dummy_br_while_start_label, FIRST, CONTINUE_LIST);
    codeBuffer.bpatch($$->continue_list, $$->start_while_label);
}

void WHILE_LPAREN_Exp_RPAREN_Setup(Node_Az *$$ ,Node_Az *$1, Node_Az *$2,Node_Az *$3){
    PushNewScope(symbols_table);//push for while token.
    checkBool($3->type,$3->lineno);//for Exp in while(EXP)
    while_ind++;


    /// ----- while hw5 -----

        string exp_bool_register = $3->llvm_register;

        string dummy_br_while_start_label = "br label @";

//        int loc_dummy_br_while_start_label = emitCommand(dummy_br_while_start_label, "just dummy label before the true label of while, NEXT LABEL IS FOR CONTINUE ");

        $$->start_while_label = $1->start_while_label;//this is from while marker
        string while_start_label =$$->start_while_label;

//        add_to_$$_label_list($$, $3, loc_dummy_br_while_start_label, FIRST, CONTINUE_LIST);
        codeBuffer.bpatch($$->continue_list, while_start_label);

    emitCommand("","");
        emitCommand(""," ------------------------WHILE_LPAREN_Exp_RPAREN_Setup ------------------------");
    emitCommand("","");

        string check_exp = "br i1 "+exp_bool_register+", label @, label @";
        int loc_br_if = emitCommand(check_exp,"WHILE_LPAREN_Exp_RPAREN_Setup rule");

        string dummy_br_true_label = "br label @";
        int loc_dummy_true_label = emitCommand(dummy_br_true_label, "just dummy label before the true label of while");

        add_to_$$_label_list($$, $3, loc_br_if, FIRST, TRUE_LIST);
        add_to_$$_label_list($$, $3, loc_dummy_true_label, FIRST, TRUE_LIST);

        add_to_$$_label_list($$, $3, loc_br_if, SECOND, FALSE_LIST);
        string label_if_true = codeBuffer.genLabel();
        codeBuffer.bpatch($$->true_list,label_if_true);
    }

void WHILE_LPAREN_Exp_RPAREN_LBRACErBRACE_Statement_Setup(Node_Az *$$ ,Node_Az *$1,Node_Az *$2,Node_Az *$3,Node_Az *$4,Node_Az *$5, Node_Az *$6){
    while_ind--;
    //Statement has an option of {}, or regular single statement.
    PopCurrentScope(symbols_table);//pop for while token.

    string continue_br_label = "br label @";
    int loc_continue_br_label = emitCommand(continue_br_label, "For Continue");
    $$->start_while_label = $5->start_while_label;

    // ------- hw5 while end ------
    string dummy_br_end_while_label = "br label @";

    int loc_dummy_end_false_label = emitCommand(dummy_br_end_while_label, "just dummy label before the end/false label of while, NEXT LABEL IS FOR BREAK");
    string dummy_end_false_label = codeBuffer.genLabel();
    codeBuffer.emit("");//space in buffer

    emitCommand("","");
    emitCommand(""," ------------------------END WHILE_LPAREN_Exp_RPAREN_Setup ------------------------");
    emitCommand("","");

    add_to_$$_label_list($$, $5, loc_dummy_end_false_label, FIRST, FALSE_LIST);
    codeBuffer.bpatch($$->false_list,dummy_end_false_label);

    add_to_$$_label_list($$, $5, loc_dummy_end_false_label, FIRST, BREAK_LIST);
    add_to_$$_label_list($$, $6, loc_dummy_end_false_label, FIRST, BREAK_LIST);//from statements
//    add_all_$op_lists_to_$$_list($$,$5);
//    add_all_$op_lists_to_$$_list($$,$6);
    codeBuffer.bpatch($$->break_list, dummy_end_false_label);
    codeBuffer.bpatch($6->break_list, dummy_end_false_label);

    add_to_$$_label_list($$, $5, loc_continue_br_label, FIRST, CONTINUE_LIST);
    add_to_$$_label_list($$, $6, loc_continue_br_label, FIRST, CONTINUE_LIST);//from statements
    codeBuffer.bpatch($$->continue_list,$$->start_while_label);
}

void BREAK_SC_Setup(Node_Az *$$, Node_Az *$1){
    verifyBreak(while_ind, $1->lineno);

    string break_br_label = "br label @";
    int loc_break_br_label = emitCommand(break_br_label, "BREAK_SC_Setup: For Break ---");
    codeBuffer.emit("");
    add_to_$$_label_list($$,$1,loc_break_br_label,FIRST,BREAK_LIST);
    $$->name = "Break";


}

void CONTINUE_SC_Setup(Node_Az *$$, Node_Az *$1){
    verifyContinue(while_ind, $1->lineno);

    string continue_br_label = "br label @";
    int loc_continue_br_label = emitCommand(continue_br_label, "CONTINUE_SC_Setup: For Continue");
    codeBuffer.emit("");
    add_to_$$_label_list($$,$1,loc_continue_br_label,FIRST,CONTINUE_LIST);

    }



// ------------------------ EnumTypeIDAssign Rules Setup ------------------------

void EnumType_ID_ASSIGN_Setup(Node_Az * $$ , Node_Az *$1, Node_Az *$2){
    checkEnumTypeUndef(symbols_table, $1->type, $1->lineno);
    InsertEntryToCurrentFrame(symbols_table,$1->type,$2->name, stam,ENUM_VAR,$2->lineno);//checks in the func for duplicates
    $$->lineno = $2->lineno;
    $$->type = $1->type;
    $$->name = $2->name;
    }


// ------------------------ Call Rules Setup ------------------------
void ID_LPAREN_ExpList_RPAREN_Setup(Node_Az * $$ , Node_Az *$1, Node_Az *$2,Node_Az *$3){
    verifyFuncDefined(symbols_table,$1->name, $1->lineno);
    verifyFuncCall(symbols_table, $3->params_types_or_enum_values ,$1->name, $1->lineno);
    $$->params_types_or_enum_values = $3->params_types_or_enum_values;
    $$->params_names = $3->params_names;
    $$->params_lineno = $3->params_lineno;
    $$->lineno = $2->lineno;

    $$->name = $1->name;//this is the name of the func.
    $$->type = getFuncRetType(symbols_table,$1->name);

    $$->llvm_type = TypeToLLVM($$->type);//llvm return type

    string list_of_parameters_llvm_type_register_llvm;


    string func_name = $$->name;
    string cmmd;

    if($1->name == "print"){
        string length_of_str = ($3->size_string );//+1 is for \00
        string print_call = "call void @print( i8* getelementptr([" + length_of_str + " x i8], [" + length_of_str + " x i8]* " + $3->llvm_register +", i32 0, i32 0))";
        emitCommand(print_call,"call to print with string" + $3->llvm_register);

    } else if($1->name == "printi"){
        string var_name = *$$->params_names.begin();
        frameEntry * ptr =  symbols_table->returnEntryByName(var_name);
        if(ptr == nullptr || ptr->entry_type != ENUM_VAR){
//            emitCommand( "",""+$3->name+" Not in Symbols Table! ");
            string reg_exp;
            reg_exp = $3->llvm_register;
            if($3->llvm_type == "i32"){
                //this is including enum.
                reg_exp = $3->llvm_register;
                string command="call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8],[4 x i8]* @.forPrinti, i32 0, i32 0), i32 "+reg_exp+")";
                emitCommand(command,"printi with literal from register: "+ reg_exp);

            } else {
                string zext = freshRegister();
                string zext_cmmd = zext +" = zext i8 "+reg_exp+" to i32";//zext i8 %.t6 to i32
                string new_reg_exp = zext;
                string command="call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8],[4 x i8]* @.forPrinti, i32 0, i32 0), i32 "+new_reg_exp+")";
                emitCommand(zext_cmmd,"zext to printi");
                emitCommand(command,"printi with byte from register: "+ new_reg_exp);
            }

        }else{
            list_of_parameters_llvm_type_register_llvm = emit_for_function_call_and_return_list_registers($$);

            emitCommand("call void @"+$1->name+"("+list_of_parameters_llvm_type_register_llvm+")","");

        }


    }
    else{
    emitCommand("","----------------------- This is the name of func: "+ $1->name);
        string llvm_type;
        llvm_type = RetTypeToLLVM(getFuncRetType(symbols_table,$$->name));

        string ret_type_func_llvm = RetTypeToLLVM($$->type);
        string list_of_parameters_llvm_type_register_llvm;

        frameEntry* entry_ptr = symbols_table->returnEntryByName($1->name);
        list_of_parameters_llvm_type_register_llvm = emit_for_function_call_and_return_list_registersA($$,entry_ptr->params_types_or_enum_values);

        if(llvm_type == "void" ){
            cmmd ="call void @"+func_name+"("+list_of_parameters_llvm_type_register_llvm+")";

        }else {
            string fresh_register = freshRegister();
            cmmd +=fresh_register+" = call "+llvm_type+" @"+func_name+"("+list_of_parameters_llvm_type_register_llvm+")";
            $$->llvm_register = fresh_register;
        }
            emitCommand(cmmd,"");
            //call void @printi(i32* %XX0)
            emitCommand("","----------------------- This is the end of function call : "+ $1->name+" function. ");
    }

}

void ID_LPAREN_RPAREN_Setup(Node_Az * $$ , Node_Az *$1){
    verifyFuncDefined(symbols_table,$1->name, $1->lineno);
    verifyFuncCall(symbols_table,stam ,$1->name, $1->lineno);
    $$->params_types_or_enum_values = stam;
    $$->params_names = stam;
    $$->params_lineno = stam_lineno;

    $$->name = $1->name;//this is the name of the func.
    $$->type = getFuncRetType(symbols_table,$$->name);
    $$->lineno = $1->lineno;
    $$->llvm_type = TypeToLLVM($$->type);//llvm return type

    string cmmd;
    string func_name = $$->name;
    string fresh_register = freshRegister();
    string llvm_type;
    llvm_type = RetTypeToLLVM(getFuncRetType(symbols_table,$$->name));

    emitCommand("","----------------------- This is the name of func: "+ $1->name+" and the Rettype:"+$$->llvm_type);
    if(llvm_type == "void" ){
        cmmd ="call void @"+func_name+"()";

    }else {
        cmmd +=fresh_register+" = call "+llvm_type+" @"+func_name+"()";
    }
    emitCommand(cmmd,"-"+llvm_type+"-");
    emitCommand("","----------------------- This is the end of function call, of : "+ $1->name+" function. ");

    $$->llvm_register = fresh_register;
}


// ------------------------ ExpList Rules Setup ------------------------

void Exp_Setup(Node_Az * $$ , Node_Az *$1){
    $$->params_types_or_enum_values = stam;
    $$->params_types_or_enum_values.push_back($1->type);

    $$->params_names = stam;
    $$->params_names.push_back($1->name);

    $$->params_lineno = stam_lineno;
    $$->params_lineno.push_back($1->lineno);

    $$->lineno = $1->lineno;

    //hw5
    $$->list_of_registers = stam;
    $$->list_of_registers.push_back($1->llvm_register);

    $$->name = $1->name;
    $$->llvm_register = $1->llvm_register;
    $$->is_global_var = $1->is_global_var;

    if($$->is_global_var){    $$->size_string = $1->size_string; }

}

void Exp_COMMA_ExpList_Setup(Node_Az * $$ , Node_Az *$1, Node_Az *$2,Node_Az *$3){
    $$->params_types_or_enum_values = $3->params_types_or_enum_values;
    $$->params_names = $3->params_names;
    $$->params_lineno = $3->params_lineno;

    $$->params_types_or_enum_values.push_back($1->type);
    $$->params_names.push_back($1->name);
    $$->params_lineno.push_back($1->lineno);

    $$->lineno = $1->lineno;

    //hw5
    $$->list_of_registers = $3->list_of_registers;
    $$->list_of_registers.push_back($1->llvm_register);

    }


// ------------------------ Tokens Setup ------------------------

void IntTokenSetup(Node_Az * $$ , Node_Az *op){
    $$->type = op->type;
    $$->lineno = op->lineno;
    $$->llvm_type = "i32";
}

void ByteTokenSetup(Node_Az * $$ , Node_Az *op){
    $$->type = op->type;
    $$->lineno = op->lineno;
    $$->llvm_type = "i8";
}

void BoolTokenSetup(Node_Az * $$ , Node_Az *op){
    $$->type = op->type;
    $$->lineno = op->lineno;
    $$->llvm_type = "i1";
}


// ------------------------ Enum Rules Setup ------------------------


void Enum_ID_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2){
    $$->type ="enum "+ $2->name;
    $$->lineno = $1->lineno;
}

void EnumDecl_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3,Node_Az *$4){
    $$->params_types_or_enum_values = $4->params_types_or_enum_values;
    $$->params_lineno = $4->params_lineno;
    $$->type = "enum " + $2->name;
    $$->lineno = $2->lineno;
    $$->eType = ENUM_DEC;
    InsertEnumEntryToCurrentFrame(symbols_table, $$->type, $2->name, $$->params_types_or_enum_values, ENUM_DEC,$1->lineno );
    pushEnumValues(symbols_table, $$->params_types_or_enum_values, $$->params_lineno,$$->type);

    // output::printProductionRule(7);
    }


// ------------------------ Exp Setup ------------------------

void LPAREN_Exp_RPAREN(Node_Az * $$ , Node_Az *$1,Node_Az *$2){
    $$->type = $2->type;
    $$->name = $2->name;
    $$->lineno = $2->lineno;

    $$->llvm_type = $2->llvm_type;
    $$->llvm_register = $2->llvm_register;
}


void Exp_MULDIV_Exp(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3){

    emitCommand("","In Exp_MULDIV_Exp ");
    checkNumTypeOfTwo($1->type,$1->lineno,$3->type,$3->lineno);
    $$->type = getBiggerNumTypeGroupName($1->type,$3->type);


    string fresh_register ;
    string llvm_type;
    string zero_division_register;
    string res;
    if($1->type == $3->type){
        fresh_register = freshRegister();
        if($1->type == "BYTE") {
            if ($2->binaryOp.type == OP_MUL) {
                emitCommand(fresh_register + " = mul  i8 " + $1->llvm_register + ", " + $3->llvm_register,
                            "Exp_MULDIV_Exp func");
            } else{
                zero_division_register = fresh_register;

                codeBuffer.emit(zero_division_register + " = icmp eq i8 " + $3->llvm_register + ", 0");
                int location = codeBuffer.emit("br i1 " + zero_division_register + ", label @, label @");

                string label_zero_exit = codeBuffer.genLabel();
                emit_zero_error_and_exit();

                string label_zero_continue = codeBuffer.genLabel();
                fresh_register = freshRegister();
                codeBuffer.emit(fresh_register + " = sdiv i8 " + $1->llvm_register + ", " + $3->llvm_register);
                pair<int, BranchLabelIndex>* p = new pair<int, BranchLabelIndex>({location, FIRST});
                codeBuffer.bpatch(codeBuffer.makelist(p), label_zero_exit);
                p = new pair<int, BranchLabelIndex>({location, SECOND});
                codeBuffer.bpatch(codeBuffer.makelist(p), label_zero_continue);
            }

            if ($$->type == "INT") {
                llvm_type = "i32";
            } else { llvm_type = "i8"; }

        }else {
            llvm_type = "i32";
            if ($2->binaryOp.type == OP_MUL) {
                emitCommand(fresh_register + " = mul  "+llvm_type+" " + $1->llvm_register + ", " + $3->llvm_register,
                            "Exp_MULDIV_Exp func");
            } else{
                zero_division_register = fresh_register;

                codeBuffer.emit(zero_division_register + " = icmp eq i32 " + $3->llvm_register + ", 0");
                int location = codeBuffer.emit("br i1 " + zero_division_register + ", label @, label @");

                string label_zero_exit = codeBuffer.genLabel();
                emit_zero_error_and_exit();

                string label_zero_continue = codeBuffer.genLabel();
                fresh_register = freshRegister();
                codeBuffer.emit(fresh_register + " = sdiv i32 " + $1->llvm_register + ", " + $3->llvm_register);

//                codeBuffer.bpatch(codeBuffer.makelist({location, FIRST}), label_zero_exit);
//                codeBuffer.bpatch(codeBuffer.makelist({location, SECOND}), label_zero_continue);

                pair<int, BranchLabelIndex>* p = new pair<int, BranchLabelIndex>({location, FIRST});
                codeBuffer.bpatch(codeBuffer.makelist(p), label_zero_exit);
                p = new pair<int, BranchLabelIndex>({location, SECOND});
                codeBuffer.bpatch(codeBuffer.makelist(p), label_zero_continue);
            }

        }
    }
    else{
        string zext_reg = freshRegister();

        if($1->type == "BYTE"){
            codeBuffer.emit(zext_reg + " = zext i8 " + $1->llvm_register + " to i32");
            $1->llvm_register = zext_reg;
        } else {
            codeBuffer.emit(zext_reg + " = zext i8 " + $3->llvm_register + " to i32");
            $3->llvm_register = zext_reg;
        }
        fresh_register = freshRegister();

        llvm_type = "i32";
        if ($2->binaryOp.type == OP_MUL) {
            emitCommand(fresh_register + " = mul  "+llvm_type+" " + $1->llvm_register + ", " + $3->llvm_register,
                        "Exp_MULDIV_Exp func");
        } else{
            zero_division_register = fresh_register;

            codeBuffer.emit(zero_division_register + " = icmp eq i32 " + $3->llvm_register + ", 0");
            int location = codeBuffer.emit("br i1 " + zero_division_register + ", label @, label @");

            string label_zero_exit = codeBuffer.genLabel();
            emit_zero_error_and_exit();

            string label_zero_continue = codeBuffer.genLabel();
            fresh_register = freshRegister();
            codeBuffer.emit(fresh_register + " = sdiv i32 " + $1->llvm_register + ", " + $3->llvm_register);

//            codeBuffer.bpatch(codeBuffer.makelist({location, FIRST}), label_zero_exit);
//            codeBuffer.bpatch(codeBuffer.makelist({location, SECOND}), label_zero_continue);
            pair<int, BranchLabelIndex>* p = new pair<int, BranchLabelIndex>({location, FIRST});
            codeBuffer.bpatch(codeBuffer.makelist(p), label_zero_exit);
            p = new pair<int, BranchLabelIndex>({location, SECOND});
            codeBuffer.bpatch(codeBuffer.makelist(p), label_zero_continue);
        }


    }
    $$->llvm_register = fresh_register;
    $$->llvm_type = llvm_type;

    codeBuffer.emit("");

    emitCommand("","out Exp_MULDIV_Exp ");
}


void IdTokenSetup(Node_Az * $$ , Node_Az *$1){
    checkIdExist(symbols_table, $1->name, $1->lineno);
    $$->type = getEntryType(symbols_table, $1->name);
    $$->name = $1->name;
    $$->lineno = $1->lineno;

    string fresh_register = freshRegister();
    string type = $$->type;
    string llvm_type ;
    //The name is the pointer to the location on the stack that holds the var's value.
    if(symbols_table->getEntryEType($1->name) == PARAM || symbols_table->getEntryEType($1->name) == ENUM_PARAM){
        string var_name = $1->name + symbols_table->getGlobalName($1->name);
        if(type == "INT"){
            emitCommand(fresh_register + " = load i32, i32* %" + var_name,"IdTokenSetup: id is : "+$$->name);
            llvm_type = "i32";
            $$->llvm_register = fresh_register;
            $$->llvm_type = llvm_type;
        } else if(type == "BOOL"){
            emitCommand(fresh_register + " = load i1, i1* %" + var_name,"IdTokenSetup: id is : "+$$->name);
            llvm_type = "i1";
            $$->llvm_register = fresh_register;
            $$->llvm_type = llvm_type;
        } else if(type == "BYTE"){
            emitCommand(fresh_register + " = load i8, i8* %" + var_name,"IdTokenSetup: id is : "+$$->name);
            llvm_type = "i8";
            $$->llvm_register = fresh_register;
            $$->llvm_type = llvm_type;
        }else{
            emitCommand(fresh_register + " = load i32, i32* %" + var_name,"IdTokenSetup: id is : "+$$->name);
            llvm_type = "i32";
            $$->llvm_register = fresh_register;
            $$->llvm_type = llvm_type;
        }
    }else{
        //cout << "offset: " << symbols_table->returnEntryByName($1->name)->offset << "\n";
        int offset_to_fp = symbols_table->returnEntryByName($1->name)->offset;
        std::stringstream off;
        off << offset_to_fp;
        string offset_relative_to_fp = off.str();
        string fresh_reg = freshRegister();
        if(type == "INT"){
            codeBuffer.emit(fresh_register + " = getelementptr [50 x i32], [50 x i32]* %fp, i32 0, i32 " + offset_relative_to_fp);
            emitCommand(fresh_reg + " = load i32, i32* " + fresh_register,"IdTokenSetup: id is : "+$$->name);
            llvm_type = "i32";
            $$->llvm_register = fresh_reg;
            $$->llvm_type = llvm_type;
        } else if(type == "BOOL"){
            codeBuffer.emit(fresh_register + " = getelementptr [50 x i32], [50 x i32]* %fp, i32 0, i32 " + offset_relative_to_fp);
            emitCommand(fresh_reg + " = load i32, i32* " + fresh_register,"IdTokenSetup: id is : "+$$->name);
            string fresh_r = freshRegister();
            codeBuffer.emit(fresh_r + " = trunc i32 " + fresh_reg + " to i1");
            llvm_type = "i1";
            $$->llvm_register = fresh_r;
            $$->llvm_type = llvm_type;
        } else if(type == "BYTE"){
            codeBuffer.emit(fresh_register + " = getelementptr [50 x i32], [50 x i32]* %fp, i32 0, i32 " + offset_relative_to_fp);
            emitCommand(fresh_reg + " = load i32, i32* " + fresh_register,"IdTokenSetup: id is : "+$$->name);
            string fresh_r = freshRegister();
            codeBuffer.emit(fresh_r + " = trunc i32 " + fresh_reg + " to i8");
            llvm_type = "i8";
            $$->llvm_register = fresh_r;
            $$->llvm_type = llvm_type;
        }else{
            vector<string>* list_enum_values = symbols_table->returnEnumValListEntryByName(type);
            bool is_enum_value = false;
            for(std::vector<string>::iterator itrator = list_enum_values->begin(); itrator != list_enum_values->end(); ++itrator){
                if((*itrator) == $1->name){
                    //cout << "iterator: " << (*itrator) << "\n";
                    is_enum_value = true;
                }
            }
            if(is_enum_value){
                std::stringstream enum_val;
                //cout << "enum_v: " << symbols_table->getIndexofEnumValue($1->name) << "\n";
                enum_val << symbols_table->getIndexofEnumValue($1->name);
                string enum_value = enum_val.str();
                emitCommand(fresh_register + " = add i32 0," + enum_value,"IdTokenSetup: id is : "+$$->name);
                llvm_type = "i32";
                $$->llvm_register = fresh_register;
                $$->llvm_type = llvm_type;
            }else{
                codeBuffer.emit(fresh_register + " = getelementptr [50 x i32], [50 x i32]* %fp, i32 0, i32 " + offset_relative_to_fp);
                emitCommand(fresh_reg + " = load i32, i32* " + fresh_register,"IdTokenSetup: id is : "+$$->name);
                llvm_type = "i32";
                $$->llvm_register = fresh_reg;
                $$->llvm_type = llvm_type;
            }
        }
    }
}

void Call_Setup(Node_Az * $$ , Node_Az *$1) {
    $$->type = $1->type;
    $$->llvm_register = $1->llvm_register;

    //%5 = call i32 @f2(i32 %4, double 1.000000e+00)
    emitCommand("", "Call_Setup: Nothing");
}

void NUM_Setup(Node_Az * $$ , Node_Az *$1){
    $$->type = "INT";
    $$->name = $1->name;//here is the value.
    $$->lineno = $1->lineno;
//    $$->int_val = CharPointerToInt_2($1->name.c_str());
    $$->intVal.setVal_char(($1->name).c_str());
//    $$->int_val =$$->intVal.val;

    string fresh_register = freshRegister();
    string command = fresh_register + " = add i32 0, " + $$->name;
    emitCommand(command,"emiting from  NUM_Setup literal: "+$$->name);
    $$->llvm_register = fresh_register;
    $$->llvm_type = "i32";
    codeBuffer.emit("");

}

void NUM_B_Setup(Node_Az * $$ , Node_Az *$1){
    checkByteSize($1->name, $1->lineno);
    //  output::printProductionRule(49);
    $$->type = "BYTE";
    $$->lineno = $1->lineno;
    $$->name = $1->name;
//    $$->int_val = CharPointerToInt_2($1->name.c_str());
    $$->byteVal.setVal_char(($1->name).c_str());
    //cout << "func NUM_B_Setup: this is $$->intVal.val :"<<$$->byteVal.val<< endl;
//    $$->int_val =$$->byteVal.val;

    string fresh_register = freshRegister();
    string command = fresh_register + " = add i8 0, " + $$->name;
    emitCommand(command,"emiting from  NUM_B_Setup literal: "+$$->name);
    $$->llvm_register = fresh_register;
    $$->llvm_type = "i8";
    codeBuffer.emit("");

}

void STRING_Setup(Node_Az * $$ , Node_Az *$1){
    //   output::printProductionRule(50);
    $$->name = $1->name;
    $$->type = "STRING";
    $$->lineno = $1->lineno;

    string size_STRING = numToString($$->name.size() +1 );//+1 is for \00
    $$->size_string = size_STRING;

    string string_val = $$->name;
    string str_global_var = "@.hw5_str." + freshGlobalInitNumber();
//    $$->str_global_var_llvm = str_global_var;

    $$->llvm_register = str_global_var;
    $$->is_global_var = true;
    $$->llvm_type = "i8";
    codeBuffer.emitGlobal($$->llvm_register+" = private unnamed_addr constant ["+size_STRING+" x i8] c"
                                         "\""+string_val+"\\00\""
                                         ", align 1");
}

void TRUE_Setup(Node_Az * $$ , Node_Az *$1){
    //  output::printProductionRule(51);
    $$->type = $1->type;
    $$->lineno = $1->lineno;
    $$->name = $1->name;
    $$->boolVal.type = BOOLVAL_TRUE;

    //hw5
    string fresh_register = freshRegister();
    string command = fresh_register + " = add i1 0, 1";
    emitCommand(command,"emiting from  TRUE_Setup literal: "+$$->name);
    $$->llvm_register = fresh_register;
    $$->llvm_type = "i1";
    codeBuffer.emit("");


}

void FALSE_Setup(Node_Az * $$ , Node_Az *$1){
    //  output::printProductionRule(52);
    $$->type = $1->type;
    $$->lineno = $1->lineno;
    $$->name = $1->name;

    string fresh_register = freshRegister();
    string command = fresh_register + " = add i1 0, 0";
    emitCommand(command,"emiting from  FALSE_Setup literal: "+$$->name);
    $$->llvm_register = fresh_register;
    $$->llvm_type = "i1";
    codeBuffer.emit("");

}

void NOT_Exp_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2){
    //  output::printProductionRule(53);
    checkBool($2->type, $2->lineno);
    $$->type = "BOOL";
    $$->lineno = $2->lineno;

    //hw5
    string fresh_register = freshRegister();
    string command = fresh_register + " = add i1 1, " + $2->llvm_register;
    emitCommand(command,"emiting from  NOT_Exp_Setup literal: "+$$->name);
    $$->llvm_register = fresh_register;
    $$->llvm_type = $2->llvm_type;
    codeBuffer.emit("");
    }

void MAND_Setup(Node_Az * $$){
//    %t9 = icmp eq i32 %t8, 1
//    br i1 %t9, label %label_37 , label %label_23
    $$->loc_short_circuit = emitCommand("br i1 @ , label @ , label @","MAND_Setup");
    $$->continue_short_circuit  = codeBuffer.genLabel();
    return;
}


/*//void Exp_AND_Exp_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3){
void Exp_AND_MAND_Exp_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3, Node_Az *$4){

    checkBoolOfTwo($1->type,$1->lineno,$4->type,$4->lineno);
    $$->type = "BOOL";
    $$->lineno = $2->lineno;

    //hw5 ------------------------------------------------------
    string and_register;
    string and_register_1;
    string and_register_2;

    and_register_1 = $1->llvm_register;
    and_register_2 = $4->llvm_register;

    int location_br_1 = $3->loc_short_circuit;
    int len_reg = $1->llvm_register.size() - 1 ;

    and_register_1 = and_register_1.substr(1,len_reg);

//    emitCommand("","and_register_1: "+and_register_1);
    codeBuffer.bpatch(codeBuffer.makelist({location_br_1, FIRST}), and_register_1);

//    string label_Exp1_continue = codeBuffer.genLabel();//continue to evaluate the second exp.
    string label_Exp1_continue = $3->continue_short_circuit;//continue to evaluate the second exp.
    int location_br_2 = codeBuffer.emit("br i1 " + and_register_2 + ", label @, label @");

    string label_false = codeBuffer.genLabel();

    int loc_go_end_false = codeBuffer.emit("br label @");

    string label_true = codeBuffer.genLabel();

    int loc_go_end_true = codeBuffer.emit("br label @");

    string label_end = codeBuffer.genLabel();
    and_register = freshRegister();
//    codeBuffer.emit(and_register+" = phi i1 [ 0, %"+label_false+"], [1, %"+label_true+"]");
    emitCommand(and_register+" = phi i1 [ 0, %"+label_false+"], [1, %"+label_true+"]","This is in and");


    codeBuffer.bpatch(codeBuffer.makelist({location_br_1, SECOND}),label_false  );
    codeBuffer.bpatch(codeBuffer.makelist({location_br_1, FIRST}),label_Exp1_continue );

    codeBuffer.bpatch(codeBuffer.makelist({location_br_2, FIRST}), label_true);
    codeBuffer.bpatch(codeBuffer.makelist({location_br_2, SECOND}), label_false);

    codeBuffer.bpatch(codeBuffer.makelist({loc_go_end_false, FIRST}), label_end);
    codeBuffer.bpatch(codeBuffer.makelist({loc_go_end_true, FIRST}), label_end);

    $$->llvm_register = and_register;
    $$->llvm_type = "i1";
}*/

//void Exp_AND_Exp_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3){
void Exp_AND_MAND_Exp_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3, Node_Az *$4){

    checkBoolOfTwo($1->type,$1->lineno,$4->type,$4->lineno);
    $$->type = "BOOL";
    $$->lineno = $2->lineno;

    //hw5 ------------------------------------------------------
    string and_register;
    string and_register_1;
    string and_register_2;

    and_register_1 = $1->llvm_register;
    and_register_2 = $4->llvm_register;

    int location_br_1 = $3->loc_short_circuit;
    int len_reg = $1->llvm_register.size() - 1 ;

    and_register_1 = and_register_1.substr(1,len_reg);

//    emitCommand("","and_register_1: "+and_register_1);

    pair<int, BranchLabelIndex>* p = new pair<int, BranchLabelIndex>({location_br_1, FIRST});
    codeBuffer.bpatch(codeBuffer.makelist(p), and_register_1);

//    string label_Exp1_continue = codeBuffer.genLabel();//continue to evaluate the second exp.
    string label_Exp1_continue = $3->continue_short_circuit;//continue to evaluate the second exp.
    int location_br_2 = codeBuffer.emit("br i1 " + and_register_2 + ", label @, label @");

    string label_false = codeBuffer.genLabel();

    int loc_go_end_false = codeBuffer.emit("br label @");

    string label_true = codeBuffer.genLabel();

    int loc_go_end_true = codeBuffer.emit("br label @");

    string label_end = codeBuffer.genLabel();
    and_register = freshRegister();
//    codeBuffer.emit(and_register+" = phi i1 [ 0, %"+label_false+"], [1, %"+label_true+"]");
    emitCommand(and_register+" = phi i1 [ 0, %"+label_false+"], [1, %"+label_true+"]","This is in and");

    p = new pair<int, BranchLabelIndex>({location_br_1, SECOND});
    codeBuffer.bpatch(codeBuffer.makelist(p),label_false  );


    p = new pair<int, BranchLabelIndex>({location_br_1, FIRST});
    codeBuffer.bpatch(codeBuffer.makelist(p),label_Exp1_continue );

    p = new pair<int, BranchLabelIndex>({location_br_2, FIRST});
    codeBuffer.bpatch(codeBuffer.makelist(p), label_true);

    p = new pair<int, BranchLabelIndex>({location_br_2, SECOND});
    codeBuffer.bpatch(codeBuffer.makelist(p), label_false);

    p = new pair<int, BranchLabelIndex>({loc_go_end_false, FIRST});
    codeBuffer.bpatch(codeBuffer.makelist(p), label_end);

    p = new pair<int, BranchLabelIndex>({loc_go_end_true, FIRST});
    codeBuffer.bpatch(codeBuffer.makelist(p), label_end);

    $$->llvm_register = and_register;
    $$->llvm_type = "i1";
}



void MOR_Setup(Node_Az * $$){
//    %t9 = icmp eq i32 %t8, 1
//    br i1 %t9, label %label_37 , label %label_23

    $$->loc_short_circuit = emitCommand("br i1 @ , label @ , label @","MOR_Setup");
    $$->continue_short_circuit  = codeBuffer.genLabel();
    return;
}
/*

//void Exp_OR_MOR_Exp_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3){
void Exp_OR_MOR_Exp_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3, Node_Az *$4){

    //    output::printProductionRule(55);
    checkBoolOfTwo($1->type,$1->lineno,$4->type,$4->lineno);
    $$->type = "BOOL";
    $$->lineno = $2->lineno;

    //hw5 ------------------------------------------------------
    string or_register;
    string or_register_1;
    string or_register_2;

    or_register_1 = $1->llvm_register;
    or_register_2 = freshRegister();

    int location_br_1 = $3->loc_short_circuit;
    int len_reg = $1->llvm_register.size() - 1 ;
    or_register_1 = or_register_1.substr(1,len_reg);
    emitCommand("","or_register_1: "+or_register_1);
    codeBuffer.bpatch(codeBuffer.makelist({location_br_1, FIRST}), or_register_1);

    string label_Exp1_continue = $3->continue_short_circuit;//continue to evaluate the second exp.

    codeBuffer.emit(or_register_2 + " = icmp eq i1 " + $4->llvm_register + ", 1");
    int location_br_2 = codeBuffer.emit("br i1 " + or_register_2 + ", label @, label @");

    string label_false = codeBuffer.genLabel();

    int loc_go_end_false = codeBuffer.emit("br label @");

    string label_true = codeBuffer.genLabel();

    int loc_go_end_true = codeBuffer.emit("br label @");

    string label_end = codeBuffer.genLabel();

    or_register = freshRegister();
    codeBuffer.emit(or_register + " = phi i1 [ 0 , %" + label_false + "], [ 1 , %" + label_true + "]");

//    codeBuffer.bpatch(codeBuffer.makelist({location_br_1, FIRST}),label_Exp1_continue );
//    codeBuffer.bpatch(codeBuffer.makelist({location_br_1, SECOND}), label_true);


    codeBuffer.bpatch(codeBuffer.makelist({location_br_1, FIRST}), label_true);
    codeBuffer.bpatch(codeBuffer.makelist({location_br_1, FIRST}),label_Exp1_continue );

    codeBuffer.bpatch(codeBuffer.makelist({location_br_2, FIRST}), label_true);
    codeBuffer.bpatch(codeBuffer.makelist({location_br_2, SECOND}), label_false);

    codeBuffer.bpatch(codeBuffer.makelist({loc_go_end_false, FIRST}), label_end);
    codeBuffer.bpatch(codeBuffer.makelist({loc_go_end_true, FIRST}), label_end);

    $$->llvm_register = or_register;
    $$->llvm_type = "i1";

}
*/

//void Exp_OR_MOR_Exp_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3){
void Exp_OR_MOR_Exp_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3, Node_Az *$4){

    //    output::printProductionRule(55);
    checkBoolOfTwo($1->type,$1->lineno,$4->type,$4->lineno);
    $$->type = "BOOL";
    $$->lineno = $2->lineno;

    //hw5 ------------------------------------------------------
    string or_register;
    string or_register_1;
    string or_register_2;

    or_register_1 = $1->llvm_register;
    or_register_2 = freshRegister();

    int location_br_1 = $3->loc_short_circuit;
    int len_reg = $1->llvm_register.size() - 1 ;
    or_register_1 = or_register_1.substr(1,len_reg);
    emitCommand("","or_register_1: "+or_register_1);
    pair<int, BranchLabelIndex> *p = new pair<int, BranchLabelIndex>({location_br_1, FIRST});
    codeBuffer.bpatch(codeBuffer.makelist(p), or_register_1);

    string label_Exp1_continue = $3->continue_short_circuit;//continue to evaluate the second exp.

    codeBuffer.emit(or_register_2 + " = icmp eq i1 " + $4->llvm_register + ", 1");
    int location_br_2 = codeBuffer.emit("br i1 " + or_register_2 + ", label @, label @");

    string label_false = codeBuffer.genLabel();

    int loc_go_end_false = codeBuffer.emit("br label @");

    string label_true = codeBuffer.genLabel();

    int loc_go_end_true = codeBuffer.emit("br label @");

    string label_end = codeBuffer.genLabel();

    or_register = freshRegister();
    codeBuffer.emit(or_register + " = phi i1 [ 0 , %" + label_false + "], [ 1 , %" + label_true + "]");

//    codeBuffer.bpatch(codeBuffer.makelist({location_br_1, FIRST}),label_Exp1_continue );
//    codeBuffer.bpatch(codeBuffer.makelist({location_br_1, SECOND}), label_true);

    p = new pair<int, BranchLabelIndex>({location_br_1, FIRST});
    codeBuffer.bpatch(codeBuffer.makelist(p), label_true);

//    p = new pair<int, BranchLabelIndex>({location_br_1, FIRST});
    codeBuffer.bpatch(codeBuffer.makelist(p),label_Exp1_continue );

    p = new pair<int, BranchLabelIndex>({location_br_2, FIRST});
    codeBuffer.bpatch(codeBuffer.makelist(p), label_true);

    p = new pair<int, BranchLabelIndex>({location_br_2, SECOND});
    codeBuffer.bpatch(codeBuffer.makelist(p), label_false);

    p = new pair<int, BranchLabelIndex>({loc_go_end_false, FIRST});
    codeBuffer.bpatch(codeBuffer.makelist(p), label_end);

    p = new pair<int, BranchLabelIndex>({loc_go_end_true, FIRST});
    codeBuffer.bpatch(codeBuffer.makelist(p), label_end);

    $$->llvm_register = or_register;
    $$->llvm_type = "i1";

}


void Exp_RELOPNONASSOC_Exp_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3){
    checkNumTypeOfTwo($1->type,$1->lineno,$3->type,$3->lineno);
    //   output::printProductionRule(56);
    $$->type = "BOOL";
    $$->lineno = $2->lineno;


    if(($1->type == "BYTE" && $3->type == "INT") || ($1->type == "INT" && $3->type == "BYTE")){
        if($1->type == "BYTE"){
            string fresh_register = freshRegister();
            codeBuffer.emit(fresh_register + " = zext i8 " + $1->llvm_register + " to i32");
            $1->llvm_register = fresh_register;
        }else{
            string fresh_register = freshRegister();
            codeBuffer.emit(fresh_register + " = zext i8 " + $3->llvm_register + " to i32");
            $3->llvm_register = fresh_register;
        }
    }
    if(($1->type == "BYTE") && ($3->type == "BYTE")){
        string fresh_register = freshRegister();
        string fresh_register_1 = freshRegister();
        codeBuffer.emit(fresh_register + " = zext i8 " + $1->llvm_register + " to i32");
        codeBuffer.emit(fresh_register_1 + " = zext i8 " + $3->llvm_register + " to i32");
        $1->llvm_register = fresh_register;
        $3->llvm_register = fresh_register_1;
    }

    string fresh_register = freshRegister();
    if($2->name == ">"){
//        codeBuffer.emit(fresh_register + " = icmp sgt i32 " + $1->llvm_register + ", " + $3->llvm_register);
        emitCommand(fresh_register + " = icmp sgt i32 " + $1->llvm_register + ", " + $3->llvm_register,"Exp_RELOPNONASSOC_Exp_Setup : >");
    }else if($2->name == "<"){
//        codeBuffer.emit(fresh_register + " = icmp slt i32 " + $1->llvm_register + ", " + $3->llvm_register);
        emitCommand(fresh_register + " = icmp slt i32 " + $1->llvm_register + ", " + $3->llvm_register,"Exp_RELOPNONASSOC_Exp_Setup : <");
    }else if($2->name == "<="){
//        codeBuffer.emit(fresh_register + " = icmp sle i32 " + $1->llvm_register + ", " + $3->llvm_register);
       emitCommand(fresh_register + " = icmp sle i32 " + $1->llvm_register + ", " + $3->llvm_register,"Exp_RELOPNONASSOC_Exp_Setup : <=");
    }else if($2->name == ">="){
//        codeBuffer.emit(fresh_register + " = icmp sge i32 " + $1->llvm_register + ", " + $3->llvm_register);
        emitCommand(fresh_register + " = icmp sge i32 " + $1->llvm_register + ", " + $3->llvm_register,"Exp_RELOPNONASSOC_Exp_Setup : >=");
    }
    $$->llvm_register = fresh_register;
    $$->llvm_type = "i1";


    }

void LPAREN_Type_RPAREN_Exp_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3,Node_Az *$4){
    //Casting  - we can cast only from enum to int according to Q&A.
    //Exp:
    //enum day {SUNDAY, MONDAY, TUESDAY};
    //enum day d = TUESDAY;(int) d;
    checkEnumCastingToInt(symbols_table, $2->type ,$4->name, $4->lineno);
    $$->type = $2->type;
    $$->lineno = $2->lineno;

    $$->llvm_register = $4->llvm_register;
    $$->llvm_type = "i32";



    }

void Exp_ADDSUB_Exp_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3){
    checkNumTypeOfTwo($1->type,$1->lineno,$3->type,$3->lineno);
    $$->type = getBiggerNumTypeGroupName($1->type,$3->type);
    $$->lineno = $2->lineno;
    //hw5
    string fresh_register;
    string llvm_type;

    if($1->type == $3->type){
             fresh_register = freshRegister();
            if($1->type == "BYTE"){
                if ($2->binaryOp.type == OP_SUM) {
                    codeBuffer.emit(fresh_register + " = add i8 " + $1->llvm_register + ", " + $3->llvm_register);
                } else {
                    codeBuffer.emit(fresh_register + " = sub i8 " + $1->llvm_register + ", " + $3->llvm_register);
                }
                if($$->type == "INT"){
                fresh_register = freshRegister();
                codeBuffer.emit(fresh_register + " = zext i8 " + $1->llvm_register + " to i32");
                    llvm_type = "i32";

                } else {                llvm_type = "i8";
                }
            } else {
                if ($2->binaryOp.type == OP_SUM) {
                    codeBuffer.emit(fresh_register + " = add i32 " + $1->llvm_register + ", " + $3->llvm_register);
                } else {
                    codeBuffer.emit(fresh_register + " = sub i32 " + $1->llvm_register + ", " + $3->llvm_register);
                }
                llvm_type = "i32";
            }

    }
    else {

            string zext_reg = freshRegister();

               if($1->type == "BYTE"){
                   codeBuffer.emit(zext_reg + " = zext i8 " + $1->llvm_register + " to i32");
                   $1->llvm_register = zext_reg;
               } else {
                   codeBuffer.emit(zext_reg + " = zext i8 " + $3->llvm_register + " to i32");
                   $3->llvm_register = zext_reg;
               }
                fresh_register = freshRegister();
                if ($2->binaryOp.type == OP_SUM) {
                    codeBuffer.emit(fresh_register + " = add i32 " + $1->llvm_register + ", " + $3->llvm_register);
                } else {
                    codeBuffer.emit(fresh_register + " = sub i32 " + $1->llvm_register + ", " + $3->llvm_register);
                }
                llvm_type = "i32";
        }

    $$->llvm_register = fresh_register;
    $$->llvm_type = llvm_type;
}

void Exp_RELOPASSOC_Exp_Setup(Node_Az * $$ , Node_Az *$1,Node_Az *$2,Node_Az *$3){
    checkNumTypeOfTwo($1->type,$1->lineno,$3->type,$3->lineno);
    //	output::printProductionRule(56);
    $$->lineno = $2->lineno;
    $$->type = "BOOL";

    //hw5 -----------------------

    if(($1->type == "BYTE" && $3->type == "INT") || ($1->type == "INT" && $3->type == "BYTE")){
        if($1->type == "BYTE"){
            string fresh_register = freshRegister();
            codeBuffer.emit(fresh_register + " = zext i8 " + $1->llvm_register + " to i32");
            $1->llvm_register = fresh_register;
        }else{
            string fresh_register = freshRegister();
            codeBuffer.emit(fresh_register + " = zext i8 " + $3->llvm_register + " to i32");
            $3->llvm_register = fresh_register;
        }
    }
    if(($1->type == "BYTE") && ($3->type == "BYTE")){
        string fresh_register = freshRegister();
        string fresh_register_1 = freshRegister();
        codeBuffer.emit(fresh_register + " = zext i8 " + $1->llvm_register + " to i32");
        codeBuffer.emit(fresh_register_1 + " = zext i8 " + $3->llvm_register + " to i32");
        $1->llvm_register = fresh_register;
        $3->llvm_register = fresh_register_1;
    }

    string fresh_register = freshRegister();
    if($2->name == "=="){
//        codeBuffer.emit(fresh_register + " = icmp eq i32 " + $1->llvm_register + ", " + $3->llvm_register);
        emitCommand(fresh_register + " = icmp eq i32 " + $1->llvm_register + ", " + $3->llvm_register,"Exp_RELOPASSOC_Exp_Setup : ==");
    }else if($2->name == "!="){
//        codeBuffer.emit(fresh_register + " = icmp ne i32 " + $1->llvm_register + ", " + $3->llvm_register);
        emitCommand(fresh_register + " = icmp ne i32 " + $1->llvm_register + ", " + $3->llvm_register,"Exp_RELOPASSOC_Exp_Setup : !=");
    }
    $$->llvm_register = fresh_register;
    $$->llvm_type = "i1";
}

#endif //INC_236360_HW5_PARSER_HW5_HPP
