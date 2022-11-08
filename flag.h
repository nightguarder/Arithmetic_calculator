//Create a Struct to hold flag data
struct Flag
{
    //initial state
    int character_is_operator = 1;
    int f_number, negative_number,
    decimal_number,last_digit,
    space_after_number, last_operator,
    perfom_operation, function_called,
    parenthesis_open,parenthesis_closed, multiple_parenthesis,
    parenthesis_open_if_operator, function_name,
    function_sqrt,parenthesis_func,
    operation_func, divide_digits;
};
//stackoverflow
static Flag initialize; //`static` forces a default initialization of zero for each value
void perfom_operation(struct StackNode **stack_output, struct StackNode **stack_operator);
void perfom_one_parameter_function(struct StackNode **stack_output, char function[]);
