#include "hw5_common.hpp" 
#include "bp.hpp" 

#define VERBOSITY_ON 1
#define VERBOSITY_LABEL_ON 0

// Enum declaration methods   ------------------------------------------------------------------->

void hw5_stuff::enter_enum_decl_scope(string name, int line) {
	CC& C = CC::getInstance();
	if (!CanDefine(name)) {
		output::errorDef(line, name);
		exit(0);
	} else {
		C.currently_defined_element = name;	
		enumtype et = enumtype(name);
		C.enums.add_enum(et);
	}
}

void hw5_stuff::add_enum_value(string value, int line) {
	if (!CanDefine(value)) {
		output::errorDef(line, value);
		exit(0);
	} 
	
	CC& C = CC::getInstance();
	C.enums.add_enum_value(C.currently_defined_element, value);
}
	
void hw5_stuff::exit_enum_decl_scope() {
	CC::getInstance().currently_defined_element = "";	
}

// Enum declaration methods   -------------------------------------------------------------------<

		
// Function methods   --------------------------------------------------------------------------->

void hw5_stuff::enter_function_scope() {
	CC& C = CC::getInstance();
	hw5_stuff::enter_scope();
	C.variables.offset_pointer = 0;
}

void hw5_stuff::new_function(string name, ret_type t, int line) {
	if (!CanDefine(name)) {
		output::errorDef(line, name);
		exit(0);
	} 
	
	CC& C = CC::getInstance();
	function func = function(name);
	func.return_type = t;
	C.functions.add_function(func);	
	C.currently_defined_element = name;
	C.currently_function_scope = name;
	
	C.curr_reg = 0;
}

void hw5_stuff::new_input(allowed_types t, string name, int line) {
	if (!CanDefine(name)) {
		output::errorDef(line, name);
		exit(0);
	} 
	
	CC& C = CC::getInstance();
	variable v = variable(0, name, t, true);
	v.llvm_ptr_id = C.curr_reg++;
	v.offset = C.functions.functions_data[C.functions.functions_name2index[C.currently_defined_element]].add_func_arg(v);
	C.variables.add_var(v);
}

void hw5_stuff::new_input_enum(string name, string enum_name, int line) {
	CC& C = CC::getInstance();
	
	// is this enum defined?
	if (!C.enums.exists(enum_name)) {
		output::errorUndefEnum(line, enum_name);
		exit(0);
	// is this ID already used?
	} else if (!CanDefine(name)) {
		output::errorDef(line, name);
		exit(0);
	} 
	
	// add new variable with the given name
	variable v = variable(0, name, enum_name, true);
	v.llvm_ptr_id = C.curr_reg++;
	v.offset = C.functions.functions_data[C.functions.functions_name2index[C.currently_defined_element]].add_func_arg(v);
	C.variables.add_var(v);	
}

void hw5_stuff::exit_function_scope() {
	CC& C = CC::getInstance();
	
	// Add code to genrate function exit
	ret_type r = C.functions.functions_data[C.functions.functions_name2index[C.currently_function_scope]].return_type;
	
	//if (CC::getInstance().last_emit_was_label) {
		if 		(r == ret_type_void)  push_code("	ret void",  "");
		else       					  push_code("	ret i32 0", "");
	//}
	
	push_code("}",  "");
	push_code("",  "");
	
	exit_scope();
	C.currently_defined_element = "";
	C.currently_function_scope  = "";	
}

void hw5_stuff::check_return_void(int line) {
	CC& C = CC::getInstance();
	if (C.functions.functions_data[C.functions.functions_name2index[C.currently_function_scope]].return_type != ret_type_void) {
		output::errorMismatch(line);
		exit(0);
	}

	emit_return_void();	
}	

void hw5_stuff::check_return(exp_element exp, int line) {
	CC& C = CC::getInstance();
	ret_type ret = C.functions.functions_data[C.functions.functions_name2index[C.currently_function_scope]].return_type;
	allowed_types t = exp.type;
	
	if ((ret == ret_type_int  && t != type_int && t != type_byte) ||
		(ret == ret_type_byte && t != type_byte) ||
		(ret == ret_type_bool && t != type_bool) ||
		(ret == ret_type_void)) {
		output::errorMismatch(line);
		exit(0);
	}
	
	emit_return(exp);	
}

// Function methods   ---------------------------------------------------------------------------<


// Scope methods   ------------------------------------------------------------------------------>
		
void hw5_stuff::enter_scope() {
	CC& C = CC::getInstance();
	C.variables.enter_scope();
	C.enums.enter_scope();	
}

void hw5_stuff::exit_scope() {
	CC& C = CC::getInstance();
	//HW03:output::endScope();
	
	print_and_clear_scope();
}
	
void hw5_stuff::enter_loop() {
	CC::getInstance().loop_depth++;
	CC::getInstance().break_bp.push_back(vector<pair<int,BranchLabelIndex>>());
	CC::getInstance().continue_bp.push_back(vector<pair<int,BranchLabelIndex>>());
}

void hw5_stuff::exit_loop() {
	CC::getInstance().loop_depth--;
}

void hw5_stuff::check_break(int line) { 
	if (CC::getInstance().loop_depth <= 0) {
		output::errorUnexpectedBreak(line);
		exit(0);
	}
}

void hw5_stuff::check_continue(int line) {
	if (CC::getInstance().loop_depth <= 0) {
		output::errorUnexpectedContinue(line);
		exit(0);
	}
}

// Scope methods   ------------------------------------------------------------------------------<


// Add Variable methods   ----------------------------------------------------------------------->

void hw5_stuff::new_variable(allowed_types t, string name, int line, bool assign_default) {
	if (!CanDefine(name)) {
		output::errorDef(line, name);
		exit(0);
	}  
	
	CC& C = CC::getInstance();
	variable v = variable(C.variables.offset_pointer, name, t, false);
	v.llvm_ptr_id = C.curr_reg++;
	C.variables.add_var(v);
	C.variables.offset_pointer++;
			
	// Add code to genrate value
	stringstream gen_command, gen_comment;
  	gen_command << "	%t" << v.llvm_ptr_id << " = getelementptr inbounds [50 x i32], [50 x i32]* %stack, i32 0, i32 " << v.offset;
  	gen_comment << "save a pointer to " << (t == type_bool ? "boolean" : (t == type_int ? "integer" : "byte")) << " variable " << name << " in stack under register %t" << v.llvm_ptr_id << " and always do load / store operations via this register";
  	push_code(gen_command.str(), gen_comment.str());
	
	if (assign_default) {
		// Add code to init value
		stringstream store_command, store_comment;
		store_command << "	store i32 0, i32* %t" << v.llvm_ptr_id;
		store_comment << "set variable " << name << " with default value";
		push_code(store_command.str(), store_comment.str());
	}
}

void hw5_stuff::assign_to_variable(string name, int line, exp_element exp) {
	CC& C = CC::getInstance();
	variable v = C.variables.get_variable(name);
	
	if (exp.void_res_exp || ! ((v.getType() == exp.type) || (v.getType() == type_int && exp.type == type_byte)) ) {
		output::errorMismatch(line);
		exit(0);
	}  
	
	if (v.getType() == type_int ) emit_var_assign_INT32  (v, exp);
	if (v.getType() == type_byte) emit_var_assign_BYTE   (v, exp);
	if (v.getType() == type_bool) emit_var_assign_BOOL   (v, exp);
	if (v.getType() == type_enum) assign_to_enum_variable(v.getEnumName(), v.name, line, exp);
}

