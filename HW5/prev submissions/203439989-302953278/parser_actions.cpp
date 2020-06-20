//
// Created by hilak on 12/14/2019.
#include "Table.hpp"
#include "yystype.hpp"
#include <iostream>
#include "hw3_output.hpp"
#include <assert.h>
#include "bp.hpp"


using namespace std;
using namespace output;

#ifndef INC_236360_HW3_PARSER_ACTIONS_CPP
#define INC_236360_HW3_PARSER_ACTIONS_CPP
vector<string> print;
vector<string> printi_int;
vector<string> printi_byte;



void outputEndOfScope(Table* symbols_table){
    endScope();
    for(auto i = symbols_table->frames.back()->entries.begin(); i != symbols_table->frames.back()->entries.end(); ++i){
        if((*i)->entry_type == VAR || (*i)->entry_type == PARAM || (*i)->entry_type == ENUM_VAR || (*i)->entry_type == ENUM_PARAM){
            printID((*i)->entry_name,(*i)->offset,(*i)->type_name);
        }else if((*i)->entry_type == FUNC){
            string function_type = makeFunctionType((*i)->type_name,(*i)->params_types_or_enum_values);
            printID((*i)->entry_name,(*i)->offset,function_type);
        }
    }
    for(auto i = symbols_table->frames.back()->entries.begin(); i != symbols_table->frames.back()->entries.end(); ++i){
         if((*i)->entry_type == ENUM_DEC){
             printEnumType((*i)->entry_name,(*i)->params_types_or_enum_values);
         }
    }
}


template <class T>
vector<T> reverseVector(vector<T> v){
    vector<T> r ;
    for (auto i = v.end()-1; i >= v.begin() ; --i) {
        r.push_back(*i);
    }
    return r;
}



Table * InitNewTable() {
    Table * ptr = new Table();
    ptr->pushFrame(); //inserting global frame.
    print.push_back("STRING");
    ptr->addEntryToFrame("print","VOID",print,FUNC);
    printi_int.push_back("INT");
    ptr->addEntryToFrame("printi","VOID",printi_int,FUNC);
//    printi_byte.push_back("BYTE");
//    ptr->addEntryToFrame("printi","VOID",printi_byte,FUNC);
    return ptr;
}

void DeleteTable(Table ** symbol_table){

    (*symbol_table)->popFrame(); //poping global frame;

    delete (*symbol_table);
}

bool checkEntryExists(Table * symbol_table,string name){
    bool b = symbol_table->isEntryExists(name);
    return b;
}

void InsertEnumEntryToCurrentFrame(Table * symbol_table,string type,string name,vector<string> params_types_or_enum_values,E_TYPE entry_type, int lineno,int offset=0){
        if(checkEntryExists(symbol_table,name)){
            errorDef(lineno,name);
        }


       //symbol_table->addEntryToFrame(name,type,params_types_or_enum_values,entry_type,offset);
       symbol_table->addEntryToFrame(name,type,params_types_or_enum_values,entry_type,offset);

}

void InsertEntryToCurrentFrame(Table * symbol_table,string type,string name,vector<string> params_types_or_enum_values,E_TYPE entry_type, int lineno,int offset=0){
        if(checkEntryExists(symbol_table,name)){
            errorDef(lineno,name);
        }

    if(params_types_or_enum_values.size() > 0){
        params_types_or_enum_values = reverseVector(params_types_or_enum_values);
    }
    symbol_table->addEntryToFrame(name,type,params_types_or_enum_values,entry_type,offset);
}

