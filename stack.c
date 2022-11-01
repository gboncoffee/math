#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include"stack.h"

/*
 * author Gabriel G. de Brito gabrielgbrito@icloud.com
 * version 1.0.0
 * since Jun 27, 2022
 */

// stack implementation for the MaTH virtual machine and compiler

int stack_dump(Stack *stack) {

    for (signed long *pt = stack->stack; pt <= stack->dp; pt++)
        printf("%lu ", *pt);

    printf("\n");

    return(errno);
}

// pops something from stack. returns EADDRNOTAVAIL if empty stack
int pop(Stack *stack, signed long *var) {

    if (stack->dp >= stack->stack) {
        *var = *stack->dp;
        stack->dp--;
    } else {
        errno = EADDRNOTAVAIL;
        return(errno);
    }

    return(errno);
}

// pushes something to stack
int push(Stack *stack, signed long value) {

    stack->dp++;
    if ((stack->dp - stack->stack) > stack->stack_size) {
        stack->stack_size += stack->stack_init_size;
        stack->stack = realloc(stack->stack, stack->stack_size);
#ifdef DEBUG_STACK
        printf("Reallocing stack %p\n", stack->stack);
#endif

        if (stack->stack == NULL) {
            return(errno);
        }

        stack->dp = &stack->stack[stack->stack_size - stack->stack_init_size + 1];
    }
    *stack->dp = value;

    return(errno);
}

Stack *stack_init(signed long init_size) {

    Stack *stack = (Stack*) malloc(sizeof(Stack));
    stack->stack = (signed long*) malloc(init_size * sizeof(signed long));
    stack->dp = stack->stack - 1;
    stack->stack_init_size = init_size;

    return(stack);
}