void hw5_stuff::new_enum_variable(string type, string name, int line, bool assign_default) {
	CC& C = CC::getInstance();
	
	// is enum type defined?
	if (!C.enums.exists(type)) {
		output::errorUndefEnum(line, name);
		exit(0);
	}  
	
	// can define variable with this name?
	if (!CanDefine(name)) {
		output::errorDef(line, name);
		exit(0);
	}  
		
	variable v = variable(C.variables.offset_pointer, name, type, false);
	v.llvm_ptr_id = C.curr_reg++;
	C.variables.add_var(v);
	C.variables.offset_pointer++;
		
	// Add code to genrate value
	stringstream gen_command, gen_comment;
  	gen_command << "	%t" << v.llvm_ptr_id << " = getelementptr inbounds [50 x i32], [50 x i32]* %stack, i32 0, i32 " << v.offset;
	gen_comment << "save a pointer to enum of type " << type << " variable " << name << " in stack under register %t" << v.llvm_ptr_id << " and always do load / store operations via this register";
  	
  	push_code(gen_command.str(), gen_comment.str());
	
	if (assign_default) {		
		// Add code to init value
		stringstream store_command, store_comment;
		store_command << "	store i32 0, i32* %t" << v.llvm_ptr_id;
		store_comment << "set enum variable " << name << " with default value of 0";
		push_code(store_command.str(), store_comment.str());
	}
}

void hw5_stuff::assign_to_enum_variable(string type, string name, int line, exp_element exp) {
	CC& C = CC::getInstance();
	
	// is enum type defined?
	if (!C.enums.exists(type)) {
		output::errorUndefEnum(line, name);
		exit(0);
	}  

	// if exp.type is not enum or the right enum
	if (exp.void_res_exp || exp.type != type_enum || strcmp(exp.enum_val, type.c_str()) != 0) {
		output::errorUndefEnumValue(line, name);
		exit(0);
	}  
	
	variable v = C.variables.get_variable(name);

	emit_var_assign_ENUM(v, exp);
}
	
// Add Variable methods   -----------------------------------------------------------------------<


// Exp Scope methods   -------------------------------------------------------------------------->

void hw5_stuff::check_exp_is(allowed_types t, exp_element ex, int line) {
	if (ex.type != t) {
		output::errorMismatch(line);
		exit(0);
	}
}

void hw5_stuff::check_byte_range(int val, int line) {
	if (val < 0 || val > 255) {
		string sval = to_string(val);
		output::errorByteTooLarge(line, sval);
		exit(0);
	}
}

void hw5_stuff::check_exp_is_numeric(exp_element ex, int line) {
	if (ex.type != type_byte && ex.type != type_int) {
		output::errorMismatch(line);
		exit(0);
	}
}

void hw5_stuff::check_exp_can_covert(allowed_types t, exp_element ex, int line) {
	if (!(t == type_int && ex.type == type_enum)) {
		output::errorMismatch(line);
		exit(0);
	}
}

void hw5_stuff::check_is_def_enum(string name, int line) {
	CC& C = CC::getInstance();
	if (!C.enums.exists(name)) {
		output::errorUndefEnum(line, name);
		exit(0);
	}
}

void hw5_stuff::check_var_id_is_def(string name, int line) {
	CC& C = CC::getInstance();
	if (!C.variables.exists(name)) {
		output::errorUndef(line, name);
		exit(0);
	}
}

void hw5_stuff::check_var_id_is_not_def(string name, int line) {
	CC& C = CC::getInstance();
	if (!CanDefine(name)) {
		output::errorDef(line, name);
		exit(0);
	}
}
	
hw5_stuff::exp_element hw5_stuff::get_exp_by_id(string name, int line) {
	CC& C = CC::getInstance();
	exp_element el;
	
	if (C.variables.exists(name)) {
		variable v = C.variables.get_variable(name);
		el.enum_val = strdup(v.getEnumName().c_str());
		el.type = v.getType();
		el.init = v.initialized;
		
		// save exr to new register
		el.llvm_ptr_id = C.curr_reg++;
		stringstream load_command, load_comment;
		load_command << "	%t" << el.llvm_ptr_id << " = load i32, i32* %t" << v.llvm_ptr_id;
		load_comment << "load the value of variable " << name << " and save it into register %t" << el.llvm_ptr_id;
		push_code(load_command.str(), load_comment.str());
		
		// used in the old system, not any more
		/*if (el.type == type_byte || el.type == type_bool) {
			int new_id = C.curr_reg++;
			stringstream trunc_command, trunc_comment;
			trunc_command << "	%t" << new_id << " = trunc i32 %t"<< el.llvm_ptr_id <<" to i";
			trunc_command << (el.type == type_byte ? 8 : 1);
			trunc_comment << "this variable " << name << " is a " << (el.type == type_byte ? "byte" : "bool" ) << " - change its length";
			push_code(trunc_command.str(), trunc_comment.str());
			el.llvm_ptr_id = new_id;
		}*/
	} else if (C.enums.value_exists(name)) {
		el.enum_val = strdup(C.enums.type_by_value(name).c_str());
		el.type = type_enum;
		el.init = true;
		
		int numeric_value = C.enums.enum_value_to_int_value(name);
		
		// save exr to new register
		el.llvm_ptr_id = C.curr_reg++;
		stringstream load_command, load_comment;
		load_command << "	%t" << el.llvm_ptr_id << " = add i32 0, " << numeric_value;
		load_comment << "save the enum value of " << name << " which has the numeric value of " << numeric_value;
		push_code(load_command.str(), load_comment.str());		
		
	} else {	
		output::errorUndef(line, name);
		exit(0);
	}
	
	return el;
}

void hw5_stuff::check_assignment(string name, exp_element exp, int line) {
	
	CC& C = CC::getInstance();
	if (!C.variables.exists(name)) {
		output::errorUndef(line, name);
		exit(0);
	}
	
	allowed_types v_type = C.variables.getType(name);
	char *v_enum = strdup(C.variables.getEnumType(name).c_str());
	
		
	if (v_type == type_enum &&
		(exp.void_res_exp || ! (v_type == type_enum && exp.type == type_enum && strcmp(exp.enum_val, v_enum) == 0)) ) {
		output::errorUndefEnumValue(line, name);
		exit(0);
	} 	
		
	if ( v_type != type_enum &&
		(exp.void_res_exp || ! ( (v_type == exp.type) || (v_type == type_int && exp.type == type_byte) )) ) {
		output::errorMismatch(line);
		exit(0);
	}
}

hw5_stuff::allowed_types hw5_stuff::get_type_from_call(ret_type ret, int line) {
	/*if (ret == ret_type_void) {
		output::errorMismatch(line);
		exit(0);		
	}*/
	if (ret == ret_type_int) return type_int;
	if (ret == ret_type_byte) return type_byte;
	/* else (ret == ret_type_byte) or void... but there is nothing to do with void */ return type_bool;
}

void hw5_stuff::check_not_void(exp_element exp, int line) {
	if (exp.void_res_exp) {
		output::errorMismatch(line);
		exit(0);
	}
}

// Exp Scope methods   --------------------------------------------------------------------------<


// Call function methods   ----------------------------------------------------------------------<

void hw5_stuff::check_called_function_exists(string name, int line) {
	CC& C = CC::getInstance();
	
	if (!C.functions.exists(name)) {
		output::errorUndefFunc(line, name);
		exit(0);
	}
	
	C.call_func_stack.push_back(C.functions.functions_data[C.functions.functions_name2index[name]]);
	C.call_arg_pointer_stack.push_back(0);
	vector<exp_element> vec = vector<exp_element>();
	C.func_args.push_back(vec);
}

