
#ifndef PARSER_ADAPTER_HPP
#define PARSER_ADAPTER_HPP

#include "parser.hpp"
#include "symbolTable.h"
#include <string>

SymbolTable symbolTable;

IdNode* ruleFuncDecl(IdNode* id_node) {
    string name = id_node->name;
	IdNode* current_node = new IdNode(id_node->lineno, name); 
	symbolTable.addSymbolFunc( current_node );
    delete(id_node);
    return current_node;
}

IdNode* ruleVarDecl(IdNode* id_node) {
    string name = id_node->name;
	IdNode* current_node = new IdNode(id_node->lineno, name); 
	symbolTable.addSymbolVar( current_node );
    delete(id_node);
    return current_node;
}

IdNode* ruleVarDeclAssign(IdNode* id_node, string var_type, string assign_type) {
    string name = id_node->name;
    IdNode* current_node = NULL;

    if(var_type != assign_type ){

        cout << " bad type !! in line: "<< id_node->lineno << endl;

    } else {
	    current_node = new IdNode(id_node->lineno, name); 
	    symbolTable.addSymbolVar( current_node );
    }

    delete(id_node);
    return current_node;
}

ExpNode* ruleExpNum(NumNode* num_node){ 

    return (new ExpNode(num_node->lineno, num_node->type_name));
}


// int stringToBinop(ExpNode* var_a, ExpNode* var_b, string op )
// {
//     int result = 0;
//     int a,b = 0;
//     if (var_a->type == "int" ){
//         a = stoi(var_a->value);
//     } else {
//         cout<< var_a->value << "  " << (var_a->value)[0] << endl;
//         a = var_a->value[0];
//         cout<<"secound variable is of type byte and value is: " << b << endl;
//     }
//      if (var_b->type == "int" ){
//         b = stoi(var_b->value);
//     } else {
//         b = var_b->value[0];
//         cout<<"secound variable is of type byte and value is: " << b << endl;
//     }

//     if(op == "+") result = a + b;
//     else if(op == "-") result = a - b;
//     else if(op == "*") result = a * b;
//     else if(op == "/") result = a / b;

//     return result;
// }

ExpNode* ruleExpBinopExp(ExpNode* exp_a,  BinopNode* binop, ExpNode* exp_b) {
    // Check exp_a & exp_b are num types. If not- raise exaption. Else, do binop and return higher num type

    if((exp_a->type != "int" && exp_a->type != "byte") || (exp_b->type != "int" && exp_b->type != "byte")){
        cout<< " ERROR in line num: "<< binop->lineno << " not numaric type using binop!"<< endl;
        exit(0);
    }
    else{
        if(exp_a->type == "int" || exp_b->type == "int"){
            return new ExpNode(binop->lineno, "int");
        }
        return new ExpNode(binop->lineno, "byte");
    }
    
}

ExpNode* ruleExpNumB(NumNode* num) {
    return new ExpNode(num->lineno, "byte");
}

// TODO: take care of type conversions


#endif //PARSER_ADAPTER_HPP