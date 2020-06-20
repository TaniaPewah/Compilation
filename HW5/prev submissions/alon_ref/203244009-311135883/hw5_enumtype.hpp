#ifndef _236360_5_enumtype_
#define _236360_5_enumtype_

#include <unordered_map> 
#include <stdbool.h>
#include <string>

using namespace std;

namespace hw5_stuff {
	
	class enumtype {
		
		public:
		
		string						name;
		vector<string>				values;
		unordered_map<string, int>	name2indexvalue;

		enumtype() {
			values = vector<string>();
			name2indexvalue = unordered_map<string, int>();
		}
		
		enumtype(string _name) : enumtype() {
			name = _name;
		}
		
		bool valueExist(string val) {
			return (name2indexvalue.find(val) != name2indexvalue.end());
		}
		
		void add(string value) {
			values.push_back(value);
			name2indexvalue[value] = values.size() - 1;
		}
	};
}

#endif