void hw5_stuff::check_called_function_arg(exp_element exp, int line) {
	CC& C = CC::getInstance();
	function func = C.call_func_stack[C.call_func_stack.size()-1];
	int input_count = C.call_arg_pointer_stack[C.call_arg_pointer_stack.size()-1];
	
	// make sure there arent too many arguments in this call
	if (input_count >= func.args.size()) {
		vector<string> args = func.get_argVec_string();
		output::errorPrototypeMismatch(line, func.name, args);
		exit(0);
	}
	
	allowed_types expected = func.args[input_count].getType();
	char *expected_enum = strdup(func.args[input_count].getEnumName().c_str());
	
	// make sure can assign this arg
	if ( ! ((expected == type_enum && exp.type == type_enum && strcmp(exp.enum_val, expected_enum) == 0) ||
			(expected == exp.type && expected != type_enum) || 
			(expected == type_int && exp.type == type_byte)) ) {
		vector<string> args = func.get_argVec_string();
		output::errorPrototypeMismatch(line, func.name, args);
		exit(0);
	}
	
	// move pointer to next variable
	 C.call_arg_pointer_stack[C.call_arg_pointer_stack.size()-1] += 1;
	 
	 // if its a boolean,  then we need to packpatch the true/false list and insert the value to register with 1 or 0 value
	 // then, we need to replcace the exp from value of bool to value of int
	 if (exp.type == type_bool) {
		exp_element ret;
		ret.init = true;
		ret.type = type_int;
		ret.void_res_exp = false;
		ret.llvm_ptr_id = C.curr_reg++;
		
		// In case of true
		string true_marker = push_label(1);
		CodeBuffer::instance().bpatch(C.label_list[exp.true_list_id], true_marker);
		C.label_list.erase(exp.true_list_id);
		int loc1 = push_code("	br label @","jump to phi from TRUE prespective");
		
		// In case of false
		string false_marker = push_label(2);
		CodeBuffer::instance().bpatch(C.label_list[exp.false_list_id], false_marker);
		C.label_list.erase(exp.false_list_id);
		int loc2 = push_code("	br label @","jump to phi from FALSE prespective");
		
		string finish_marker = push_label(3);
		CodeBuffer::instance().bpatch(CodeBuffer::instance().makelist({loc1, FIRST}), finish_marker);
		CodeBuffer::instance().bpatch(CodeBuffer::instance().makelist({loc2, FIRST}), finish_marker);
		
		// add phi command
		stringstream phi_command, phi_comment;
		phi_command << "	%t" << ret.llvm_ptr_id << " = phi i32 [1, %" << true_marker << "], [0, %" << false_marker << "]";
		phi_comment << "according to the source label, put 1 or 0 in register %t" << ret.llvm_ptr_id;
		push_code(phi_command.str(),phi_comment.str());
		
		 C.func_args[C.func_args.size()-1].push_back(ret);
	 } else {
		 C.func_args[C.func_args.size()-1].push_back(exp);
	 }
}

hw5_stuff::exp_element hw5_stuff::check_called_function_close(int line) {
	CC& C = CC::getInstance();
	function func = C.call_func_stack[C.call_func_stack.size()-1];
	int input_count = C.call_arg_pointer_stack[C.call_arg_pointer_stack.size()-1];
	C.call_func_stack.pop_back();
	C.call_arg_pointer_stack.pop_back();
	
	if (func.args.size() != input_count) {
		vector<string> args = func.get_argVec_string();
		output::errorPrototypeMismatch(line, func.name, args);
		exit(0);
	}
		
	exp_element ret;
	ret.init = true;	
	
	// make a call to this function
	stringstream call_command;
	
	if (func.return_type == ret_type_void) {
		ret.void_res_exp = true;
		
		call_command << "	call void @" << func.name << "(";
	}else {
		ret.void_res_exp = false;
		if (func.return_type == ret_type_bool) {ret.type = type_bool; }
		else if (func.return_type == ret_type_byte) { ret.type = type_byte; }
		else /*if (func.return_type == ret_type_int)*/{ ret.type = type_int; }
		
		ret.llvm_ptr_id = C.curr_reg++;
		
		call_command << "	%t" << ret.llvm_ptr_id << " = call i32 @" << func.name << "(";
	}
	
	vector<exp_element> vec = C.func_args[C.func_args.size()-1];
	C.func_args.pop_back();
	
	if (vec.size() > 0) {
		call_command << get_var_call_string(vec[0]);
		for (int i=1; i < vec.size(); i++) call_command << ", " << get_var_call_string(vec[i]);
	}
	
	call_command << ")";
	
	push_code(call_command.str(), "");
	
	if (func.return_type == ret_type_bool) {
		// do the actual compartion
		stringstream calc_command, calc_comment;
		stringstream comp_command, comp_comment;
		int calc = C.curr_reg++;

		// used to be i1, now i32
		calc_command << "	%t" << calc << " = icmp eq i32 %t" << ret.llvm_ptr_id << ", 1";
		calc_comment << "check the boolean value returned from function " << func.name << ", which is being held inside %t" << calc;
		push_code(calc_command.str(), calc_comment.str());	
		
		comp_command << "	br i1 %t" << calc << ", label @, label @";
		int loc1 = push_code(comp_command.str(), "");
		
		// make a new truelist
		vector<pair<int,BranchLabelIndex>> truelist = CodeBuffer::instance().makelist({loc1,FIRST});
		C.label_list[C.label_list_key_gen] = truelist;
		ret.true_list_id = C.label_list_key_gen++;
		
		// make a new falselist
		vector<pair<int,BranchLabelIndex>> falselist = CodeBuffer::instance().makelist({loc1,SECOND});
		C.label_list[C.label_list_key_gen] = falselist;
		ret.false_list_id = C.label_list_key_gen++;
	}
		
	return ret;
}

string hw5_stuff::get_var_call_string(exp_element exp) {
	// make a call to this function
	stringstream vars;
	
	if (exp.type == type_bool || exp.type == type_int || exp.type == type_byte || exp.type == type_enum) {
		vars << "i32 %t" << exp.llvm_ptr_id;
	} else if (exp.type == type_string) {
		vars << "i8* getelementptr inbounds ([" << exp.llvm_str_len << " x i8], [" << exp.llvm_str_len << " x i8]* @.str_" << exp.llvm_ptr_id << ", i32 0, i32 0)";
	}
 	
	return vars.str();
}

// Call function methods   ---------------------------------------------------------------------->


void hw5_stuff::start_program() {
	CC& C = CC::getInstance();
	
	// add print & printi methods
	C.add_print();
	C.add_printi();
	
	// emit them
	CodeBuffer::instance().emitGlobal("@.int_specifier = constant [4 x i8] c\"%d\\0A\\00\"");
	CodeBuffer::instance().emitGlobal("@.str_specifier = constant [4 x i8] c\"%s\\0A\\00\"");
	CodeBuffer::instance().emitGlobal("@.str_zero_div  = constant [23 x i8] c\"Error division by zero\\00\"");
	push_code("","");
	push_code("declare i32 @printf(i8*, ...)","");
	push_code("declare void @exit(i32)","");
	push_code("","");
	push_code("define void @printi(i32) {","");
	push_code("	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0), i32 %0)","");
	push_code("	ret void","");
	push_code("}","");
	push_code("","");
	push_code("define void @print(i8*) {","");
	push_code("	call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0), i8* %0)","");
	push_code("	ret void","");
	push_code("}","");
	push_code("","");
}