void PushParams(Table* symbol_table,vector<string> params_types,vector<string> params_names, vector<int> params_lineno,vector<E_TYPE> etypes){
   // cout << "-------------checkpoint 6-------------"<<endl;

    vector<string> empty;
   // cout << "-------------checkpoint 7-------------"<<endl;
    int offset_to_param = -1;
 //   cout << "-------------checkpoint 8-------------"<<endl;
  //  cout << "-------------checkpoint 9-------------"<<endl;
   // assert(params_names.size() == params_types.size() == params_lineno.size());
  if(params_types.size()>0) {
      params_names = reverseVector(params_names);
      params_types = reverseVector(params_types);
      params_lineno = reverseVector(params_lineno);
      etypes = reverseVector(etypes);
      auto k = params_lineno.begin();
      auto l = etypes.begin();
      for (auto i = params_types.begin(), j = params_names.begin();
           i != params_types.end(), j != params_names.end(), k != params_lineno.end(), l != etypes.end(); ++i, ++j, ++k, ++l) {
          //  cout << "-------------checkpoint 10 offset: "<<offset_to_param<<"-------------"<<endl;
          if(*l == VAR){
              InsertEntryToCurrentFrame(symbol_table, *i, *j, empty, PARAM, *k, offset_to_param);
          }else if(*l == ENUM_PARAM){
              InsertEntryToCurrentFrame(symbol_table, *i, *j, empty, ENUM_PARAM, *k, offset_to_param);

          }

          //  cout << "-------------checkpoint 11 offset: "<<offset_to_param<<"-------------"<<endl;
          offset_to_param--;
      }
  }
   // cout << "-------------checkpoint 12-------------"<<endl;

}
/*
void PrintParams(vector<string> params_types, vector<string> params_names){
    cout << "This is the Params: "<<endl;
    for(auto i = params_types.begin(), j = params_names.begin(); i != params_types.end(), j != params_names.end(); ++i, ++j){
        cout << "Param type: "<< *i << ", name: " <<*j << endl;
    }
    cout << "Finished printing the params."<<endl;
}
*/

void pushEnumValues(Table* symbol_table,vector<string> enum_values, vector<int> params_lineno, string enum_type){
    vector<string> empty;
    int index_enum = 0;
    auto k=params_lineno.begin();
    for(auto i = enum_values.begin();k!= params_lineno.end(), i != enum_values.end(); ++k,++i){
        InsertEntryToCurrentFrame(symbol_table,enum_type,*i,empty,ENUM_VALUE,*k);
        symbol_table->setIndexofEnumValue(*i, index_enum);
        index_enum++;
    }
}

bool checkEnumValExist(Table* symbol_table, string enum_type, string enum_val_to_check){
    vector<string>* ptr = symbol_table->returnEnumValListEntryByName(enum_type);
    if(ptr == nullptr) return false;
    vector<string> v = *ptr;
    for(auto i = v.begin(); i != v.end(); i++){
        if(*i == enum_val_to_check) {
            return true;
        }
    }
    return false;
}

void PrintEnumValues( vector<string> enum_values){
    cout << "This is the Enums: "<<endl;
    for(auto i = enum_values.begin(); i != enum_values.end(); ++i){
        cout << "Enum : "<< *i << endl;
    }
    cout << "Finished printing the enums."<<endl;
}


/*
string entryTypeToString(int entry_type){
    switch(entry_type){
        case 0:
            return "variable";
        case 1:
            return "function";
        case 2:
            return "parameter";
        case 3:
            return "enum";
        case 4:
            return "enum value";
        case 5:
            return "enum decleartion";
        default:
            return "error";
    }
}
*/
/*
bool isEnumParamVectorLegal(vector<string> params_types_or_enum_values, vector<int> params_lineno){

    return true;
}
*/


void printEntry(Table* symbol_table,string entry_name){
    frameEntry * ptr= symbol_table->returnEntryByName(entry_name);
    if(ptr == nullptr){
        cout << "Can't find entry: '"<<entry_name<< "'." << endl;
        return;
    }
    cout << "--- Found Entry! ---"<< endl;
    cout << "This entry contains " << symbol_table->entryTypeToString((ptr)->entry_type) << ": ";
    cout << "name: " << (ptr)->entry_name << "  |  ";
    if((ptr)->entry_type == FUNC){
        cout << "RetType: " << (ptr)->type_name << "  | input parameters: (";
        for(auto k = (ptr)->params_types_or_enum_values.begin(); k != (ptr)->params_types_or_enum_values.end(); ++k){
            cout << " " << (*k) ;
        }
        cout << " )" <<endl;

    } else {
        cout << "type: " << (ptr)->type_name << "  |  ";
        cout << "offset: " << (ptr)->offset << "\n";
    }
}

/*
 * The function checks if 'call f(list of parameters) is legal by checking if the parameters matches.
 */
bool isParametersMatch(Table* symbol_table,string func_name, vector<string> parameters_types){
    vector<string> parameters_excepted = symbol_table->returnEntryByName(func_name)->params_types_or_enum_values;
    return parameters_excepted == parameters_types;
}

/*
 * The function recives function name and returns vector of parameters types
 */
