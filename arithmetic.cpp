//PREZENTACE
//PRACE SE SOUBORY
FILE *fp;
Flag f;
//VARIABLES
    int read;
    char *priklad;
    double temp, number_remainder,decimal_number, res = 0;
    char functions[5]; //size max 5
//COUNTERS
    int counter_multiple_parenthesis = 0;
    int divider_digits_after_decimal_point = 1;
    int counter_for_letters_in_func = 0;
//Public Struct
struct StackNode *stack_out = NULL;
struct StackNode *stack_op = NULL;
//allocate memory size 
int allocate_size(int argc, char** argv){
    //point to file passed
    char *ptr = argv[2];
    //invalid conversion from 'void*' to 'char*'
    priklad = (char*)malloc(sizeof(char) * strlen(ptr));
    priklad = strcpy(priklad, ptr);
    return 1;
}
int allocate_file(int argc,char** argv){
    while (*++argv && **argv == '-')
    { // parameter processing
        if (!strcmp(*argv, "-i"))
        {
            *++argv;
            priklad = (char*)malloc(sizeof(char) * strlen(*argv));
            priklad = strcpy(priklad, *argv);
        }
    }
    return 1;
}
int get_priority(char operation)
{
    if (operation == '*' || operation == '/')
        return 2;
    if (operation == '-' || operation == '+')
        return 1;
    //else
    return 0;
}
double do_the_math(double a, double b, char operation, struct StackNode **stack_op)
{
    double result = 0;
    switch (operation)
    {
    case '+':
        result = a + b; // associativni
        break;
    case '-':
        result = b - a; // zalezi na poradi tak prvni cislo ktere dostanem bude mensitel a druhe mensenec
        break;
    case '*':
        result = a * b; // associativni
        if (!is_empty((*stack_op)) && peek(&(*stack_op)) == '-')
        { // secure, e.g 8-7/1+5 without this would be -4 instead of 6
            pop(&(*stack_op));
            push(&(*stack_op), '+');
            result *= -1;
        }
        break;
    case '/':
        if (a == 0)
            error("\nERROR. Division by zero\n");
        result = b / a;
        if (!is_empty((*stack_op)) && peek(&(*stack_op)) == '-')
        {
            pop(&(*stack_op));
            push(&(*stack_op), '+');
            result *= -1;
        }
        break;
    default: // low chanses to execute
        error("\nERROR. Unknown operator\n");
        break;
    }
    return result;
}
void decide_operation(int read){
    switch (read){
        case '.':
            if (f.f_number && f.last_digit)
                decimal_number = 1;
            else
                error("ERORR. Operator . not in right place");
            break;
        case ' ':
            f.space_after_number = 1;
            f.last_digit = 0;
            break;
        case '\n':
            f.last_digit = 0;
            break;
        case '+':
            if (f.character_is_operator)
                error("ERROR. Operator before any number");
            if (f.last_operator)
                error("ERROR. Operator after operator without number");
            f.last_operator = 1;
            if (f.parenthesis_open)
                error("ERROR. Operator after parenthesis without number before");
            if (f.parenthesis_func)
                f.operation_func = 1;
            if (f.f_number){ 
                push(&stack_out, temp);
                f.last_digit = 0;
                f.f_number = 0;
            }
            if (!is_empty(stack_op) && get_priority(peek(&stack_op)) >= 1)
                perfom_operation(&stack_out, &stack_op);
            push(&stack_op, '+');
            break;
        case '-':
            if (f.last_operator || f.character_is_operator)
                f.negative_number = 1;
            if (f.parenthesis_open)
                error("ERROR. Operator after parenthesis without number before");
            if (f.parenthesis_func)
                f.operation_func = 1;
            f.last_operator = 1;
            if (f.f_number){
                push(&stack_out, temp);
                f.last_digit = 0;
                f.f_number = 0;
            }
            if (!is_empty(stack_op) && get_priority(peek(&stack_op)) >= 1 && !f.negative_number)
                perfom_operation(&stack_out, &stack_op);
            if (!f.negative_number)
                push(&stack_op, '-');
            break;
        case '*':
            if (f.character_is_operator)
                error("ERROR. Operator before any number number");
            if (f.last_operator)
                error("ERROR. Operator after operator without number");
            if (f.parenthesis_open)
                error("ERROR. Operator after parenthesis without number before");
            if (f.parenthesis_func)
                f.operation_func = 1;
            f.last_operator = 1;
            if (f.f_number){
                push(&stack_out, temp);
                f.last_digit = 0;
                f.f_number = 0;
            }
            if (!is_empty(stack_op) && get_priority(peek(&stack_op)) >= 2)
                perfom_operation(&stack_out, &stack_op);
            push(&stack_op, '*');
            break;
        case '/':
            if (f.character_is_operator)
                error("ERROR. Operator before any temp number");
            if (f.last_operator)
                error("ERROR. Operator after operator without number");
            if (f.parenthesis_open)
                error("ERROR. Operator after parenthesis without number before");
            if (f.parenthesis_func)
                f.operation_func = 1;
            f.last_operator = 1;
            if (f.f_number){
                push(&stack_out, temp);
                f.last_digit = 0;
                f.f_number = 0;
            }
            if (!is_empty(stack_op) && get_priority(peek(&stack_op)) >= 2)
                perfom_operation(&stack_out, &stack_op);
            push(&stack_op, '/');
            break;
        case '(':
            f.last_operator = 1;
            f.last_digit = 0;
            // if(f.parenthesis_func) error("\nOpened patenthesis in func weren't closed\n");
            if (!f.function_called)
            {
                f.parenthesis_open = 1;
                push(&stack_op, '(');
                // previous ( multiple parenthesis
                if (f.parenthesis_open == 1){ 
                    f.multiple_parenthesis = 1;
                    counter_multiple_parenthesis += 1;
                }
                f.parenthesis_open = 1;
            }
            else{
                // previous ( multiple parenthesis
                if (f.parenthesis_func == 1){
                    f.multiple_parenthesis = 1;
                    counter_multiple_parenthesis += 1;
                }
                push(&stack_op, '(');
                f.parenthesis_func = 1;
            }
            break;
        case ')':
            if (!f.function_called){
                if (!f.parenthesis_open)
                    error("ERROR. Closing brackets without opening.");
                f.parenthesis_closed = 1;
                f.last_digit = 0;
            }
            else{
                if (f.f_number)
                    push(&stack_out, temp);
                if (f.parenthesis_func && f.operation_func){
                    f.f_number = 0;
                    f.last_digit = 0;
                    while (peek(&stack_op) != '(')
                        perfom_operation(&stack_out, &stack_op);
                    if (!f.multiple_parenthesis){
                        f.operation_func = 0;
                        f.parenthesis_func = 0;
                    }
                    else if (counter_multiple_parenthesis == 0){
                        f.multiple_parenthesis = 0;
                        f.parenthesis_func = 0;
                    }
                    else
                        counter_multiple_parenthesis -= 1;
                    // if (!is_empty(stack_op) && peek(&stack_op) == '(' && counter_multiple_parenthesis == 0 && multiple_parenthesis)
                    //     error("Opened bracket inside function weren't closed");
                }
                if (peek(&stack_op) == '(')
                    pop(&stack_op); // double checking before deleting
                if (!f.multiple_parenthesis){
                    perfom_one_parameter_function(&stack_out, functions);
                    counter_for_letters_in_func = 0;
                    memset(functions, 0, sizeof functions);
                    f.function_name = 0;
                    f.function_called = 0;
                    f.last_digit = 0;
                    temp = 0;
                    decimal_number = 0;
                    f.divide_digits = 1;
                    f.parenthesis_func = 0;
                }
            }
            break;
            default:
                error("Error. No such operator found.");
                exit(0);
                break;
            }
}
//operate from the Stack
void perfom_operation(struct StackNode **stack_out, struct StackNode **stack_op){
    double num1 = peek(&(*stack_out));
    pop(&(*stack_out));

    double num2 = peek(&(*stack_out));
    pop(&(*stack_out));

    char operation = peek(&(*stack_op));
    pop(&(*stack_op));

    double result = do_the_math(num1, num2, operation, &(*stack_op));
    push(&(*stack_out), result);
}

