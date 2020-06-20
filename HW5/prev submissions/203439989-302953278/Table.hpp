//
// Created by hilak on 12/13/2019.
//

#ifndef INC_236360_HW3_TABLE_HPP
#define INC_236360_HW3_TABLE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <string>
#include <iomanip>
//#include "parser"

using namespace std;
#define ERROR (-1)

typedef enum{
    VAR,
    FUNC,
    PARAM,
    ENUM_VAR,
    ENUM_VALUE,
    ENULL,
    ENUM_DEC,
    ENUM_PARAM
} E_TYPE;

struct frameEntry{
    string entry_name;
    string type_name;//for function it's return type.
    vector<string> params_types_or_enum_values;//in case of function it's the parameters types, otherwise it's empty vector
    int offset;
   // int entry_type;//what the entry contains:var,func or func's parameter
    E_TYPE entry_type = ENULL;
    string value;//In case of this being ENUM_VAR or VAR
    string globalNum;
    int index_of_enum_value = 0;           //example: enum day {sunday, monday} sunday is 0, monday is 1.

};

struct frame{
    vector<frameEntry*> entries;
    int offset_frame=0;
};


template <class T>
vector<T> reverseVectorTable(vector<T> v){
    vector<T> r ;
    for (auto i = v.end()-1; i >= v.begin() ; --i) {
        r.push_back(*i);
    }
    return r;
}


class Table{
public:
    vector<frame*> frames;
    vector<int> offsets;


    Table(){}
    ~Table(){}

    /* adds new frame to the table. if the frame is the first one, then add 0 to the offset stack.
     * otherwise, copies the offset from the head of the stack.
    */
    void pushFrame(){
        frame* f = new frame;

       if(this->frames.size()> 0){
           f->offset_frame= this->frames.back()->offset_frame;
       }

        this->frames.push_back(f);

        int offset_to_push = 0;
        if(this->offsets.size() != 0){
            offset_to_push = this->offsets.back();
        }
        this->offsets.push_back(offset_to_push);
    }
    void popFrame(){
        //printTable();
        for(auto i = this->frames.back()->entries.begin(); i != this->frames.back()->entries.end(); ++i){
            delete (*i);
        }
        //printTable();
        delete this->frames.back();
        this->frames.pop_back();
        this->offsets.pop_back();
        //printTable();
    }

    /* adds new entry to the frame that in the head of the stack.
     * takes the offset from the head of the stack and incrementes the head of the stack by 1.
     * Notice to enter negative offset (=-1), when it's a parameter
    */
    void addEntryToFrame(string e_name,string t_name,vector<string> params_types,E_TYPE entry_type,int offset=0){
        frameEntry* fe = new frameEntry;
        fe->entry_name = e_name;
        fe->type_name = t_name;
        fe->entry_type = entry_type;
        fe->params_types_or_enum_values = params_types;//empty unless the new entry is function or enum

        int offset_current_frame = this->frames.back()->offset_frame;


        if(entry_type == VAR){
            //fe->entry_type = VAR;
            fe->offset = this->offsets.back();
            this->offsets.back()++;
            fe->offset = this->frames.back()->offset_frame;
            this->frames.back()->offset_frame++;

        }else if(entry_type == FUNC){
            //fe->entry_type = FUNC;
            fe->offset = 0;//if the entry is function then offset not important
        }else if(entry_type == PARAM || entry_type == ENUM_PARAM){
         //   fe->entry_type = PARAM;
            fe->offset = offset;
        }
        else if(entry_type == ENUM_VAR){
           // fe->entry_type = ENUM_VAR;
            fe->offset = offset;
            fe->offset = this->frames.back()->offset_frame;
            this->frames.back()->offset_frame++;
        }
        else if(entry_type == ENUM_DEC){
          //  fe->entry_type = ENUM_DEC;
            fe->offset = 0;//if the entry is enum declaration  then offset not important
        }else if(entry_type == ENUM_VALUE){
            fe->offset = 0;
        }

        this->frames.back()->entries.push_back(fe);
    }