void hw5_stuff::finish_program() {
	CC& C = CC::getInstance();
	
	if (!C.functions.exists("main")) {
		output::errorMainMissing();
		exit(0);
	} 
	
	function main = C.functions.functions_data[C.functions.functions_name2index["main"]];
	
	if (main.return_type != ret_type_void || main.args.size() > 0) {
		output::errorMainMissing();
		exit(0);		
	}
	
	//HW03:output::endScope();
		
	// Print all functions
	for (int i = 0; i < C.functions.functions_data.size(); i++) {
		function curr = C.functions.functions_data[i];
		string retType = curr.get_terType_string();
		vector<string> inputs = curr.get_argVec_string();
		//HW03:output::printID(curr.name, 0, output::makeFunctionType(retType, inputs));
	}
	
	print_and_clear_scope();
	
	CodeBuffer::instance().printGlobalBuffer();
	CodeBuffer::instance().printCodeBuffer();
}


// Program methods   ----------------------------------------------------------------------------<

bool hw5_stuff::CanDefine(string name) {
	CC& C = CC::getInstance();
	if (C.functions.exists(name)	)	return false;
	if (C.enums.exists(name)		)	return false;
	if (C.enums.value_exists(name)	)	return false;
	if (C.variables.exists(name)	)	return false;
	return true;	
}

void hw5_stuff::print_and_clear_scope() {
	CC& C = CC::getInstance();
	
	// get all variables defined in this scope
	vector<variable> vec_var = C.variables.exit_scope();
	
	// print them
	for (int i=vec_var.size()-1; i >= 0; i--) 
		//HW03:output::printID(vec_var[i].name, vec_var[i].offset, vec_var[i].getTypeString()); 
	
	// move back vec_var.size() steps in the variable offset
	C.variables.offset_pointer -= vec_var.size();
	
	// get all enums defined in this scope	
	vector<enumtype> vec_enum = C.enums.exit_scope();
	
	// print them
	//HW03:for (int i=vec_enum.size()-1; i >= 0; i--) 
		//HW03:output::printEnumType(vec_enum[i].name, vec_enum[i].values);

}

// Program methods   ---------------------------------------------------------------------------->


// Code Generation Stuff ------------------------------------------------------------------------<

int hw5_stuff::push_code(string command, string comment) {
	CC::getInstance().last_emit_was_label = false;
#if VERBOSITY_ON == 1
	stringstream command_stream;
	string value = comment.empty() ? "" : "; " + comment;
	if (!command.empty()) command_stream << std::left << std::setw(80) << command;
	command_stream << value;
	return CodeBuffer::instance().emit(command_stream.str());
#else
	return CodeBuffer::instance().emit(command);
#endif
}
	
string hw5_stuff::push_label(int id) {
#if VERBOSITY_LABEL_ON == 1
	stringstream comment;
	comment << "caller id " << id;
	push_code("", comment.str());
#endif
	CC::getInstance().last_emit_was_label = true;
	return CodeBuffer::instance().genLabel();
}
	
	
// emiting value expr

hw5_stuff::exp_element hw5_stuff::emit_exp_INT32(int value) {
	CC& C = CC::getInstance();
	exp_element ret;
	
	ret.type = type_int; 
	ret.init = true; 
	ret.void_res_exp = false;
	
	// add new register, with value of zero
	int new_reg = C.curr_reg++;
		
	// Add code to genrate value
	stringstream gen_command, gen_comment;
	
  	gen_command << "	%t" << new_reg << " = add i32 0, " << value;
  	gen_comment << "define register %t" << new_reg << " to hold a constant 32 bit value (i32) of " << value;
  	push_code(gen_command.str(), gen_comment.str());
	ret.llvm_ptr_id = new_reg;
	
	return ret;
}

hw5_stuff::exp_element hw5_stuff::emit_exp_BYTE(int value, int line) {
	CC& C = CC::getInstance();
	exp_element ret;	
	
	check_byte_range(value, line); 
	ret.type = type_byte; 
	ret.init = true; 
	ret.void_res_exp = false;
	
	int new_reg = C.curr_reg++;
	
	// Add code to genrate value
	stringstream gen_command, gen_comment;
  	gen_command << "	%t" << new_reg << " = add i32 0, " << value;
  	gen_comment << "define register %t" << new_reg << " to hold a constant 8 bit value (but its still an i32 reg) of " << value;
  	push_code(gen_command.str(), gen_comment.str());
	ret.llvm_ptr_id = new_reg;
	
	return ret;
}

hw5_stuff::exp_element hw5_stuff::emit_exp_STRING(string value) {
	CC& C = CC::getInstance();
	exp_element ret;	
	ret.init = true; 
	ret.void_res_exp = false;
	
	ret.type = type_string;	
	ret.llvm_str_len = value.length() - 1;
	ret.llvm_ptr_id = C.curr_string++;
	
	stringstream string_command;
	string_command << "@.str_" << ret.llvm_ptr_id << " = constant [" << ret.llvm_str_len << " x i8] c\"" << value.substr(1,value.length()-2) << "\\00\"";
	CodeBuffer::instance().emitGlobal(string_command.str());
	return ret;
}
	
hw5_stuff::exp_element hw5_stuff::emit_exp_ID(string value, int line) {
	CC& C = CC::getInstance();
	exp_element ret;
	ret = get_exp_by_id(value, line);	
	ret.void_res_exp = false;
	
	if (C.variables.exists(value)) {
		variable v = C.variables.get_variable(value);
		
		// if we just got a boolean value, we should add branches
		if (v.getType() == type_bool) {
			// do the actual compartion
			stringstream calc_command, calc_comment;
			stringstream comp_command, comp_comment;
			int calc = C.curr_reg++;

			// used to be i1, now i32
			//calc_command << "	%t" << calc << " = icmp eq i1 %t" << ret.llvm_ptr_id << ", 1";
			calc_command << "	%t" << calc << " = icmp eq i32 %t" << ret.llvm_ptr_id << ", 1";
			calc_comment << "check the value of variable " << value << ", which is being held inside %t" << calc;
			push_code(calc_command.str(), calc_comment.str());	
			
			comp_command << "	br i1 %t" << calc << ", label @, label @";
			comp_comment << "branch for each possible case of " << value << "'s value";
			int loc1 = push_code(comp_command.str(), comp_comment.str());
			
			// make a new truelist
			vector<pair<int,BranchLabelIndex>> truelist = CodeBuffer::instance().makelist({loc1,FIRST});
			C.label_list[C.label_list_key_gen] = truelist;
			ret.true_list_id = C.label_list_key_gen++;
			
			// make a new falselist
			vector<pair<int,BranchLabelIndex>> falselist = CodeBuffer::instance().makelist({loc1,SECOND});
			C.label_list[C.label_list_key_gen] = falselist;
			ret.false_list_id = C.label_list_key_gen++;
		}
	}
	
	return ret;
}


// emiting boolean expr

hw5_stuff::exp_element hw5_stuff::emit_exp_TRUE() {
	CC& C = CC::getInstance();
	exp_element exp;
	
	exp.type = type_bool; 
	exp.init = true; 
	exp.void_res_exp = false;
	
	// Generating the code :

	// Will be in the true list
	int loc1 = push_code("	br label @","always TRUE expr");
	
	// no need for falselist
	vector<pair<int,BranchLabelIndex>> falselist = vector<pair<int,BranchLabelIndex>>();
	C.label_list[C.label_list_key_gen] = falselist;
	exp.false_list_id = C.label_list_key_gen++;
	
	// make a new truelist
	vector<pair<int,BranchLabelIndex>> truelist = CodeBuffer::instance().makelist({loc1,FIRST});
	C.label_list[C.label_list_key_gen] = truelist;
	exp.true_list_id = C.label_list_key_gen++;
	
	return exp;
}

