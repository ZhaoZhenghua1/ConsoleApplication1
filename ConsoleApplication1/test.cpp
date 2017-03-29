class xpath_ast_node
{
private:
	// node type
	ast_type_t _type;
	xpath_value_type _rettype;

	// for ast_step
	axis_t _axis;

	// for ast_step/ast_predicate/ast_filter
	nodetest_t _test;

	// tree node structure
	xpath_ast_node* _left;
	xpath_ast_node* _right;
	xpath_ast_node* _next;

	union
	{
		// value for ast_string_constant
		const char_t* string;
		// value for ast_number_constant
		double number;
		// variable for ast_variable
		xpath_variable* variable;
		// node test for ast_step (node name/namespace/node type/pi target)
		const char_t* nodetest;
		// table for ast_opt_translate_table
		const unsigned char* table;
	} _data;
};