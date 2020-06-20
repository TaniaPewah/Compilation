//
// Created by hilak on 1/12/2020.
//

//#include "aidFunction.hpp"

#ifndef INC_236360_HW5_NODE_HPP
#define INC_236360_HW5_NODE_HPP

#include "typeEnums.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <cmath>
//#include "aidFunction.hpp"

using namespace std;

class Node {
public:
    Node(){}
    virtual ~Node() {}
};


class Type : public Node {
public:
    TypeNameEnum name;
    Type(TypeNameEnum _typeName) : name(_typeName) {}
};

class BinaryOp : public Node {
public:
    opTypeEnum type;

    BinaryOp( BinaryOp &binaryOp2){  this->type =  binaryOp2.getType();}
    BinaryOp():type(OP_NULL){}
    BinaryOp(opTypeEnum _opType) : type(_opType) {}
    BinaryOp(char * _opType) {

        std::string strType(_opType);
        if(strType == "+"){
            type = OP_SUM;

        }
        else if(strType == "-"){
            type = OP_SUB;
        }
        else if(strType == "*"){
            type = OP_MUL;
        }
        else if(strType == "/"){
            type = OP_DIV;
        }
        else if(strType == "and"){
            type = OP_AND;
        }
        else if(strType == "or"){
            type = OP_OR;
        }
        else if(strType == "not"){
            type = OP_NOT;
        }
    }

    opTypeEnum getType(){return type;}
    void setType(char * _opType){
        std::string strType(_opType);

        if(strType == "+"){
            type = OP_SUM;
            //printf("+");
        }
        else if(strType == "-"){
            type = OP_SUB;
        }
        else if(strType == "*"){
            type = OP_MUL;
        }
        else if(strType == "/"){
            type = OP_DIV;
        }
        else if(strType == "and"){
            type = OP_AND;
        }
        else if(strType == "or"){
            type = OP_OR;
        }
        else if(strType == "not"){
            type = OP_NOT;
        }
    }
};

class RelativeOp : public Node {
    opTypeEnum type;
public:
    RelativeOp():type(OP_NULL){}
    RelativeOp(opTypeEnum _opType) : type(_opType) {}
    RelativeOp(char * _opType) {
        std::string strType(_opType);
        if(strType == "=="){
            type = OP_EQU;
        }
        else if(strType == "!="){
            type = OP_NEQ;
        }
        else if(strType == "!="){
            type = OP_NEQ;
        }
        else if(strType == "<"){
            type = OP_RIGHTBIG;
        }
        else if(strType == ">"){
            type = OP_LEFTBIG;
        }
        else if(strType == "<="){
            type = OP_RIGHTBIGEQ;
        }
        else if(strType == ">="){
            type = OP_LEFTBIGEQ;
        }
    }
    opTypeEnum getType(){return type;}
};

class BoolVal : public Node {
public:
    ValBoolTypeEnum type;

    BoolVal():type(BOOLVAL_FALSE){}
    BoolVal(ValBoolTypeEnum _ValboolType) : type(_ValboolType) {}
};


class CmdWord : public Node {
    cmdTypeEnum type;
public:
    CmdWord(cmdTypeEnum _cmdType) : type(_cmdType)  {}
};

class ScopeVal : public Node {
    scopeValEnum type;
public:
    ScopeVal(scopeValEnum _scopeType) : type(_scopeType) {}

};


class NumVal : public Node {
public:
    int val;
    std::string str;
    int CharPointerToInt(const char *str) {
        int i = 0;
        int val = 0;
        while (str[i] != '\0') {
            val *= 10;
            val += (int) (str[i] - '0');
            i++;
        }
        return val;
    }

    NumVal():val(0){}
    NumVal(const char * strNum) : val(CharPointerToInt(strNum)) , str(strNum) {}
    int getVal() {return val;}
    std::string getStr() {return str;}
    void setVal_int(int v){val = v;}
    void setVal_char(const char * strNum){val = CharPointerToInt(strNum);}
};

class IntVal : public NumVal{
public:
    IntVal(char * strNum) :NumVal(strNum){}
    IntVal():NumVal(){}
};

class ByteVal : public NumVal{
public:
    ByteVal(char * strNum) :NumVal(strNum){}
    ByteVal():NumVal(){}
};

class IdVal : public Node {
public:
    std::string IdStr;
    IdVal():IdStr(""){}

    IdVal(char * strId) : IdStr(strId) {}
    std::string GetVal() {return IdStr;}
};

class StrVal : public Node {
public:
    std::string str;
    StrVal():str(""){}
    StrVal(char * strVal) : str(strVal) {}
    std::string GetStr() {return str;}
};




#endif //INC_236360_HW5_NODE_HPP
