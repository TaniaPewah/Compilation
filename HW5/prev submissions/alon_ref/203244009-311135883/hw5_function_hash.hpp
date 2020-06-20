#ifndef _236360_5_function_hash_
#define _236360_5_function_hash_

#include <unordered_map> 
#include <stdbool.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>

#include "hw5_bisonStructs.hpp" 
#include "hw5_function.hpp" 

using namespace std;

namespace hw5_stuff {
	
	class function_hash {
		
		public:
		
		unordered_map<string, int>	functions_name2index;
		vector<function> 			functions_data;
				
		
		function_hash() {
			functions_name2index 	= unordered_map<string, int>();
			functions_data 			= vector<function>();
		}		
				
		void add_function(function var) { 
			functions_data.push_back(var); 
			functions_name2index[var.name] = functions_data.size() - 1;
		}
				
		bool exists(string name) { 
			return (functions_name2index.find(name) != functions_name2index.end());
		}
	};
}

#endif