hw5_stuff::exp_element hw5_stuff::emit_exp_FALSE() {
	CC& C = CC::getInstance();
	exp_element exp;
	
	exp.type = type_bool; 
	exp.init = true; 
	exp.void_res_exp = false;
	
	// Generating the code :

	// Will be in the true list
	int loc1 = push_code("	br label @","always FALSE expr");
	
	// no need for truelist
	C.label_list[C.label_list_key_gen] = vector<pair<int,BranchLabelIndex>>();
	exp.true_list_id = C.label_list_key_gen++;
	
	// make a new falselist
	C.label_list[C.label_list_key_gen] = CodeBuffer::instance().makelist({loc1,FIRST});
	exp.false_list_id = C.label_list_key_gen++;
	
	return exp;
}

hw5_stuff::exp_element hw5_stuff::emit_exp_NOT(exp_element exp, int line) {
	exp_element ret;
	check_not_void(exp, line);
	check_exp_is(type_bool, exp, line);

	ret.type            = exp.type;
	ret.enum_val        = exp.enum_val;
	ret.init            = exp.init;
	ret.void_res_exp    = exp.void_res_exp;
	ret.llvm_ptr_id     = exp.llvm_ptr_id;
	
	ret.false_list_id   = exp.true_list_id;
	ret.true_list_id    = exp.false_list_id;
	
	return ret;
}

hw5_stuff::exp_element hw5_stuff::emit_exp_AND(exp_element exp_l, exp_element exp_r, int line, string marker) {
	CC& C = CC::getInstance();
	exp_element ret;
	check_not_void(exp_l, line);
	check_not_void(exp_r, line);
	check_exp_is(type_bool, exp_l, line);
	check_exp_is(type_bool, exp_r, line);
	ret.type = type_bool; 
	ret.init = exp_l.init && exp_r.init;
	ret.void_res_exp = false;
		
	// backpatch all the first ones truelist to the begining of the second exp
	CodeBuffer::instance().bpatch(C.label_list[exp_l.true_list_id], marker);
	C.label_list.erase(exp_l.true_list_id);
	
	// the new true list id is the right exp truelist
	ret.true_list_id = exp_r.true_list_id;
	
	// make a new falselist
	C.label_list[C.label_list_key_gen] = CodeBuffer::instance().merge(C.label_list[exp_l.false_list_id] , 
																	  C.label_list[exp_r.false_list_id]);
	ret.false_list_id = C.label_list_key_gen++;
	C.label_list.erase(exp_l.false_list_id);             // erasing by key
	C.label_list.erase(exp_r.false_list_id);             // erasing by key
	
	return ret;
}

hw5_stuff::exp_element hw5_stuff::emit_exp_OR(exp_element exp_l, exp_element exp_r, int line, string marker) {
	CC& C = CC::getInstance();
	exp_element ret;
	check_not_void(exp_l, line);
	check_not_void(exp_r, line);
	check_exp_is(type_bool, exp_l, line);
	check_exp_is(type_bool, exp_r, line);
	ret.type = type_bool; 
	ret.init = exp_l.init && exp_r.init;
	ret.void_res_exp = false;
		
	// backpatch all the first ones truelist to the begining of the second exp
	// then, no need for left list any more
	CodeBuffer::instance().bpatch(C.label_list[exp_l.false_list_id], marker);
	C.label_list.erase(exp_l.false_list_id);
	
	// the new true list id is the right exp truelist
	ret.false_list_id = exp_r.false_list_id;
	
	// make a new falselist
	vector<pair<int,BranchLabelIndex>> truelist = CodeBuffer::instance().merge(
		C.label_list[exp_l.true_list_id] , C.label_list[exp_r.true_list_id]
	);
	C.label_list[C.label_list_key_gen] = truelist;
	ret.true_list_id = C.label_list_key_gen++;
	C.label_list.erase(exp_l.true_list_id);
	C.label_list.erase(exp_r.true_list_id);	
	
	return ret;
}

hw5_stuff::exp_element hw5_stuff::emit_exp_EQ(exp_element exp_l, exp_element exp_r, int line, bool check_eq_not_neq) {
	CC& C = CC::getInstance();
	exp_element ret;
	check_not_void(exp_l, line);
	check_not_void(exp_r, line);
	check_exp_is_numeric(exp_l, line);
	check_exp_is_numeric(exp_r, line);
	ret.type = type_bool; 
	ret.init = exp_l.init && exp_r.init;
	ret.void_res_exp = false;
		
	int comp_1, comp_2;
	
	/*if (exp_l.type == type_byte && exp_r.type == type_int) {
		comp_1 = C.curr_reg++;
		stringstream zext_command, zext_comment;
		zext_command << "	%t" << comp_1 <<" = zext i8 %t" << exp_l.llvm_ptr_id << " to i32";
		zext_comment << "converting the value from 8 bits to 32 bits by pending zeros from the left";
		push_code(zext_command.str(), zext_comment.str());	
	} else {*/
		comp_1 = exp_l.llvm_ptr_id;
	/*}
	
	if (exp_r.type == type_byte && exp_l.type == type_int) {
		comp_2 = C.curr_reg++;
		stringstream zext_command, zext_comment;
		zext_command << "	%t" << comp_2 <<" = zext i8 %t" << exp_r.llvm_ptr_id << " to i32";
		zext_comment << "converting the value from 8 bits to 32 bits by pending zeros from the left";
		push_code(zext_command.str(), zext_comment.str());		
	} else {*/
		comp_2 = exp_r.llvm_ptr_id;
	/*}*/
	
	// do the actual compartion
	stringstream calc_command, calc_comment;
	stringstream comp_command, comp_comment;
	int calc = C.curr_reg++;

	calc_command << "	%t" << calc << " = icmp " << (check_eq_not_neq ? "eq" : "ne") << " i32" << 
		/*((exp_r.type == type_byte && exp_l.type == type_byte) ? "8" : "32") <<*/
		" %t" << comp_1 << ", %t" << comp_2;
	calc_comment << "check if t" << comp_1 << ", %t" << comp_2 << " are " <<  (check_eq_not_neq ? "equil" : "not equil");
	push_code(calc_command.str(), calc_comment.str());	
	
	comp_command << "	br i1 %t" << calc << ", label @, label @";
	comp_comment << "";//"jump for each possible case";
	int loc2 = push_code(comp_command.str(), comp_comment.str());
	
	// make a new truelist
	vector<pair<int,BranchLabelIndex>> truelist = CodeBuffer::instance().makelist({loc2,FIRST});
	C.label_list[C.label_list_key_gen] = truelist;
	ret.true_list_id = C.label_list_key_gen++;
	
	// make a new falselist
	vector<pair<int,BranchLabelIndex>> falselist = CodeBuffer::instance().makelist({loc2,SECOND});
	C.label_list[C.label_list_key_gen] = falselist;
	ret.false_list_id = C.label_list_key_gen++;
	
	return ret;
}

