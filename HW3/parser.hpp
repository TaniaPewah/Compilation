//
// Created by tt on 5/22/2020.
//

#ifndef HW3_PARSER_HPP
#define HW3_PARSER_HPP
#include <iostream>
using namespace std;


class Node{
    /* This class is the basic node with line num. which all other types inherite from */
    public:
    int lineno;
    Node(int lineno) : lineno(lineno) {};
};

class IdNode: public Node{
    /* This class gives basic support all ID's (variable name) */
    public:
    string name;
    IdNode( int lineno, string name ) : Node(lineno), name(name) {
    };
};

class TypeNode : public Node{
    /* This class gives basic support all types */
    public:
    string type_name;
    TypeNode( int lineno, string type ) : Node(lineno), type_name(type) {
    };
};

class NumNode: public TypeNode{
    /* This class will support int variable without a name*/
    public:
    string value;
    NumNode( int lineno, string value, string type ) : TypeNode(lineno, type), value(value) {
    };
};

class ExpNode: public Node{
    /* This class supports the rule expressions, need further checks on CALL methods*/
    public:
    string type;
    ExpNode( int lineno, string type ) : Node(lineno), type(type) {
    };
};

class VarNode: public IdNode{
    /* This class supports variable types, such as int, bool... */
    public:
    string type;
    VarNode( int lineno, string name, string type ) : IdNode(lineno, name), type(type) {
    };
};

class BinopNode: public Node{
    /* This class supports Binop operataros , such as +, - ... */
    public:
    string binop;
    BinopNode( int lineno, string binop ) : Node(lineno), binop(binop){
    };
};



#define YYSTYPE Node*
#endif //HW3_PARSER_HPP
