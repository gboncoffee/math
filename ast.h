#include"mathvm.h"

typedef struct ASTNode {
    struct ASTNode *left;
    struct ASTNode *right;
    signed long num;
    signed long operation;
} ASTNode;

ASTNode *create_node(signed long operation, signed long num, ASTNode *left, ASTNode *right);
int discard_tree(ASTNode *node);

#define create_empty_node() create_node(OP_NULL, 0, NULL, NULL)
