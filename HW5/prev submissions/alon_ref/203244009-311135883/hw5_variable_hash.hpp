#ifndef _236360_5_variable_hash_
#define _236360_5_variable_hash_

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
	
	class variable_hash {
		
		private:
		
		unordered_map<string, int>	variables_name2index;
		vector<variable> 			variables_data;
		vector<int>					variables_inScope;
				
		public:
		
		int offset_pointer;
		
		variable_hash() {
			variables_name2index 	= unordered_map<string, int>();
			variables_data 			= vector<variable>();
			variables_inScope 		= vector<int>();
			offset_pointer			= 1;
		}		
				
		void enter_scope() {
			variables_inScope.push_back(0);
		}
		
		vector<variable> exit_scope() {
			vector<variable> res = vector<variable>();
			
			if (variables_inScope.size() <= 0) return res;

			int amount = variables_inScope[variables_inScope.size()-1];
				
			// Remove the last 	
			for (int i=0; i < amount; i++) {
				res.push_back(variables_data[variables_data.size()-1]);
				variables_name2index.erase(variables_data[variables_data.size()-1].name);
				variables_data.pop_back();
			}
			
			variables_inScope.pop_back();
			
			return res;
		}
		
		void add_var(variable var) { 
			variables_data.push_back(var); 
			variables_name2index[var.name] = variables_data.size() - 1;
			variables_inScope[variables_inScope.size()-1] += 1;
		}
				
		bool exists(string name) { 
			return (variables_name2index.find(name) != variables_name2index.end());
		}
		
		bool initialized(string name) { 
			return (variables_data[variables_name2index[name]].initialized);
		}
		
		void initVariable(string name) { 
			variables_data[variables_name2index[name]].initialized = true;
		}
		
		allowed_types getType(string name) { 
			return (variables_data[variables_name2index[name]].getType());
		}
		
		string getEnumType(string name) { 
			return (variables_data[variables_name2index[name]].getEnumName());
		}
		
		// variable must exist!!
		variable get_variable(string name) {
			return variables_data[variables_name2index[name]];
		}
	};

}

#endif