#ifndef _236360_5_bisonstruct_
#define _236360_5_bisonstruct_

#include <vector>
#include <string>
#include <stdbool.h>

using namespace std;

namespace hw5_stuff {
	
	typedef enum allowed_types {
		type_int  = 1,
		type_byte = 2,
		type_bool = 3,
		type_enum = 4,
		type_string = 5
	} allowed_types;
			
	typedef enum ret_type {
		ret_type_int  = 1,
		ret_type_byte = 2,
		ret_type_bool = 3,
		ret_type_void = 4
	} ret_type;
	
	struct input_var {
		allowed_types	type;
		char			*name;		
		char 			*enum_name;
	};

	struct exp_element {
		allowed_types	type;
		char			*enum_val;
		bool			init;
		bool			void_res_exp;
		
		// for hw5
		int 			llvm_ptr_id;
		int 			false_list_id;
		int 			true_list_id;
		char			*exp_start_label;
		int				llvm_str_len;
	};

	union allTypes {
		char 			*string;
		int  			number;
		allowed_types	vartype;
		ret_type		rettype;
		input_var		var;
		exp_element		expr;
	};
}

#endif