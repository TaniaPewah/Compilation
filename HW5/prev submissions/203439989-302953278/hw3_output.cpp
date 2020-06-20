//
// Created by hilak on 12/8/2019.
//
#include <iostream>
#include "hw3_output.hpp"
#include <sstream>
#include "Table.hpp"
using namespace std;

void output::endScope(){
  //  cout << "---end scope---" << endl;

}

void output::printID(const string& id, int offset, const string& type) {
    //cout << id << " " << type <<  " " << offset <<  endl;
}

string typeListToString(const std::vector<string>& argTypes) {
    stringstream res;
    res << "(";
    for(int i = 0; i < argTypes.size(); ++i) {
        res << argTypes[i];
        if (i + 1 < argTypes.size())
            res << ",";
    }
    res << ")";
    return res.str();
}

string valueListsToString(const std::vector<string>& values) {
    stringstream res;
    res << "{";
    for(int i = 0; i < values.size(); ++i) {
        res << values[i];
        if (i + 1 < values.size())
            res << ",";
    }
    res << "}";
    return res.str();
}

const string output::rules[] = {
        //git
        "Program -> Enums Funcs",// rule 1
        "Funcs -> epsilon", // rule 2
        "Funcs -> FuncDecl Funcs", // rule 3
        "FuncDecl -> RetType ID LPAREN Formals RPAREN LBRACE Statements RBRACE", // rule 4
        "Enums -> epsilon", // rule 5
        "Enums -> EnumDecl Enums", // rule 6
        "EnumDecl -> ENUM ID LBRACE EnumeratorList RBRACE SC", // rule 7
        "RetType -> Type", // rule 8
        "RetType ->  VOID", // rule 9
        "Formals -> epsilon", // rule 10
        "Formals -> FormalsList", // rule 11
        "FormalsList -> FormalDecl", // rule 12
        "FormalsList -> FormalDecl COMMA FormalsList", // rule 13
        "FormalDecl -> Type ID", // rule 14
        "FormalDecl -> EnumType ID", // rule 15
        "EnumeratorList -> Enumerator", // rule 16
        "EnumeratorList -> EnumeratorList COMMA Enumerator", // rule 17
        "Enumerator -> ID", // rule 18
        "Statements -> Statement", // rule 19
        "Statements -> Statements Statement", // rule 20
        "Statement -> LBRACE Statements RBRACE", // rule 21 //scope
        "Statement -> Type ID SC", // rule 22
        "Statement -> EnumType ID SC", // rule 23
        "Statement -> EnumDecl", // rule 24
        "Statement -> Type ID ASSIGN Exp SC", // rule 25
        "Statement -> EnumType ID ASSIGN Exp SC", // rule 26
        "Statement -> ID ASSIGN Exp SC", // rule 27
        "Statement -> Call SC", // rule 28
        "Statement -> RETURN SC", // rule 29
        "Statement -> RETURN Exp SC", // rule 30
        "Statement -> IF LPAREN Exp RPAREN Statement", // rule 31
        "Statement -> IF LPAREN Exp RPAREN Statement ELSE Statement", // rule 32
        "Statement -> WHILE LPAREN Exp RPAREN Statement", // rule 33
        "Statement -> BREAK SC", // rule 34
        "Statement -> CONTINUE SC", // rule 35
        "Call -> ID LPAREN ExpList RPAREN", // rule 36
        "Call -> ID LPAREN RPAREN", // rule 37
        "ExpList -> Exp", // rule 38
        "ExpList -> Exp COMMA ExpList", // rule 39
        "Type -> INT", // rule 40
        "Type -> BYTE", // rule 41
        "Type -> BOOL", // rule 42
        "EnumType -> ENUM ID", // rule 43
        "Exp -> LPAREN Exp RPAREN", // rule 44
        //"Exp -> Exp MULDIV Exp", // rule 45
        "Exp -> Exp BINOP Exp", // rule 45
        "Exp -> ID", // rule 46
        "Exp -> Call", // rule 47
        "Exp -> NUM", // rule 48
        "Exp -> NUM B", // rule 49
        "Exp -> STRING", // rule 50
        "Exp -> TRUE", // rule 51
        "Exp -> FALSE", // rule 52
        "Exp -> NOT Exp", // rule 53
        "Exp -> Exp AND Exp", // rule 54
        "Exp -> Exp OR Exp", // rule 55
        "Exp -> Exp RELOP Exp", // rule 56
        "Exp -> LPAREN Type RPAREN Exp", // rule 57
        "Exp -> Exp ADDSUB Exp", // rule 58
        "Exp -> Exp RELOP Exp" // rule 59
};
void output::printProductionRule(int ruleno) {
    cout << ruleno << ": " << output::rules[ruleno-1] << endl;
}

string output::makeFunctionType(const string& retType, std::vector<string>& argTypes)
{
    stringstream res;
    res << typeListToString(argTypes) << "->" << retType;
    return res.str();
}

void output::printEnumType(const string& name, std::vector<string>& values)
{
  //  cout << "enum " << name << " " << valueListsToString(values) <<  endl;
}

void output::errorLex(int lineno){
    cout << "line " << lineno << ":" << " lexical error" << endl;
    exit(0);
}

void output::errorSyn(int lineno){
    cout << "line " << lineno << ":" << " syntax error" << endl;
    exit(0);
}

void output::errorUndef(int lineno, const string& id){
    cout << "line " << lineno << ":" << " variable " << id << " is not defined" << endl;
    exit(0);
}

void output::errorDef(int lineno, const string& id){
    cout << "line " << lineno << ":" << " identifier " << id << " is already defined" << endl;
    exit(0);
}

void output::errorUndefFunc(int lineno, const string& id) {
    cout << "line " << lineno << ":" << " function " << id << " is not defined" << endl;
    exit(0);
}

void output::errorUndefEnum(int lineno, const string& id) {
    cout << "line " << lineno << ":" << " enum " << id << " is not defined" << endl;
    exit(0);
}

void output::errorUndefEnumValue(int lineno, const string& id) {
    cout << "line " << lineno << ":" << " assignment of undefined value to " << id << endl;
    exit(0);
}

void output::errorMismatch(int lineno){
    cout << "line " << lineno << ":" << " type mismatch" << endl;
    exit(0);
}

void output::errorPrototypeMismatch(int lineno, const string& id, std::vector<string>& argTypes) {
    cout << "line " << lineno << ": prototype mismatch, function " << id << " expects arguments " << typeListToString(argTypes) << endl;
    exit(0);
}

void output::errorUnexpectedBreak(int lineno) {
    cout << "line " << lineno << ":" << " unexpected break statement" << endl;
    exit(0);
}

void output::errorUnexpectedContinue(int lineno) {
    cout << "line " << lineno << ":" << " unexpected continue statement" << endl;
    exit(0);
}

void output::errorMainMissing() {
    cout << "Program has no 'void main()' function" << endl;
    exit(0);
}

void output::errorByteTooLarge(int lineno, const string& value) {
    cout << "line " << lineno << ": byte value " << value << " out of range"<< endl;
    exit(0);
}

