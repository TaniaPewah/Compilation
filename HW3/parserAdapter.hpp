
#ifndef PARSER_ADAPTER_HPP
#define PARSER_ADAPTER_HPP

#include "parser.hpp"
#include "symbolTable.h"

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

VarNode* ruleExpNum(NumNode* num_node){ 
    return (new VarNode(num_node->lineno, num_node->type_name, num_node->type_name, num_node->int_value));
}


#endif //PARSER_ADAPTER_HPP