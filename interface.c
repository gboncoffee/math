#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include"compiler.h"
#include"mathvm.h"

/*
 * author Gabriel G. de Brito gabrielgbrito@icloud.com
 * version 0.0.0
 * since Jul 30, 2022
 */

/*
 * Ez interface for compiling expressions and running the math vm
 */

// table of parsing/compiling errors
int printmatherr(int error) {

#define printerr(msg) \
    printf("ERROR: %s\n", msg)

    switch(error) {
        case ERR_EXPECTED_CLOSING_BRACKET:
            printerr("Expected closing bracket");
            break;
        case ERR_EXPECTED_OPENING_BRACKET:
            printerr("Expected opening bracket");
            break;
        case ERR_EXPECTED_OPERATOR:
            printerr("Expected operator");
            break;
        case ERR_EXPECTED_OPERAND:
            printerr("Expected operand");
            break;
        default:
            printf("Math error table: No such error: %d\n", error);
            errno = -1;
    }

    return(errno);
}

// ez parsing interface, returning NULL if error
ASTNode *parse(char *exp) {

#ifdef DEBUG_EVAL
    printf("Parsing %s\n", exp);
#endif

    ASTNode *root;
    if (inner_parse(exp, &root)) 
        return(NULL);

    return(root);
}

// ez compiling interface, returning NULL if error
signed long *compile(ASTNode *root) {

    signed long *text = (signed long*) malloc(
        sizeof(signed long) * get_ast_text_size(root) + 1
    );

    if (init_compile(root, text))
        return(NULL);

    return(text);
}

// ez parsing and compiling, returning NULL if any error (need to check errno
// for better error handling)
signed long *parse_and_compile(char *exp) {

    ASTNode *root = parse(exp);

    if (root == NULL)
        return(NULL);

    signed long *text = compile(root);

    if (text == NULL)
        return(NULL);

    return(text);
}

// ez interface for everything lol
//
// parses, compiles and evaluates the string
int eval(char *exp) {

#ifdef DEBUG_EVAL
    printf("Evaluating %s\n", exp);
#endif

    signed long *text = parse_and_compile(exp);
    if (text == NULL) {
        printmatherr(errno);

    } else {
        signed long r = init(text);
        if (!errno)
            printf("%ld", r);
        printf("\n");
    }

    return(errno);
}

#ifdef DEBUG_EVAL
int main() {
    return(eval("2*(20-5)/(2+1)"));
}
#endif