hw5_stuff::exp_element hw5_stuff::emit_exp_RELOP(exp_element exp_l, exp_element exp_r, int line, int type) {
	CC& C = CC::getInstance();
	exp_element ret;
	
	check_not_void(exp_l, line);
	check_not_void(exp_r, line);
	check_exp_is_numeric(exp_l, line);
	check_exp_is_numeric(exp_r, line);
	ret.type = type_bool; 
	ret.init = exp_l.init && exp_r.init;
	ret.void_res_exp = false;
	
	int comp_1 = exp_l.llvm_ptr_id;
	int comp_2 = exp_r.llvm_ptr_id;
	
	// do the actual compartion
	int calc = C.curr_reg++;
	
	string comp_type, comp_type_cmt;
	comp_type = (type == 0 ? "sge" : (type == 1 ? "sle" : (type == 2 ? "sgt" : "slt")));
	comp_type_cmt = (type == 0 ? "greater or equil" : (type == 1 ? "smaller or equil" : (type == 2 ? "greater" : "smaller")));
	
	stringstream calc_command, calc_comment;
	calc_command << "	%t" << calc << " = icmp " << comp_type << " i32" << " %t" << comp_1 << ", %t" << comp_2;
	calc_comment << "calculate if %t" << comp_1 << " is " << comp_type_cmt << " then %t" << comp_2;
	
	push_code(calc_command.str(), calc_comment.str());	
	
	stringstream comp_command;
	comp_command << "	br i1 %t" << calc << ", label @, label @";
	int loc2 = push_code(comp_command.str(), "");
	
	// make a new truelist
	vector<pair<int,BranchLabelIndex>> truelist = CodeBuffer::instance().makelist({loc2,FIRST});
	C.label_list[C.label_list_key_gen] = truelist;
	ret.true_list_id = C.label_list_key_gen++;
	
	// make a new falselist
	vector<pair<int,BranchLabelIndex>> falselist = CodeBuffer::instance().makelist({loc2,SECOND});
	C.label_list[C.label_list_key_gen] = falselist;
	ret.false_list_id = C.label_list_key_gen++;
	
	return ret;
}

// emiting plus & minus calculations

hw5_stuff::exp_element hw5_stuff::emit_plus_minus(exp_element exp_l, exp_element exp_r, int line, bool plus_not_minus) {
	CC& C = CC::getInstance();
	exp_element ret;	
	check_not_void(exp_l, line);
	check_not_void(exp_r, line);
	check_exp_is_numeric(exp_l, line);
	check_exp_is_numeric(exp_r, line);
	ret.type = (exp_l.type == type_byte && exp_r.type == type_byte ? type_byte : type_int); 
	ret.init = exp_l.init && exp_r.init;
	
	int val_1, val_2;
	
	/*if (exp_l.type == type_byte && exp_r.type == type_int) {
		val_1 = C.curr_reg++;
		stringstream zext_command, zext_comment;
		zext_command << "	%t" << val_1 <<" = zext i8 %t" << exp_l.llvm_ptr_id << " to i32";
		zext_comment << "converting the value from 8 bits to 32 bits by pending zeros from the left";
		push_code(zext_command.str(), zext_comment.str());	
	} else {*/
		val_1 = exp_l.llvm_ptr_id;
	/*}
	
	if (exp_r.type == type_byte && exp_l.type == type_int) {
		val_2 = C.curr_reg++;
		stringstream zext_command, zext_comment;
		zext_command << "	%t" << val_2 <<" = zext i8 %t" << exp_r.llvm_ptr_id << " to i32";
		zext_comment << "converting the value from 8 bits to 32 bits by pending zeros from the left";
		push_code(zext_command.str(), zext_comment.str());		
	} else {*/
		val_2 = exp_r.llvm_ptr_id;
	/*}*/
	
	// do the actual calculation
	stringstream calc_command, calc_comment;
	ret.llvm_ptr_id = C.curr_reg++;

	calc_command << "	%t" << ret.llvm_ptr_id << " = " << (plus_not_minus ? "add" : "sub") <<
		/*(exp_l.type == type_int || exp_r.type == type_int ? " i32 " : " nsw i8 ") <<*/ " i32 " <<
		"%t" << val_1 << ", %t" << val_2;
	calc_comment << "calculate the " << (plus_not_minus ? "sum" : "diff") << " of reg %t" << val_1 << " and reg %t" << val_2;
	push_code(calc_command.str(), calc_comment.str());	
	
	// if its a byte, cut the 26 upper bits
	if (ret.type == type_byte) {
		stringstream bitwiseand_command, bitwiseand_comment;
		int new_reg = C.curr_reg++;
		bitwiseand_command << "	%t" << new_reg << " = and i32 %t" << ret.llvm_ptr_id << ", 255";
		bitwiseand_comment << "nothice that in order to calculate byte calculation a bit-wise and is requiered to save the first 8 bits";
		push_code(bitwiseand_command.str(), bitwiseand_comment.str());	
		ret.llvm_ptr_id = new_reg;
	}
	
	return ret;
}

hw5_stuff::exp_element hw5_stuff::emit_mult(exp_element exp_l, exp_element exp_r, int line) {
	CC& C = CC::getInstance();
	exp_element ret;	
	check_not_void(exp_l, line);
	check_not_void(exp_r, line);
	check_exp_is_numeric(exp_l, line);
	check_exp_is_numeric(exp_r, line);
	ret.type = (exp_l.type == type_byte && exp_r.type == type_byte ? type_byte : type_int); 
	ret.init = exp_l.init && exp_r.init;
	
	int val_1, val_2;
	
	/*if (exp_l.type == type_byte && exp_r.type == type_int) {
		val_1 = C.curr_reg++;
		stringstream zext_command, zext_comment;
		zext_command << "	%t" << val_1 <<" = zext i8 %t" << exp_l.llvm_ptr_id << " to i32";
		zext_comment << "converting the value from 8 bits to 32 bits by pending zeros from the left";
		push_code(zext_command.str(), zext_comment.str());	
	} else {*/
		val_1 = exp_l.llvm_ptr_id;
	/*}
	
	if (exp_r.type == type_byte && exp_l.type == type_int) {
		val_2 = C.curr_reg++;
		stringstream zext_command, zext_comment;
		zext_command << "	%t" << val_2 <<" = zext i8 %t" << exp_r.llvm_ptr_id << " to i32";
		zext_comment << "converting the value from 8 bits to 32 bits by pending zeros from the left";
		push_code(zext_command.str(), zext_comment.str());		
	} else {*/
		val_2 = exp_r.llvm_ptr_id;
	/*}*/
	
	// do the actual calculation
	stringstream calc_command, calc_comment;
	stringstream comp_command, comp_comment;
	ret.llvm_ptr_id = C.curr_reg++;
	
	calc_command << "	%t" << ret.llvm_ptr_id << " = mul" <<
		/*(exp_l.type == type_int || exp_r.type == type_int ? " i32 " : " nsw i8 ") <<*/ " i32 "<<
		"%t" << val_1 << ", %t" << val_2;
	calc_comment << "calculate the mult of reg %t" << val_1 << " and reg %t" << val_2;
	push_code(calc_command.str(), calc_comment.str());	
	
	// if its a byte, cut the 26 upper bits
	if (ret.type == type_byte) {
		stringstream bitwiseand_command, bitwiseand_comment;
		int new_reg = C.curr_reg++;
		bitwiseand_command << "	%t" << new_reg << " = and i32 %t" << ret.llvm_ptr_id << ", 255";
		bitwiseand_comment << "nothice that in order to calculate byte calculation a bit-wise and is requiered to save the first 8 bits";
		push_code(bitwiseand_command.str(), bitwiseand_comment.str());	
		ret.llvm_ptr_id = new_reg;
	}
	
	return ret;	
}

