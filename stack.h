typedef struct Stack {
    signed long *dp;
    signed long *stack;
    signed long stack_size;
    signed long stack_init_size;
} Stack;

int push(Stack *stack, signed long value);
int pop(Stack *stack, signed long *var);
int stack_dump(Stack *stack);
Stack *stack_init(signed long init_size);
