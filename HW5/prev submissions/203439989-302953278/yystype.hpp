//
// Created by hilak on 12/12/2019.
//

#ifndef INC_236360_HW3_YYSTYPE_H
#define INC_236360_HW3_YYSTYPE_H

#include <vector>
#include <string>
#include <iostream>
#include "Table.hpp"
//#include "typeEnums.hpp"
#include "Node.hpp"
#include "bp.hpp"

using namespace std;

extern CodeBuffer& codeBuffer;
extern vector<string> stam;//just a vector for inputing var

enum ListLabelType {TRUE_LIST,FALSE_LIST,CONTINUE_LIST,BREAK_LIST, END_IF_LIST };


struct Node_Az{
    string type;
    string name;
    vector<string>  params_types_or_enum_values = stam;
    vector<string> params_names;
    vector<int> params_lineno;
    int lineno;
    E_TYPE eType = ENULL;//only for enum.
    vector<E_TYPE> etypes;

    //Code for HW5;
    BinaryOp  binaryOp;
    RelativeOp  relativeOp;
    BoolVal  boolVal;
    IntVal intVal;
    ByteVal byteVal;
    NumVal  numVal;
    IdVal  idVal;
    StrVal  strVal;

    //string var;//in buffer we save it.

    //for dealing with buffer:
    int code_loc_start;
    int code_loc_end;
    string code_label_start;
    string code_label_end;
    vector<pair<int, BranchLabelIndex>*> true_list ;//has <0,0>
    vector<pair<int, BranchLabelIndex>*> false_list ;//= codeBuffer.makelist({});//has <0,0>;
    vector<pair<int, BranchLabelIndex>*> end_if_list ;//= codeBuffer.makelist({});//has <0,0>;
    vector<pair<int, BranchLabelIndex>*> break_list ;//= codeBuffer.makelist({});//has <0,0>;
    vector<pair<int, BranchLabelIndex>*> continue_list ;//= codeBuffer.makelist({});//has <0,0>;
    string next_instruction;

    string llvm_register= "";//format is %<number> or %<name><offset> for variable.
    int init_num_declartaion_id=0;//in case that we have multiple declarations of "type id;"
    string llvm_type="";
    vector<string> list_of_registers = stam; //for ExpList, format is %<number> or %<name><offset> for variable.
    string res_of_func=""; //register type according to type.

    string start_while_label = "";

    bool is_global_var = false ;//for string var.
//    string str_global_var_llvm ;
    string size_string= "1";

    //for short circuit.
    int loc_short_circuit ;
    string continue_short_circuit;
//
};


#define YYSTYPE Node_Az



#endif //INC_236360_HW3_YYSTYPE_H