hw5_stuff::exp_element hw5_stuff::emit_div(exp_element exp_l, exp_element exp_r, int line) {
	CC& C = CC::getInstance();
	exp_element ret;	
	check_not_void(exp_l, line);
	check_not_void(exp_r, line);
	check_exp_is_numeric(exp_l, line);
	check_exp_is_numeric(exp_r, line);
	ret.type = (exp_l.type == type_byte && exp_r.type == type_byte ? type_byte : type_int); 
	ret.init = exp_l.init && exp_r.init;
	
	emit_div_check_not_zero(exp_r);
	
	int val_1, val_2;
	
	/*if (exp_l.type == type_byte && exp_r.type == type_int) {
		val_1 = C.curr_reg++;
		stringstream zext_command, zext_comment;
		zext_command << "	%t" << val_1 <<" = zext i8 %t" << exp_l.llvm_ptr_id << " to i32";
		zext_comment << "converting the value from 8 bits to 32 bits by pending zeros from the left";
		push_code(zext_command.str(), zext_comment.str());	
	} else {*/
		val_1 = exp_l.llvm_ptr_id;
	/*}
	
	if (exp_r.type == type_byte && exp_l.type == type_int) {
		val_2 = C.curr_reg++;
		stringstream zext_command, zext_comment;
		zext_command << "	%t" << val_2 <<" = zext i8 %t" << exp_r.llvm_ptr_id << " to i32";
		zext_comment << "converting the value from 8 bits to 32 bits by pending zeros from the left";
		push_code(zext_command.str(), zext_comment.str());		
	} else {*/
		val_2 = exp_r.llvm_ptr_id;
	/*}*/
	
	// do the actual calculation
	stringstream calc_command, calc_comment;
	stringstream comp_command, comp_comment;
	ret.llvm_ptr_id = C.curr_reg++;
	
	calc_command << "	%t" << ret.llvm_ptr_id << " =" << " sdiv i32 "
		/*(exp_l.type == type_int || exp_r.type == type_int ? " sdiv " : " udiv ") <<
		(exp_l.type == type_int || exp_r.type == type_int ? " i32 " : " i8 ") <<*/
		"%t" << val_1 << ", %t" << val_2;
	calc_comment << "calculate the div of reg %t" << val_1 << " in reg %t" << val_2;
	push_code(calc_command.str(), calc_comment.str());	
	
	// if its a byte, cut the 26 upper bits
	if (ret.type == type_byte) {
		stringstream bitwiseand_command, bitwiseand_comment;
		int new_reg = C.curr_reg++;
		bitwiseand_command << "	%t" << new_reg << " = and i32 %t" << ret.llvm_ptr_id << ", 255";
		bitwiseand_comment << "nothice that in order to calculate byte calculation a bit-wise and is requiered to save the first 8 bits";
		push_code(bitwiseand_command.str(), bitwiseand_comment.str());	
		ret.llvm_ptr_id = new_reg;
	}
		
	return ret;
}

hw5_stuff::exp_element hw5_stuff::emit_div_check_not_zero(exp_element exp) {
	CC& C = CC::getInstance();

	// do the actual compartion
	stringstream calc_command, calc_comment;
	stringstream comp_command, comp_comment;
	int calc = C.curr_reg++;

	calc_command << "	%t" << calc << " = icmp eq i" <</* ((exp.type == type_byte) ? "8" : "32")*/"32" << " %t" << exp.llvm_ptr_id << ", 0";
	calc_comment << "check if the devider is zero";
	push_code(calc_command.str(), calc_comment.str());	
	
	comp_command << "	br i1 %t" << calc << ", label @, label @";
	comp_comment << "";//"jump for each possible case";
	int loc2 = push_code(comp_command.str(), comp_comment.str());
	
	// make a new truelist
	string eq_to_zero = push_label(4);
	push_code("	call void @print(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str_zero_div, i32 0, i32 0))", "");//"notify user that the program tried to devied in zero");
	push_code("	call void @exit(i32 0)", "");// "devider was zero, exit program");
	push_code("	unreachable", "");
	string not_eq_to_zero = push_label(5);
	
	CodeBuffer::instance().bpatch(CodeBuffer::instance().makelist({loc2,FIRST}), eq_to_zero);
	CodeBuffer::instance().bpatch(CodeBuffer::instance().makelist({loc2,SECOND}), not_eq_to_zero);
}

// emiting assign to variables

void hw5_stuff::emit_var_assign_ENUM(variable& v, exp_element exp) {
	CC& C = CC::getInstance();
	
	// Add code to set value
	int vaule_from = exp.llvm_ptr_id;
	stringstream store_command, store_comment;
	store_command << "	store i32 %t" << vaule_from << ", i32* %t" << v.llvm_ptr_id;
	store_comment << "set enum variable " << v.name << " " << v.llvm_ptr_id << " with value of register %t" << exp.llvm_ptr_id;
	push_code(store_command.str(), store_comment.str());
}

void hw5_stuff::emit_var_assign_INT32(variable& v, exp_element exp) {
	CC& C = CC::getInstance();
	
	// Add code to set value
	int vaule_from = exp.llvm_ptr_id;
	stringstream store_command, store_comment;
	store_command << "	store i32 %t" << vaule_from << ", i32* %t" << v.llvm_ptr_id;
	store_comment << "set variable " << v.name << " with value of register %t" << exp.llvm_ptr_id;
	push_code(store_command.str(), store_comment.str());
}

void hw5_stuff::emit_var_assign_BYTE(variable& v, exp_element exp) {
	CC& C = CC::getInstance();
	
	// Add code to set value
	int vaule_from = exp.llvm_ptr_id; /*C.curr_reg++;
	stringstream zext_command, zext_comment;
	zext_command << "	%t" << vaule_from <<" = zext i8 %t" << exp.llvm_ptr_id << " to i32";
	zext_comment << "converting the value from 8 bits to 32 bits by pending zeros from the left";
	push_code(zext_command.str(), zext_comment.str());*/		
	
	stringstream store_command, store_comment;
	store_command << "	store i32 %t" << vaule_from << ", i32* %t" << v.llvm_ptr_id;
	store_comment << "set variable " << v.name << " with value of register %t" << exp.llvm_ptr_id;
	push_code(store_command.str(), store_comment.str());
}

void hw5_stuff::emit_var_assign_BOOL(variable& v, exp_element exp) {
	CC& C = CC::getInstance();
		
	// In case of true
	string true_marker = push_label(6);
		
	CodeBuffer::instance().bpatch(C.label_list[exp.true_list_id], true_marker);
	C.label_list.erase(exp.true_list_id);
	
	stringstream store_command, store_comment;
	store_command << "	store i32 1, i32* %t" << v.llvm_ptr_id;
	store_comment << "here we store the boolean value TRUE into the boolean variable " << v.name << " (using ptr %t" << v.llvm_ptr_id << ")";
	push_code(store_command.str(), store_comment.str());
	
	// lets save this spot to jump over the FALSE assign case
	int loc1 = push_code("	br label @","after setting a TRUE value, jump over the FALSE update to the end of the assignment");
	// In case of false
	string false_marker = push_label(7);
	CodeBuffer::instance().bpatch(C.label_list[exp.false_list_id], false_marker);
	C.label_list.erase(exp.false_list_id);
	
	stringstream store_command2, store_comment2;
	store_command2 << "	store i32 0, i32* %t" << v.llvm_ptr_id;
	store_comment2 << "here we store the boolean value FALSE into the boolean variable " << v.name << " (using ptr %t" << v.llvm_ptr_id << ")";
	
	
	push_code(store_command2.str(), store_comment2.str());
	int loc2 = push_code("	br label @","after setting a FALSE value, jump to the end of the assignment");
	
	string finish_marker = push_label(8);
	CodeBuffer::instance().bpatch(CodeBuffer::instance().makelist({loc1, FIRST}), finish_marker);
	CodeBuffer::instance().bpatch(CodeBuffer::instance().makelist({loc2, FIRST}), finish_marker);
}

// convertion

hw5_stuff::exp_element hw5_stuff::emit_var_convertion(allowed_types t, exp_element exp, int line) {
	CC& C = CC::getInstance();
	exp_element ret;

	check_not_void(exp, line);
	check_exp_can_covert(t, exp, line);
	ret.type = t;
	ret.init = exp.init;
	ret.void_res_exp = false;
	ret.llvm_ptr_id = exp.llvm_ptr_id;
	
	return ret;
}