    /* checks if the entry exists in the table. goes from the last frame that was added to the first one.
        * inside frame goes from the first antry that was added to the last one.
        */
    bool isEntryExists(string e_name){
        for(auto i = this->frames.rbegin(); i != this->frames.rend(); ++i){
            for(auto j = (*i)->entries.begin(); j != (*i)->entries.end(); ++j){

                if((*j)->entry_name == e_name  ){
                    return true;
                }
            }
        }
        return false;
    }
    //input enum_type suppose to be "enum <id>"
    bool isEnumExists(string enum_type){
        for(auto i = this->frames.rbegin(); i != this->frames.rend(); ++i){
            for(auto j = (*i)->entries.begin(); j != (*i)->entries.end(); ++j){
                if((*j)->type_name == enum_type  ){
                    return true;
                }
            }
        }
        return false;
    }

    //returns "" if the entry with e_name doesn't exists.
    string getEntryTypeName(string e_name){
        frameEntry * ptr = this->returnEntryByName(e_name);
        if(ptr == nullptr) {
            return "";
        }
        return ptr->type_name;
    }
    E_TYPE getEntryEType(string e_name){
        frameEntry * ptr = this->returnEntryByName(e_name);
        if(ptr == nullptr) {
            return ENULL;
        }
        return ptr->entry_type;
    }

    string entryTypeToString(int entry_type){
        switch(entry_type){
            case VAR:
                return "variable";
            case FUNC:
                return "function";
            case PARAM:
                return "parameter";
            case ENUM_VAR:
                return "enum var";
            case ENUM_VALUE:
                return "enum value";
            case ENUM_DEC:
                return "enum declaration";
            case ENULL:
                return "e null";
            default:
                return "error";
        }}

    void printTable(){
        cout << "----------------------------------- print_table ----------------------------------- " << endl;
        int frame_num = this->frames.size();
        int param_num = 1;
        for(auto i = this->frames.rbegin(); i != this->frames.rend(); ++i,frame_num--){
            cout << endl <<"frame number: "<< frame_num;
            if(frame_num == 1)   cout << " ---  Global Frame --- " <<endl;
            else   cout <<"\n";

            for(auto j = (*i)->entries.begin(); j != (*i)->entries.end(); ++j){
                cout << "This entry contains " << entryTypeToString((*j)->entry_type) << ": ";
                cout << "name: " << (*j)->entry_name << "  |  ";
                if((*j)->entry_type == ENUM_DEC){
                    cout << "enum type:  " << (*j)->type_name << "  | enum val: ( ";
                    for(auto k = (*j)->params_types_or_enum_values.begin(); k != (*j)->params_types_or_enum_values.end(); ++k){
                        cout <<  (*k) ;

                        if(k < (*j)->params_types_or_enum_values.end() -1 ){
                            cout <<", ";
                        }
                    }
                    cout << " )" <<endl;
                }else
                if((*j)->entry_type == FUNC){
                    cout << "RetType: " << (*j)->type_name << "  | input parameters: ( ";
                    for(auto k = (*j)->params_types_or_enum_values.begin(); k != (*j)->params_types_or_enum_values.end(); ++k){
                        cout <<  (*k) ;

                        if(k < (*j)->params_types_or_enum_values.end() -1 ){
                            cout <<", ";
                        }
                    }
                    cout << " )" <<endl;

                } else {
                    if((*j)->entry_type == ENUM_VALUE){
                        cout << "enum type:  " << (*j)->type_name << "  |  ";
                        cout << "index: " << (*j)->index_of_enum_value;

                    }else{
                        cout << "type: " << (*j)->type_name << "  |  ";
                    }
                    cout << "offset: " << (*j)->offset << "\n";
                }

            }
        }

        cout <<endl<< "----------------------------------- end print_table ----------------------------------- " << endl;
    }

    

    /* if can't find entry , returns nullptr.
     */
    frameEntry* returnEntryByName(string name){
        for(auto i = this->frames.rbegin(); i != this->frames.rend(); ++i){
            for(auto j = (*i)->entries.begin(); j != (*i)->entries.end(); ++j){
                if((*j)->entry_name == name  ){
                    return (*j);
                }
            }
        }
        return nullptr;
    }

    void setGlobalNameForEntry(string name, string global_num){
        frameEntry* entry = returnEntryByName(name);
        entry->globalNum = global_num;
    }

