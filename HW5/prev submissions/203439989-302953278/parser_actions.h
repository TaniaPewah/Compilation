//
// Created by hilak on 12/14/2019.
//

#ifndef INC_236360_HW3_PARSER_ACTIONS_H
#define INC_236360_HW3_PARSER_ACTIONS_H
//
#include "Table.hpp"
#include "hw3_output.hpp"
#include "bp.hpp"
//#include <assert.h>
//
Table * InitNewTable();
void DeleteTable(Table ** symbol_table);
bool checkEntryExists(Table * symbol_table,string name);


//also check if input entry is already  in the all the symbol table.
//void InsertEntryToCurrentFrame(Table * symbol_table,string type,string name,vector<string> params_types,int entry_type,int offset=0);
void InsertEntryToCurrentFrame(Table * symbol_table,string type,string name,vector<string> params_types_or_enum_values,E_TYPE entry_type, int lineno,int offset=0);
void PushNewScope(Table * symbol_table);
void PopCurrentScope(Table * symbol_table);
void PushParams(Table* symbol_table,vector<string> params_types,vector<string> params_names, vector<int> params_lineno,vector<E_TYPE> etypes);
void pushEnumValues(Table* symbol_table,vector<string> enum_values, vector<int> params_lineno, string enum_type)    ;
void PrintEnumValues( vector<string> enum_values);
string entryTypeToString(int entry_type);

bool isEnumParamVectorLegal(vector<string> params_types_or_enum_values, vector<int> params_lineno);
void InsertEnumEntryToCurrentFrame(Table * symbol_table,string type,string name,vector<string> params_types_or_enum_values,E_TYPE entry_type, int lineno,int offset=0);
void parserErrorUndefEnum(int lineno, const string& id);
/*void PrintParams(vector<string> params_types, vector<string> params_names);*/
void printEntry(Table* symbol_table,string entry_name);

string getBiggerNumTypeGroupName( string name_type_group1, string name_type_group2);
void checkEnumDef(Table * symbol_table, int l, const string& s );
int stringToNum(const char* str);
void checkEnumTypeDef(Table* symbol_table,string enum_type);
bool checkEnumValExist(Table* symbol_table, string enum_type, string enum_val_to_check);
//void checkEnumValLegal(Table* symbols_table, string enum_type, string enum_val, int lineno);

string getEntryType(Table* symbols_table,string id_name);
void checkEnumTypeUndef(Table* symbol_table,string enum_type, int lineno);
void checkEnumAssign(Table* symbol_table, string enum_type, string id_or_enum_val,string name,int lineno);
void checkIdExist(Table* symbols_table,string id_name, int lineno);
void checkByteSize(string num, int lineno);
void checkBool(string b, int lineno);
void checkBoolOfTwo(string b1, int lineno1, string b2, int lineno2);

void checkAssign(Table* symbol_table, string name_type, string id_or_enum_val,int lineno);
void checkAssignID(Table* symbol_table, string id_name, string id_or_enum_val,int lineno);

void checkNumType( string name_type_group, int lineno);
void checkNumTypeOfTwo( string name_type_group1,int lineno1, string name_type_group2,int lineno2);
void checkEnumCastingToInt(Table* symbol_table,string type, string enum_id, int lineno);
template <class T>
vector<T> reverseVector(vector<T> v);
void closeTableAndVerifyMain(Table* symbol_table);
void verifyMainExists(Table* symbol_table);

void verifyBreak(int inside_a_while, int lineno);
void verifyContinue(int inside_a_while, int lineno);

void verifyFuncDefined(Table* symbol_table,string name_func, int lineno);

string getFuncRetType(Table* symbol_table,string name_func);

void verifyFuncCall(Table* symbol_table,vector<string> types_to_check ,string name_func, int lineno);

void verifyFuncReturn(Table* symbol_table,string current_func_name ,string return_type, int lineno);

void outputEndOfScope(Table* symbols_table);

//void initAssembly();

template <class T>
void printVec(vector<T> v){
    for (auto i = v.end()-1; i >= v.begin() ; --i) {
        cout <<*i<<" "<<endl;
    }
}

int getIndexofEnumValue(string name);
void setIndexofEnumValue(string name, int index);

#endif //INC_236360_HW3_PARSER_ACTIONS_H
