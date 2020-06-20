#ifndef _236360_5_enumtype_hash_
#define _236360_5_enumtype_hash_

#include <unordered_map> 
#include <stdbool.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <string>

#include "hw5_bisonStructs.hpp" 
#include "hw5_enumtype.hpp" 

using namespace std;

namespace hw5_stuff {
	
	class enumtype_hash {
		
		public:
		
		unordered_map<string, int>	enumtypes_name2index;
		vector<enumtype> 			enumtypes_data;
		vector<int>					enumtypes_inScope;
		unordered_map<string, int>	enumtypes_value2typeIndex;
				
		
		enumtype_hash() {
			enumtypes_name2index 	= unordered_map<string, int>();
			enumtypes_data 			= vector<enumtype>();
			enumtypes_inScope 		= vector<int>();
			enter_scope();
		}		
				
		void enter_scope() {
			enumtypes_inScope.push_back(0);
		}
		
		vector<enumtype> exit_scope() {
			vector<enumtype> res = vector<enumtype>();
			
			if (enumtypes_inScope.size() <= 0) return res;

			int amount = enumtypes_inScope[enumtypes_inScope.size()-1];
				
			// Remove the last 	
			for (int i=0; i < amount; i++) {
				res.push_back(enumtypes_data[enumtypes_data.size()-1]);
				
				// remove the values of this enum
				for (int j=0; j < enumtypes_data[enumtypes_data.size()-1].values.size(); j++) {
					enumtypes_value2typeIndex.erase(enumtypes_data[enumtypes_data.size()-1].values[j]);
				}
				
				// remove the enum
				enumtypes_name2index.erase(enumtypes_data[enumtypes_data.size()-1].name);		
				enumtypes_data.pop_back();
			}
			
			enumtypes_inScope.pop_back();
			
			return res;
		}
		
		// checks if enum exists
		bool exists(string name) { 
			return (enumtypes_name2index.find(name) != enumtypes_name2index.end());
		}
		
		// adds this enum
		//		enum must not exist
		void add_enum(enumtype var) { 
			enumtypes_data.push_back(var); 
			enumtypes_name2index[var.name] = enumtypes_data.size() - 1;
			enumtypes_inScope[enumtypes_inScope.size()-1] += 1;
		}
		
		// checks if value exists
		bool value_exists(string name) { 
			return (enumtypes_value2typeIndex.find(name) != enumtypes_value2typeIndex.end());
		}

		// adds the value to the given enum
		//		enum must exist
		//		value must not exist
		void add_enum_value(string name, string value) { 
			enumtypes_data[enumtypes_name2index[name]].add(value);
			enumtypes_value2typeIndex[value] = enumtypes_name2index[name];
		}
		
		// you give the value, and get back its enum name
		//		value must exist		
		string type_by_value(string value) {
			return enumtypes_data[enumtypes_value2typeIndex[value]].name;
		}
		
		int enum_value_to_int_value(string value) {		
			if (!value_exists(value)) return 0;
			enumtype var = enumtypes_data[enumtypes_value2typeIndex[value]];
			return distance(var.values.begin(), find(var.values.begin(), var.values.end(), value));
		}
	};
}

#endif