void perfom_one_parameter_function(struct StackNode **stack_out, char function[]) 
{                                                                                    
    double number = peek(&(*stack_out));
    pop(&(*stack_out));
    if (!strcmp(function, "cos"))
        number = cos(number);
    else if (!strcmp(function, "sin"))
        number = sin(number);
    else if (!strcmp(function, "tan"))
        number = tan(number);
    else if (!strcmp(function, "log"))
    {
        if (number < 0)
            error("ERROR. Log call on negative number");
        number = log10(number);
    }
    else if (!strcmp(function, "exp"))
        number = exp(number);
    else if (!strcmp(function, "sqrt"))
    {
        if (number < 0)
            error("ERROR. SQRT call on negative number");
        number = sqrt(number);
    }
    else
        error("ERROR. Unknown function name.");
    push(&(*stack_out), number);
}
//read from the file.
int handle_file(){
    char avaliable_functions[] = "sin, cos, tan, log, exp, sqrt";
    //PREZENTACE
    if (!(fp = fopen(priklad, "rb")))
        error("ERROR. File not found!");
    
    //Read line until EOF
    while ((read = getc(fp)) != EOF){
        printf("%c", read);
        //2+5, 13-4
        if (isdigit(read)){
            f.character_is_operator = 0;
            if (f.space_after_number && !f.last_operator)
                error("ERROR. Number after number without operator");
            f.space_after_number = 0;
            f.parenthesis_open = 0;
            f.last_operator = 0; 
            if (f.last_digit){
                if (f.decimal_number){
                    f.divide_digits *= 10;
                    number_remainder = read - '0';
                    number_remainder /= f.divide_digits;
                    temp += number_remainder;
                }
                else
                    temp = (temp * 10) + read - '0'; 
            }
            else{
                temp = read - '0';
                if (f.negative_number){
                    temp *= -1;
                    f.negative_number = 0;
                }

                f.last_digit = 1;
                f.f_number = 1;
            }
        }
        // cos sin tan log exp sqrt
        else if (isalpha(read)){
            if (!f.last_operator && !f.character_is_operator && counter_for_letters_in_func == 0)
                error("Function doesn't have operator before to pefrom");
            if (!strchr(avaliable_functions, read))
                error("ERROR. Undefined character.");
            if (read == 't' && !strcmp(functions, "sqr")){
                f.function_sqrt = 1;
                f.function_name = 0;
            }
                
            functions[counter_for_letters_in_func] = read;
            counter_for_letters_in_func++;
            f.last_operator = 0;

            if (f.function_name)
                error("Function called without resulting previous");
            if (counter_for_letters_in_func == 4 && f.function_sqrt)
                f.function_name = 1;
            else if (counter_for_letters_in_func == 3 && !f.function_sqrt)
                f.function_name = 1; 
            f.function_called = 1;
            f.last_digit = 0;
            f.f_number = 0;
        }
        //decide which operand to perform
        else{
            decide_operation(read);
        }
        if (!f.last_digit && temp)
        {
            push(&stack_out, temp);
            temp = 0;
            decimal_number = 0;
            f.divide_digits = 1; // reset
        }

        if (f.parenthesis_open && f.parenthesis_closed)
        {
            while (peek(&stack_op) != '(')
                perfom_operation(&stack_out, &stack_op);
            if (peek(&stack_op) == '(')
                pop(&stack_op); // double checking before deleting
            else
                exit(-1);
            f.parenthesis_closed = 0;
            if (!f.multiple_parenthesis)
                f.parenthesis_open = 0;
            else if (counter_multiple_parenthesis == 0)
            {
                f.multiple_parenthesis = 0;
                f.parenthesis_open = 0;
            }
            else
                counter_multiple_parenthesis -= 1;
        }
    }
    printf("Read succesful.\n");
    if (f.last_operator) // checking for last operator 
        error("ERROR. Last operator without number");
    if (f.parenthesis_open && !f.parenthesis_closed)
        error("ERROR. Opened brackets weren't closed");
    if (f.decimal_number)
        error("ERORR. Number with '.' doesn't have fractional part");
    // push(&stack_out, pop(&stack_op));
    while (!is_empty(stack_op))
        perfom_operation(&stack_out, &stack_op);
    printf("\n");
    res = pop(&stack_out);
    if (!is_empty(stack_out) || !is_empty(stack_op))
        error("ERROR.Something went wrong. Check your expression");
    printf("Final result is %.3f\n", res);
    printf("Output stack is Empty? %d\n", is_empty(stack_out));
    printf("Operator stack is Empty? %d\n", is_empty(stack_op));
    fclose(fp);
    free(priklad);
    return 1;
}
//Decide which operation to run based on input
void Arithmetic_Operation(int argc, char **argv){
    struct StackNode *stack_out = NULL;
    struct StackNode *stack_op = NULL;
    //Variables
    //double temp, number_remainder, res = 0;
    char functions[5];
    char log_functions[64] = "sin, cos, tan, log, exp, sqrt";
    //Counters
    int counter_multiple_parenthesis = 0;
    int divide_digits = 1;
    int counter_for_letters_in_func = 0;
    //Flags
    Flag structFl;
    //allocate size for file passed
    allocate_file(argc,argv);
    //check if file is valid
    handle_file();
    //printf("continue");
    

}