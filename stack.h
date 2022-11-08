//https://www.youtube.com/watch?v=8nix1eqoMHM
//push – Adds or pushes an element into the stack.
//pop – Removes or pops an element out of the stack.
//peek – Gets the top element of the stack but doesn’t remove it.
//is_empty – Tests if the stack is empty.
struct StackNode
{
    double data;
    struct StackNode *nextPtr; 
};

struct StackNode *create_new_node(double data){
    //allocate new size for each node
    struct StackNode *new_node = (struct StackNode *)malloc(sizeof(struct StackNode)); 
    new_node->data = data;
    new_node->nextPtr = NULL;
    return new_node;
}

int is_empty(struct StackNode *top){
    return !top;
}

void push(struct StackNode **top, double data){
    struct StackNode *new_node = create_new_node(data);
    new_node->nextPtr = *top;
    *top = new_node; 
}

double pop(struct StackNode **top){ 
    if (is_empty(*top))
        exit(-1);
    struct StackNode *temp = *top;
    double popped = temp->data;
    *top = (*top)->nextPtr; 
    free(temp);
    return popped;
}
//check next prvek
double peek(struct StackNode **top){ 
    if (is_empty(*top))
        exit(-1);
    double data = (*top)->data;
    return data;
}