// if, else, while

void hw5_stuff::emit_if(exp_element exp, string true_label, int true_finish_branch) {
	CC& C = CC::getInstance();

	// Add finish label
	string finish_label = push_label(9);
	
	// true list should point at true_label
	CodeBuffer::instance().bpatch(C.label_list[exp.true_list_id], true_label);
	C.label_list.erase(exp.true_list_id);
	
	// false list & true_finish_branch should go to finish_label
	CodeBuffer::instance().bpatch(CodeBuffer::instance().makelist({true_finish_branch,FIRST}), finish_label);
	CodeBuffer::instance().bpatch(C.label_list[exp.false_list_id], finish_label);
	C.label_list.erase(exp.false_list_id);
}

void hw5_stuff::emit_if_else(exp_element exp, string true_label, int true_finish_branch, string false_label, int false_finish_branch) {
	CC& C = CC::getInstance();

	// Add finish label
	string finish_label = push_label(10);
		
	// true list should point at true_label	
	CodeBuffer::instance().bpatch(C.label_list[exp.true_list_id], true_label);
	C.label_list.erase(exp.true_list_id);
	
	// false list should point at false_label
	CodeBuffer::instance().bpatch(C.label_list[exp.false_list_id], false_label);
	C.label_list.erase(exp.false_list_id);
	
	// false_finish_branch & true_finish_branch should go to finish_label
	CodeBuffer::instance().bpatch(CodeBuffer::instance().makelist({true_finish_branch,FIRST}), finish_label);
	CodeBuffer::instance().bpatch(CodeBuffer::instance().makelist({false_finish_branch,FIRST}), finish_label);
}

void hw5_stuff::emit_while(int code_start_branch, string cond_label, exp_element exp, string code_label, int code_finish_branch, string loop_end_label) {
	CC& C = CC::getInstance();

	// true list should point at code_label	
	CodeBuffer::instance().bpatch(C.label_list[exp.true_list_id], code_label);
	C.label_list.erase(exp.true_list_id);
	
	// false list should point at loop_end_label	
	CodeBuffer::instance().bpatch(C.label_list[exp.false_list_id], loop_end_label);
	C.label_list.erase(exp.true_list_id);
	
	// code_finish_branch & start_branch should go to cond_label
	CodeBuffer::instance().bpatch(CodeBuffer::instance().makelist({code_finish_branch,FIRST}), cond_label);
	CodeBuffer::instance().bpatch(CodeBuffer::instance().makelist({code_start_branch,FIRST}), cond_label);
	
	// break branches should go to loop_end_label
	CodeBuffer::instance().bpatch(C.break_bp[C.break_bp.size()-1], loop_end_label);
	C.break_bp.pop_back();
		
	// continue branches should go to cond_label
	CodeBuffer::instance().bpatch(C.continue_bp[C.continue_bp.size()-1], cond_label);
	C.continue_bp.pop_back();
}

void hw5_stuff::emit_break_command() {
	CC& C = CC::getInstance();
#if VERBOSITY_ON == 1
	int loc1 = push_code("	br label @  ; (this is a break)", "");
#else
	int loc1 = push_code("	br label @", "");
#endif
	C.break_bp[C.break_bp.size()-1].push_back({loc1, FIRST});
}

void hw5_stuff::emit_continue_command() {
	CC& C = CC::getInstance();
#if VERBOSITY_ON == 1
	int loc1 = push_code("	br label @  ; (this is a continue)", "");
#else
	int loc1 = push_code("	br label @", "");
#endif
	C.continue_bp[C.continue_bp.size()-1].push_back({loc1, FIRST});	
}

int hw5_stuff::emit_br_command() {
	return push_code("	br label @", "");
}

// functions

void hw5_stuff::emit_current_function_header() {
	CC& C = CC::getInstance();
	function& f = C.functions.functions_data[C.functions.functions_name2index[C.currently_defined_element]];
	vector<variable>& vars = f.args;
	string name = f.name;
	ret_type t = f.return_type;	
		
	// Add code to genrate function declaretion
	stringstream new_func;
	new_func << "define " << (t == ret_type_void ? "void" : "i32") << " @" << name << "(";
	if (vars.size() > 0) {
		new_func << "i32";
		for (int i=1; i<vars.size(); i++) new_func << ", i32";
	} 	
	new_func << ") {";	
	push_code(new_func.str(), "");	
	
#if VERBOSITY_ON == 1	
	if (vars.size() > 0)  push_code("	; ======================= input arguments definition =======================", "");
#endif
	
	for (int i=0; i<vars.size(); i++) {
		variable& v = vars[i];
		
		// make place
		stringstream new_input_command, new_input_comment;
		new_input_command << "	%t" << v.llvm_ptr_id << " = alloca i32";
		new_input_comment << "define a space in the stack for input " << v.getTypeDescString() << " variable " << v.name 
						  << " and save to register %t" << v.llvm_ptr_id << " a pointer to it";
		push_code(new_input_command.str(), new_input_comment.str());
				
		// Add code to store variable
		stringstream store_command, store_comment;
		store_command << "	store i32 %" << v.llvm_ptr_id << ", i32* %t" << v.llvm_ptr_id;
		store_comment << "store the input " << v.getTypeDescString() << " variable " << v.name << " in it's assigned space in stack";
		push_code(store_command.str(), store_comment.str());
	}
	
#if VERBOSITY_ON == 1
	push_code("	; ============================ stack definition ============================", "");
#endif
	
	push_code("	%stack = alloca [50 x i32]", "define a space in the stack for maximum 50 variables");
	
	#if VERBOSITY_ON == 1
	push_code("	; ============================= function body ==============================", "");
#endif
}

void hw5_stuff::emit_return_void() {
	push_code("	ret void", "exit the function and return nothing");
}

void hw5_stuff::emit_return(exp_element exp) {
	
	if (exp.type != type_bool) {	
		stringstream ret, ret_comment;
		ret << "	ret i32 %t" << exp.llvm_ptr_id;
		ret_comment << "exit function and return the value that register %t" << exp.llvm_ptr_id << " holds";
		push_code(ret.str(), ret_comment.str());
	} else {
		CC& C = CC::getInstance();

		// In case of true
		string true_marker = push_label(11);
			
		CodeBuffer::instance().bpatch(C.label_list[exp.true_list_id], true_marker);
		C.label_list.erase(exp.true_list_id);
		
		stringstream store_command, store_comment;
		store_command << "	ret i32 1";
		store_comment << "exit function and return TRUE";
		push_code(store_command.str(), store_comment.str());
		
		// In case of false
		string false_marker = push_label(12);
		CodeBuffer::instance().bpatch(C.label_list[exp.false_list_id], false_marker);
		C.label_list.erase(exp.false_list_id);
		
		stringstream store_command2, store_comment2;
		store_command2 << "	ret i32 0";
		store_comment2 << "exit function and return FALSE";
		push_code(store_command2.str(), store_comment2.str());
	}
}

// in case we dont use this function return value but it returns bool, need to backpatch it
void hw5_stuff::eimit_call_backpatch(exp_element exp) {
	if (!exp.void_res_exp && exp.type == type_bool) {
		CC& C = CC::getInstance();

		// Add finish label
		string finish_label = push_label(13);
		
		// true list should point at finish_label
		CodeBuffer::instance().bpatch(C.label_list[exp.true_list_id], finish_label);
		CodeBuffer::instance().bpatch(C.label_list[exp.false_list_id], finish_label);
		C.label_list.erase(exp.true_list_id);
		C.label_list.erase(exp.false_list_id);
	}
}

// Code Generation Stuff ------------------------------------------------------------------------>
