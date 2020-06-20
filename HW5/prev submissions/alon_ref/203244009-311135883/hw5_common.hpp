#ifndef _236360_5_common_
#define _236360_5_common_

#include <unordered_map> 
#include <string.h> 
#include <string>
#include <sstream>
#include <iomanip>

#include "hw5_bisonStructs.hpp"
#include "hw3_output.hpp" 
#include "hw5_CC.hpp" 


using namespace std;

namespace hw5_stuff {
	
	// Enum declarations
	void enter_enum_decl_scope(string name, int line);
	void add_enum_value(string value, int line);
	void exit_enum_decl_scope();
	
	// Function 
	void enter_function_scope();	
	void new_function(string name, ret_type t, int line);	
	void new_input(allowed_types t, string name, int line);
	void new_input_enum(string name, string enum_name, int line);
	void exit_function_scope();	
	void check_return_void(int line);    
	void check_return(exp_element exp, int line);
	
	// scope functions	
	void enter_scope();
	void exit_scope();
	void enter_loop();
	void exit_loop();
	void check_break(int line);   
	void check_continue(int line);
	
	// new variables
	void new_variable(allowed_types t, string name, int line, bool assign_default);
	void assign_to_variable(string name, int line, exp_element exp);
	
	// new enum variable
	void new_enum_variable(string type, string name, int line, bool assign_default);
	void assign_to_enum_variable(string type, string name, int line, exp_element exp);
	
	// Exp functions
	void check_exp_is(allowed_types t, exp_element ex, int line);
	void check_exp_is_numeric(exp_element ex, int line);
	void check_byte_range(int val, int line);
	void check_is_def_enum(string name, int line);
	void check_var_id_is_def(string name, int line);
	void check_var_id_is_not_def(string name, int line);
	void check_exp_can_covert(allowed_types t, exp_element ex, int line);
	exp_element get_exp_by_id(string name, int line);
	void check_assignment(string name, exp_element exp, int line);
	allowed_types get_type_from_call(ret_type ret, int line); 	
	void check_not_void(exp_element exp, int line);
	
	// finish program
	void start_program();
	void finish_program();
	
	// Call functions
	void check_called_function_exists(string name, int line);
	void check_called_function_arg(exp_element exp, int line);
	exp_element check_called_function_close(int line);

	// general
	bool CanDefine(string str);
	void print_and_clear_scope();
	
	// Code Generation	
	int push_code(string command, string comment);
	string push_label(int id);
	
	// emiting value expr
	exp_element emit_exp_INT32  (int value); 
	exp_element emit_exp_BYTE  	(int value, int line); 
	exp_element emit_exp_STRING (string value); 
	exp_element emit_exp_ID		(string value, int line);
	
	// emiting boolean expr
	exp_element emit_exp_TRUE	();
	exp_element emit_exp_FALSE	();
	exp_element emit_exp_NOT	(exp_element exp, int line);
	exp_element emit_exp_AND	(exp_element exp_l, exp_element exp_r, int line, string marker);
	exp_element emit_exp_OR 	(exp_element exp_l, exp_element exp_r, int line, string marker);
	exp_element emit_exp_EQ 	(exp_element exp_l, exp_element exp_r, int line, bool check_eq_not_neq);
	exp_element emit_exp_RELOP 	(exp_element exp_l, exp_element exp_r, int line, int type);
	
	
	//  emiting numeric expr + - / *
	exp_element emit_plus_minus(exp_element exp_l, exp_element exp_r, int line, bool plus_not_minus);
	exp_element emit_mult(exp_element exp_l, exp_element exp_r, int line);
	exp_element emit_div(exp_element exp_l, exp_element exp_r, int line);
	exp_element emit_div_check_not_zero(exp_element exp);
	
	// variables
	void emit_var_create		(string name, exp_element exp);
	void emit_var_assign_ENUM	(variable& v, exp_element exp);
	void emit_var_assign_INT32	(variable& v, exp_element exp);
	void emit_var_assign_BYTE 	(variable& v, exp_element exp);
	void emit_var_assign_BOOL 	(variable& v, exp_element exp);
	
	// if else while
	int emit_br_command ();
	void emit_if		(exp_element exp, string true_label, int true_finish_branch);
	void emit_if_else	(exp_element exp, string true_label, int true_finish_branch, string false_label, int false_finish_branch);
	void emit_while		(int code_start_branch, string cond_label, exp_element exp, string code_label, int code_finish_branch, string loop_end_label);
	void emit_break_command();
	void emit_continue_command();
	
	// covertions
	exp_element emit_var_convertion	(allowed_types t, exp_element exp, int line);
	
	// functions
	void emit_current_function_header();
	string get_var_call_string(exp_element ret);
	void emit_return_void();
	void emit_return(exp_element exp);
	void eimit_call_backpatch(exp_element exp);
}

#endif