vector<string> returnVectorOfParameters(Table* symbol_table,string func_name){
    vector<string> parameters_excepted = symbol_table->returnEntryByName(func_name)->params_types_or_enum_values;
    return parameters_excepted;
}

bool isAssignLegal(Table* symbol_table,string type_l,string type_r){
    return type_l == type_r || (type_l == "INT" && type_r == "BYTE");
}


void PushNewScope(Table * symbol_table){
    symbol_table->pushFrame();
}

void PopCurrentScope(Table * symbol_table){
    outputEndOfScope(symbol_table);
    symbol_table->popFrame();

}

void parserErrorUndefEnum(int lineno, const string& id) {
    cout << "line " << lineno << ": " << id << " is not defined" << endl;
//    cout << "line " << lineno << ":" << " enum " << id << " is not defined" << endl;
    exit(0);
}

void checkEnumDef(Table * symbol_table, int l, const string& s ){
    if(  symbol_table->isEntryExists(s)){
        errorDef(l,s);
    }
}

int stringToNum(const char* str){
    int res=0;
    while(*str != 0){
        res= res*10 + (*str - '0');
        str++;
    }
    return res;
}

void checkByteSize(string num, int lineno){
    int n = stringToNum(num.c_str());
    if( n > 255){
        errorByteTooLarge(lineno,num);
    }
}

//input enum_type = "enum "+id_string
void checkEnumTypeUndef(Table* symbol_table,string enum_type,int lineno){
    if( ! symbol_table->isEnumExists(enum_type)){
        parserErrorUndefEnum(lineno,enum_type);
    }
}


/*void checkEnumValLegal(Table* symbols_table, string enum_type, string enum_val, int lineno){
if($4.type != "ID" || !checkEnumValExist(symbols_table, $1.type ,$4.name)){
//Enumerator can only come from ID.
errorMismatch($4.lineno);
}
}*/

void checkBool(string b, int lineno){
    if(b != "BOOL"){
        errorMismatch(lineno);
    }
}
void checkBoolOfTwo(string b1, int lineno1, string b2, int lineno2){
    checkBool(b1,lineno1);
    checkBool(b2,lineno2);
}

void checkIdExist(Table* symbols_table,string id_name, int lineno){
    if( !symbols_table->isEntryExists(id_name)){
        errorUndef(lineno,id_name);
    }
    if(symbols_table->isEntryExists(id_name)){
        if(symbols_table->getEntryEType(id_name) == ENUM_DEC){
            errorUndef(lineno,id_name);
        }
    }
}

//we assume the entry exists.
string getEntryType(Table* symbols_table,string id_name){
    return symbols_table->getEntryTypeName(id_name);
}

void checkEnumAssign(Table* symbol_table, string enum_type,string name, string id_or_enum_val,int lineno){
    //string id_or_enum_val_type_name = symbol_table->getEntryTypeName(id_name);
    //cout << enum_type << " | " << id_or_enum_val_type_name << " | " << id_or_enum_val << "\n";
    if(enum_type != id_or_enum_val){
        errorUndefEnumValue(lineno,name);
    }
}

void checkAssign(Table* symbol_table, string name_type, string id_or_enum_val,int lineno){
    bool byte2Int = (name_type == "INT" && id_or_enum_val == "BYTE");
    bool sameType = (name_type == id_or_enum_val);


    bool ok = byte2Int || sameType;

    //cout << "this is name type: "<<name_type<<" id_or_enum: "<< id_or_enum_val  <<endl;
    if(!ok){
        errorMismatch(lineno);
    }

}


//assume id_name is in the table
void checkAssignID(Table* symbol_table, string id_name, string id_or_enum_val,int lineno){
    if(symbol_table->getEntryEType(id_name)==FUNC){
        output::errorUndef(lineno,id_name);
    }
    checkAssign(symbol_table, symbol_table->getEntryTypeName(id_name),id_or_enum_val,lineno);
}



//assume input is legal.
 string getBiggerNumTypeGroupName( string name_type_group1, string name_type_group2){
    if(name_type_group1 == "INT"  || name_type_group2 == "INT"){
        return "INT";
    }
    return "BYTE";
}

void checkNumType( string name_type_group, int lineno){
    //cout <<"this is the name_type_group: "<< name_type_group << endl;

    if(name_type_group != "INT" && name_type_group != "BYTE"){
        errorMismatch(lineno);
    }
}

