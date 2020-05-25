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

//remake num
class Num: public IdNode{
public:
    Num(int lineno, const string name, const string type ) : IdNode(lineno, name) {};
};


#define YYSTYPE Node*
#endif //HW3_PARSER_HPP
