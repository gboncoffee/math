#include"compiler.h"

signed long *compile(ASTNode *root);
signed long *parse_and_compile(char *exp);
int printmatherr(int error);
ASTNode *parse(char *exp);
int eval(char *exp);