    string getGlobalName(string name){
        frameEntry* entry = returnEntryByName(name);
        return entry->globalNum;
    }


    void setIndexofEnumValue(string name, int index){
        frameEntry* entry = returnEntryByName(name);
        entry->index_of_enum_value = index;
    }

    int getIndexofEnumValue(string name){
        frameEntry* entry = returnEntryByName(name);
        return entry->index_of_enum_value;
    }


    vector<string>* returnEnumValListEntryByName(string enumType_str){
        for(auto i = this->frames.rbegin(); i != this->frames.rend(); ++i){
            for(auto j = (*i)->entries.begin(); j != (*i)->entries.end(); ++j){
                if( (*j)->entry_type == ENUM_DEC && (*j)->type_name == enumType_str ){
                    return &(*j)->params_types_or_enum_values;
                }
            }
        }
        return nullptr;
    }
    int returnEnumValListSize(string enumTypeStr){
        vector<string>* ptr = (this->returnEnumValListEntryByName(enumTypeStr));
        if(ptr == nullptr){
            return ERROR;
        }
        return (*ptr).size();
    }


    bool doesMainExist(){
        frameEntry* func_to_check = this->returnEntryByName("main");
        bool existFuncWithMainName = func_to_check != nullptr;
        if(! existFuncWithMainName) return false;

        bool retTypeVoid = func_to_check->type_name == "VOID";
        int num_params = func_to_check->params_types_or_enum_values.size();
        bool noParams = num_params == 0;
        bool ok =  retTypeVoid && noParams;
        if(!ok){
            return false;
        }
        return true;
    }
  /*  //we assume that the input is legal.
//    int returnEnumValInInt(string enumTypeStr, string enumVal){
//        vector<string>* ptr = (this->returnEnumValListEntryByName(enumTypeStr));
//        if(ptr == nullptr){
//            return ERROR;
//        }
//        vector<string> vector = *ptr;
//        int enum_int = 0;
//        for(auto i = vector.begin();(enumVal != *i) && i != vector.end(); i++,enum_int++){
//
//        }
//
//        return enum_int;
//    }
*/

    int offset_stack_size(){
        return this->offsets.size();
    }
    int frames_stack_size(){
        return this->frames.size();
    }
    int last_frame_size(){
        return this->frames.back()->entries.size();
    }

    string TypeToLLVM_Table(string type){
        if(type == "INT"){
            return "i32";
        } else if(type == "BOOL"){
            return "i1";
        } else if(type == "BYTE"){
            return "i8";
        } else {return "i32";}
        return type;
    }

    string ParameterListToFuncDecLLVM(const string& func_name){

        vector<string> paramter_list = returnEntryByName(func_name)->params_types_or_enum_values;

        stringstream list;
        list << " ";

            if(paramter_list.size() > 0){
                auto parameter_iterator = paramter_list.begin();
//                for(int i = paramter_list.size() - 1 ;i > 0 ;i--){
                    for(int i = 0 ;i < paramter_list.size() - 1 ;i++){
                list << TypeToLLVM_Table(paramter_list[i]) ;
                    list <<", ";
                }
                list << TypeToLLVM_Table(paramter_list[paramter_list.size() - 1]) << " ";
            }
         return list.str();
    }

   static int offset_parameter_to_llvm(int parameter_offset){
        if(parameter_offset < 0){
            return (parameter_offset + 1)*(-1);
        }
        return 0;
    }

    int getParameterOffset(string var_parameter){
        frameEntry * ptr = returnEntryByName(var_parameter);
        return offset_parameter_to_llvm(ptr->offset);
    }

    vector<string> get_func_parameters_type_in_reverse_order(string func_name){
        vector<string> paramter_list_type = returnEntryByName(func_name)->params_types_or_enum_values;
        return paramter_list_type;
    }

    string getParamName(int llvm_offset) {
        auto top_frame = this->frames.rbegin();
        for(auto j = (*top_frame)->entries.begin(); j != (*top_frame)->entries.end(); ++j){
            if((*j)->offset == ((llvm_offset+1)*(-1))  ){
                return (*j)->entry_name;
            }
        }
    }

    };



#endif //INC_236360_HW3_TABLE_HPP
