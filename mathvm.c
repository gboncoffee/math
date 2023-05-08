#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include"mathvm.h"
#include"stack.h"

/*
 * LICENSE {{{
 * Copyright 2022 © Gabriel G. de Brito
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of 
 * this software and associated documentation files (the “Software”), to deal 
 * in the Software without restriction, including without limitation the rights to 
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
 * of the Software, and to permit persons to whom the Software is furnished to do 
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 * }}}
 */

/*
 * Stack-based machine to perform arithmetic operations
 */

// stack should only be touched by push_vm() and pop_vm() so it can be handled properly
// easily, but NO WAY SOMEONE WILL NEED MORE THAN 1 FUCKING KILOBYTE
signed long *text;
signed long *ip;
Stack *stack;

/*
 * author Gabriel G. de Brito gabrielgbrito@icloud.com
 * version 1.0.0
 * since Jun 22, 2022
 */

int push_vm(signed long value) {

    if (push(stack, value) != 0) {
        int errv = errno;
        fprintf(stderr, "ERROR: Cannot increase stack: %m\n");
        exit(errv);
    }

    return(errno);
}

int pop_vm(signed long *pop_var) {

    if (pop(stack, pop_var) != 0) {
        int errv = errno;
        fprintf(stderr, "ERROR: Cannot pop from empty stack: %m\n");
        exit(errv);
    }

    return(errno);
}

signed long run(void) {
#ifdef DEBUG_VM
    printf("REACHED RUN:\n");
    printf("text: %p\n", (void *) text);
    printf("ip: %p\n", (void *) ip);
    printf("stack: %p\n", (void *) stack->stack);
    printf("dp: %p\n", (void *) stack->dp);
    printf("\n");
#endif

    signed long x, y;
    while (*ip != EXIT) {
        switch (*ip) {
            case ADD:
                pop_vm(&x);
                pop_vm(&y);
                push_vm(y + x);
#ifdef DEBUG_VM
                printf("push_vming on sum: %lu\n", x + y);
                stack_dump(stack);
#endif
                break;
            case SUB:
                pop_vm(&x);
                pop_vm(&y);
                push_vm(y - x);
#ifdef DEBUG_VM
                printf("push_vming on sub: %lu\n", y - x);
                stack_dump(stack);
#endif
                break;
            case MUL:
                pop_vm(&x);
                pop_vm(&y);
                push_vm(y * x);
#ifdef DEBUG_VM
                printf("push_vming on mul: %lu\n", y * x);
                stack_dump(stack);
#endif
                break;
            case DIV:
                pop_vm(&x);
                pop_vm(&y);
                if (x == 0) {
                    fprintf(stderr, "ERROR: Division by zero. Aborting.");
                    return((errno = ENOTSUP));
                }
                push_vm(y / x);
#ifdef DEBUG_VM
                printf("push_vming on div: %lu\n", y / x);
                stack_dump(stack);
#endif
                break;
            case PUSH:
                ip++;
                push_vm(*ip);
#ifdef DEBUG_VM
                printf("push_vming: %lu\n", *ip);
                stack_dump(stack);
#endif
                break;
            default:
                fprintf(stderr, "ERROR: Invalid instruction %lux\n", *ip);
                return((errno = ENOTSUP));
        }

        ip++;
    }

    signed long result = *stack->dp;
    free(stack->stack);
    free(stack);

    return(result);
}

// inits the machine. text needs to be ONLY code, get rid of the file
// identificator before calling it.
signed long init(signed long *_text) {

    stack = stack_init(STACK_INIT_SIZE);

    text = _text;
    ip = text;

    return(run());
}

#ifdef DEBUG_VM
int main(void) {
    // 2 . (20 - 5) / (2 + 1)
    signed long text[] = {
        '<', 2, '<', 20, '<', 5,
        '-',
        '*',
        '<', 2, '<', 1,
        '+',
        '/',
        EOF
    };
    printf("%lu\n", init(text));
}
#endif