void checkNumTypeOfTwo( string name_type_group1,int lineno1, string name_type_group2,int lineno2){
    checkNumType(name_type_group1, lineno1);
    checkNumType(name_type_group2, lineno2);
}

void checkEnumCastingToInt(Table* symbol_table,string type, string enum_id, int lineno){
    E_TYPE eType  = symbol_table->getEntryEType(enum_id);
    //string type_name = symbol_table->getEntryTypeName(enum_id);
     //cout << "a1:" << eType << "\n";
    if((eType != ENUM_VAR && eType != ENUM_VALUE && eType != ENUM_PARAM)  || type != "INT"){
        errorMismatch(lineno);
    }
}

// PushParams(Table* symbol_table,vector<string> params_types,vector<string> params_names, vector<int> params_lineno)
void verifyMainExists(Table* symbol_table){
    if (!symbol_table->doesMainExist()){
        errorMainMissing();
        exit(0);
    }
}


void closeTableAndVerifyMain(Table* symbol_table){
    verifyMainExists(symbol_table);
    outputEndOfScope( symbol_table);
}

void verifyBreak(int inside_a_while, int lineno){
    if (!inside_a_while){
        errorUnexpectedBreak(lineno);
        exit(0);
    }
}
void verifyContinue(int inside_a_while, int lineno){
    if (!inside_a_while){
        errorUnexpectedContinue(lineno);
        exit(0);
    }
}
void verifyFuncDefined(Table* symbol_table,string name_func, int lineno){
    if(! checkEntryExists(symbol_table,name_func)){
        errorUndefFunc(lineno,name_func);
    }
    if(checkEntryExists(symbol_table,name_func)){
        if(symbol_table->getEntryEType(name_func) != FUNC){
            errorUndefFunc(lineno,name_func);
        }
    }
}

//we assume it's legal input.
string getFuncRetType(Table* symbol_table,string name_func){
    return symbol_table->getEntryTypeName(name_func);
}
//we assume the func exists.

void verifyFuncCall(Table* symbol_table,vector<string> types_to_check ,string name_func, int lineno){
    frameEntry * ptr= symbol_table->returnEntryByName(name_func);
    assert(ptr->entry_type == FUNC);
    vector<string> exp_params = ptr->params_types_or_enum_values;
    if(exp_params.size() == 0 && types_to_check.size() == 0 ) {return ;}

    int ep_size = exp_params.size();
    int check_types_size  = types_to_check.size();
    bool check_size  = check_types_size == ep_size;
    //cout << "1:" << check_types_size << "\n";
    //cout << " | " << types_to_check.pop_back() << "\n";
    if(! check_size){
        errorPrototypeMismatch(lineno, name_func, exp_params);
    }
    //cout <<"-------------------- checkpoint 1 --------------------"<<endl;
    bool check_assign = true;
    types_to_check = reverseVector(types_to_check);
    for(auto i_exp = exp_params.begin() , i_input = types_to_check.begin(); i_exp != exp_params.end() && i_input != types_to_check.end() && check_assign ; ++i_input,++i_exp){
       check_assign = ((*i_exp == *i_input) || (*i_exp == "INT" && *i_input == "BYTE") ) ;
     //  cout << "2:" << " | " << *i_exp << " | " << *i_input << "\n";
    }

    //bool check1 = exp_params != reverseVector(types_to_check);////////////////?????????????????????
    //bool ok = check_size && (check1 ||check_assign )  ;

    if(!check_assign){

        errorPrototypeMismatch(lineno, name_func, exp_params);
     //  cout <<"-------------------- checkpoint 2 --------------------"<<endl;
    }
    //cout <<"-------------------- checkpoint 3 --------------------"<<endl;
}

void verifyFuncReturn(Table* symbol_table,string current_func_name ,string return_type, int lineno){
    frameEntry * ptr= symbol_table->returnEntryByName(current_func_name);
    string func_type = ptr->type_name;
    //cout << func_type << "\n";
    if(func_type != return_type){
        if(func_type != "INT" || return_type != "BYTE"){
            errorMismatch(lineno);
        }
    }
}

//CodeBuffer * InitNewCodeBuffer(){
//    return new CodeBuffer();
//}
//void DeleteCodeBuffer(CodeBuffer ** pCodeBuffer);


#endif //INC_236360_HW3_PARSER_ACTIONS_CPP
