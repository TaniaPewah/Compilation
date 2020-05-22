//
// Created by tt on 5/22/2020.
//

#ifndef HW3_PARSER_HPP
#define HW3_PARSER_HPP
#include "hw3_output.hpp"
using namespace std;

struct Node{
    string name;
    int offset;
    string type;
public:
    Node( string name, int offset, string type ) : name(name), offset(offset), type(type) {}
};

class Num: public Node{
public:
    Num(const string& name, int offset, const string& type ) : Node(name, offset, type) {};
    string prints(){
        cout << "heyyy!" << endl;
    };
};



#define YYSTYPE Node*
#endif //HW3_PARSER_HPP
