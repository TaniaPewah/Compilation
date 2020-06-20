#ifndef _236360_5_objects_
#define _236360_5_objects_

#include <vector>
#include <string>
#include <stdbool.h>

#include "hw5_bisonStructs.hpp" 

using namespace std;

namespace hw5_stuff {
		
	class variable {
		
		private:
		
		allowed_types	type;
		string 			enum_name;
		
		public:
		
		int				offset;
		string			name;
		bool 			initialized;
		
		int 			llvm_ptr_id;

		variable(int _offset, string _name, allowed_types _type) {
			offset = _offset;
			name = _name;
			type = _type;
			enum_name = "";
			initialized = false;
		}
		
		variable(int _offset, string _name, allowed_types _type, bool _initialized) {
			offset = _offset;
			name = _name;
			type = _type;
			enum_name = "";
			initialized = _initialized;
		}
		
		variable(int _offset, string _name, string _enum_name) {
			offset = _offset;
			name = _name;
			type = type_enum;
			enum_name = _enum_name;
			initialized = false;
		}
		
		variable(int _offset, string _name, string _enum_name, bool _initialized) {
			offset = _offset;
			name = _name;
			type = type_enum;
			enum_name = _enum_name;
			initialized = _initialized;
		}

		variable(const variable &var) {
			type 	    = var.type 	     ;
			enum_name   = var.enum_name  ; 
			offset		= var.offset	 ;	
			name		= var.name		 ;
			initialized	= var.initialized;
			llvm_ptr_id = var.llvm_ptr_id;
			
		}
		
		variable() {}
		
		allowed_types getType() {
			return type;
		}						
		
		string getEnumName() {
			return enum_name;
		}					

		string getTypeString() {
			if (type == type_int) return "INT";
			if (type == type_byte) return "BYTE";
			if (type == type_bool) return "BOOL";
			if (type == type_string) return "STRING";
			return "enum " + enum_name;
		}
		
		string getTypeDescString() {
			if (type == type_int) return "integer";
			if (type == type_byte) return "byte";
			if (type == type_bool) return "boolean";
			if (type == type_string) return "string";
			return "enum " + enum_name;
		}
		
	};
}

#endif