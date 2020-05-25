
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

IdNode* ruleVarDeclAssign(IdNode* id_node) {
    string name = id_node->name;
	IdNode* current_node = new IdNode(id_node->lineno, name); 
	symbolTable.addSymbolVar( current_node );
    delete(id_node);
    return current_node;
}



#endif //PARSER_ADAPTER_HPP