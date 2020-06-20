//
// Created by hilak on 1/12/2020.
//

#ifndef INC_236360_HW5_TYPEENUMS_HPP
#define INC_236360_HW5_TYPEENUMS_HPP

enum ValBoolTypeEnum { BOOLVAL_FALSE,BOOLVAL_TRUE };

enum opTypeEnum { OP_AND, OP_OR , OP_NOT  , OP_EQU ,
    OP_NEQ , OP_LEFTBIG , OP_RIGHTBIG , OP_LEFTBIGEQ ,
    OP_RIGHTBIGEQ ,
    OP_SUM , OP_SUB , OP_MUL , OP_DIV,OP_NULL};



enum TypeNameEnum { TYPE_NONEXIST  , TYPE_FUNC , TYPE_VOID , TYPE_INT ,
    TYPE_BYTE , TYPE_BOOL , TYPE_STR };

enum cmdTypeEnum {CMD_RETURN , CMD_IF , CMD_ELSE , CMD_WHILE , CMD_BREAK,
    CMD_CONTINUE , CMD_PRECOND , CMD_SECTION , CMD_COMMA , CMD_ASGN};

enum scopeValEnum {SCOPE_LPAREN , SCOPE_RPAREN , SCOPE_LBRACE
    , SCOPE_RBRACE};


#endif //INC_236360_HW5_TYPEENUMS_HPP
