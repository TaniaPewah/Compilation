#ifndef _236360_5_func_hash_
#define _236360_5_func_hash_

#include <unordered_map> 
#include <stdbool.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>

#include "hw5_bisonStructs.hpp" 
#include "hw5_variable.hpp" 

using namespace std;

namespace hw5_stuff {
	
	class function {
		
		public:
		
		string						name;
		ret_type 					return_type;
		vector<variable>			args;
		int 						curr_arg_offset;
				
		function() {
			args = vector<variable>();
			this->curr_arg_offset = -1;
		}

		function(string _name) : function() {
			this->name = _name;
		}
		
		int add_func_arg(variable var){
			var.offset = curr_arg_offset;
			this->args.push_back(var);
			this->curr_arg_offset--;
			return var.offset;
		}
						
		string get_terType_string() {
			if (return_type == ret_type_int) return "INT";
			if (return_type == ret_type_byte) return "BYTE";
			if (return_type == ret_type_bool) return "BOOL";
			else /*(return_type == ret_type_void)*/ return "VOID";
		}
		
		vector<string> get_argVec_string() {
			vector<string> res = vector<string>();
			for (int i=0; i<args.size(); i++) res.push_back(args[i].getTypeString());
			return res;
		}
	};
}

#endif	