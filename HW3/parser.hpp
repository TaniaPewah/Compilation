//
// Created by tt on 5/22/2020.
//

#ifndef HW3_PARSER_HPP
#define HW3_PARSER_HPP
#include <iostream>
using namespace std;

struct Node{
    string name;
    string type;
public:
    Node( string name, string type ) : name(name), type(type) {
        cout << "node created ~~~~~~~~~~~~~~~~~~~~~" << name << endl;
    }
};

class Num: public Node{
public:
    Num(const string& name, const string& type ) : Node(name, type) {};
};


#define YYSTYPE Node*
#endif //HW3_PARSER_HPP
