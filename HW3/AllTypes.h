//
// Created by tt on 5/22/2020.
//

#ifndef HW3_ALLTYPES_H
#define HW3_ALLTYPES_H
#include "hw3_output.hpp"

struct Node{
    string name;
    int offset;
    string type;
public:
    Node( const string& name, int offset, const string& type ) : name(name), offset(offset), type(type) {}
    virtual std::string prints();
};

class Num: public Node{
public:
    Num(const string& name, int offset, const string& type ) : Node(name, offset, type) {} {};
    string prints(){
        output::printID(this->name, this->offset, this->type);
    };
};




#endif //HW3_ALLTYPES_H
