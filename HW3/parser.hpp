//
// Created by tt on 5/22/2020.
//

#ifndef HW3_PARSER_HPP
#define HW3_PARSER_HPP
#include <iostream>
using namespace std;


class Node{
    public:
    int lineno;
    Node(int lineno) : lineno(lineno) {};
};

class IdNode: public Node{
    public:
    string name;
    IdNode( int lineno, string name ) : Node(lineno), name(name) {
    };
};

class TypeNode : public Node{
    public:
    string type_name;
    TypeNode( int lineno, string name ) : Node(lineno), type_name(name) {
    };
};

class NumNode: public TypeNode{
    public:
    string int_value;
    NumNode( int lineno, string value, string type ) : TypeNode(lineno, type), int_value(value) {
    };
};

class StringNode: public Node{
    public:
    string string_value;
    StringNode( int lineno, string value ) : Node(lineno), string_value(value) {
    };
};

class ExpNode: public Node{
    public:
    string type;
    string value;
    ExpNode( int lineno,  string type, string value ) : Node(lineno), type(type), value(value) {
    };
};

class VarNode: public IdNode{
    public:
    string type;
    string value;
    VarNode( int lineno, string name, string type, string value ) : IdNode(lineno, name), type(type), value(value) {
    };
};



#define YYSTYPE Node*
#endif //HW3_PARSER_HPP
