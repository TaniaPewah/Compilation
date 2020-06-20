#ifndef _236360_5_CC_
#define _236360_5_CC_

#include "hw5_variable_hash.hpp" 
#include "hw5_enumtype_hash.hpp"
#include "hw5_function_hash.hpp"
#include "bp.hpp"

using namespace std;

namespace hw5_stuff {
	
	class CC {
			
		public:
		
		function_hash functions;
		enumtype_hash enums;	
		variable_hash variables;	
			
		string currently_defined_element;
		string currently_function_scope;
		int loop_depth;
		
		vector<function> call_func_stack;
		vector<int>      call_arg_pointer_stack;
		
		// for hw 5
		int curr_reg;
		int curr_string;		
		unordered_map<int, vector<pair<int,BranchLabelIndex>>>	label_list;
		int label_list_key_gen;
		vector<vector<pair<int,BranchLabelIndex>>> break_bp;
		vector<vector<pair<int,BranchLabelIndex>>> continue_bp;
		vector<vector<exp_element>> func_args;
		bool last_emit_was_label;
		
		private:
		
        CC() { 
			
			functions = function_hash();
			enums	  = enumtype_hash();
			variables = variable_hash();
			
			call_func_stack = vector<function>();
			call_arg_pointer_stack = vector<int>();
			
			currently_defined_element = "";
			currently_function_scope = "";
			loop_depth = 0;
			
			// HW5
			curr_reg = 0;			
			curr_string = 0;
			label_list = unordered_map<int, vector<pair<int,BranchLabelIndex>>>();
			label_list_key_gen = 0;
			break_bp = vector<vector<pair<int,BranchLabelIndex>>>();
			continue_bp = vector<vector<pair<int,BranchLabelIndex>>>();
			func_args = vector<vector<exp_element>>();
			last_emit_was_label = false;
		}
		
		public:
		
		static CC& getInstance() {
			static CC _instance;
			return _instance;
		}
		
		void add_print() {
			function print = function("print");
			print.return_type = ret_type_void;
			variable print_arg = variable(-1, "print_arg", type_string);
			print.add_func_arg(print_arg);
			functions.add_function(print);
		}
		
		void add_printi() {
			function printi = function("printi");
			printi.return_type = ret_type_void;
			variable printi_arg = variable(-1, "printi_arg", type_int);
			printi.add_func_arg(printi_arg);	
			functions.add_function(printi);
		}
	};
}

#endif