#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include"stack.h"
#include"mathvm.h"
#include"compiler.h"
#define MODE_NULL     0
#define MODE_OPERATOR 1
#define MODE_NUMBER   2

#define syntaxerr(msg) \
    fprintf(stderr, "Syntax error: %s.\n", msg);\
    errno = ENOTSUP; \
    return(errno);

/*
 * author Gabriel G. de Brito gabrielgbrito@icloud.com
 * version 0.0.0
 * since Jun 27, 2022
 */

/*
 * we need to keep track of a lot of syntax errors, for example:
 *
 * 20 +         operator without number
 * + 20         operator withour number again
 * 20 . (       unclosed bracket
 * 20 )         unopened bracket
 * 20 a 2       unknown operator
 *
 * what will actually happen is that "operator without a number" and the
 * "unknown operator" will be detected by the number parser and the brackets
 * will be solved by the parser itself.
 *
 * maybe we should implement a better number parser then strtol to improve the
 * error detection, currently something like '20 0 0' will be parsed as 20 and
 * not 2000 or something.
 */

// PARSE UTILS {{{

// returns a pointer to the end of the exp
char *find_end(char *exp) {

    char *end = exp;
    for (;*end != '\0'; end++);

    return(end);
}

char *exp_sanitize(char *exp) {

    char *end = find_end(exp);
    end--;

    for (;*exp == ' '; exp++);
    for (;*end == ' '; end--);

    if ((*exp != '(') || (*end != ')')) 
        return(exp);

    for (char *c = exp + 1; c != end; c++)
        if (*c == ')')
            return(exp);

    exp++;
    *end = '\0';

    return(exp);
}

// checks precedence. returns 0 if outside, -1 if reached prec_lvl limit and 1
// if inside
int on_precedence(char token, int prec_lvl) {

    if (precedence[prec_lvl][0] == '\0') {
        return(-1);
    }

    for (int c = 0; precedence[prec_lvl][c] != '\0'; c++)
        if (precedence[prec_lvl][c] == token)
            return(1);

    return(0);
}

// }}}

// PARSE {{{
//
// parses the exp into an ast and returns a *ASTNode
// exp must be a null-terminated string
int inner_parse(char *exp, ASTNode **base) {

    *base = create_empty_node();
    char *numend; // needed for number parsing

#ifdef DEBUG_PARSE
    printf("Reached parse for %s\n", exp);
#endif

    // sanitize the string (just remove parenthesis if exp is inside it)
    exp = exp_sanitize(exp);
    char *end = find_end(exp);

#ifdef DEBUG_PARSE
    printf("Parsing as %s\n", exp);
#endif

    int prec_lvl = 0;
    unsigned long bracket_lvl = 0;

    // ignores operator loop if exp starts with + or - (so indicating a signed
    // number)
    if ((*exp == '-') || (*exp == '+')) {
#ifdef DEBUG_PARSE
        printf("Is signed, jumping to num parse\n");
#endif
        goto num_parse;
    }

    // operators loop
    for (;;) {

        if (end < exp) {
            end = find_end(exp);
            prec_lvl++;
            // get out if reached end of precedence
            if (precedence[prec_lvl][0] == '\0')
                break;
        }

        if (*end == TOKEN_CLOSE_B) {
            bracket_lvl++;

        } else if (*end == TOKEN_OPEN_B) {
            if (bracket_lvl == 0) {
                errno = ERR_EXPECTED_CLOSING_BRACKET;
                return(errno);
            }
            bracket_lvl--;
        }

        if (bracket_lvl == 0) {

#ifdef DEBUG_PARSE
        printf("Checking precedence\n");
#endif
            // if find operator
            if (on_precedence(*end, prec_lvl)) {
                (*base)->operation = *end;
                *end = '\0';
                end++;

#ifdef DEBUG_PARSE
        printf("Found operator %c\n", (*base)->operation);
#endif

                if (inner_parse(exp, &((*base)->left))) return(errno);
                inner_parse(end, &((*base)->right));
                return(errno);
            }
        }

        end--;
    }

    if (bracket_lvl) {
        errno = ERR_EXPECTED_OPENING_BRACKET;
        return(errno);
    }

num_parse:

    // number parse
    (*base)->num = strtol(exp, &numend, 10);

    if ((errno == ERANGE)|| (!strcmp(numend, exp)))
        return((errno = ERR_EXPECTED_OPERAND));

    (*base)->operation = PUSH;

    return(errno);
}
// }}}

// COMPILE UTILS {{{
signed long ast_text_size(ASTNode *root, signed long size) {

    if (root == NULL)
        return(size);

    if (root->operation == PUSH) {
        size += 2;
    } else {
        size += 1;
    }

    size = ast_text_size(root->left,  size);
    size = ast_text_size(root->right, size);

    return(size);
}
// }}}

// COMPILE {{{
signed long *inner_compile(ASTNode *root, signed long *text) {

    if (root->operation == PUSH) {
        *text = PUSH;
        text += 1;
        *text = root->num;
        return(text += 1);
    }

    text = inner_compile(root->left, text);
    if (text == NULL) return(NULL);
    text = inner_compile(root->right, text);
    if (text == NULL) return(NULL);

    if (root->operation != PUSH) {
        *text = root->operation;
        text += 1;
    }

    return(text);
}

int init_compile(ASTNode *root, signed long *text) {

    signed long *end;
    if ((end = inner_compile(root, text)) == NULL) {
        free(text);
        return(errno);

    } else {
        *end = EXIT;
    }

#ifdef DEBUG_COMPILE
    signed long *c = text;
    do {
        switch (*c) {
            case PUSH:
                printf("%c ", *c);
                c += 1;
                printf("%ld\n", *c);
                break;
            default:
                printf("%c\n", *c);
        }
        c += 1;
    } while (*c != EXIT);
#endif

    discard_tree(root);
    return(errno);
}
// }}}

#ifdef DEBUG_PARSE
int tree_view(ASTNode *node, int pad);
int main() {
    ASTNode *root;
    if (!inner_parse("20 0*(20-5)/(2+1)", &root))
        tree_view(root, 0);
    else
        discard_tree(root);
    
    return(errno);
}
#endif

#ifdef DEBUG_COMPILE
int tree_view(ASTNode *node, int pad);

int main() {

    ASTNode *root;
    if (!inner_parse("2*(20-5)/(2+1)", &root)) {
        printf("Generated AST successfully:\n\n");
        tree_view(root, 0);
    }

    signed long *text = (signed long*) malloc(
            ast_text_size(root, 0) + 1
        );

    if (!init_compile(root, text)) {
        printf("Compiled succesfully\n\n");
    }

    return(errno);
}
#endif
