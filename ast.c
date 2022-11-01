#include<stdlib.h>
#include<errno.h>
#include"ast.h"
#include"mathvm.h"

/*
 * author Gabriel G. de Brito gabrielgbrito@icloud.com
 * version 0.0.0
 * since Jun 28, 2022
 */
ASTNode *create_node(signed long operation, signed long num, ASTNode *left, ASTNode *right) {

    ASTNode *node = (ASTNode*) malloc(sizeof(ASTNode));
    node->operation = operation;
    node->num = num;
    node->left = left;
    node->right = right;

    return(node);
}

int discard_tree(ASTNode *node) {

    if (node->left != NULL)
        discard_tree(node->left);
    if (node->right != NULL)
        discard_tree(node->right);

    free(node);

    return(errno);
} 

#if defined(DEBUG_TREE) || defined(DEBUG_PARSE) || defined(DEBUG_COMPILE)
#include<stdio.h>
int tree_view(ASTNode *node, int pad) {

    for (int padc = pad; padc != 0; padc--)
        printf("    ");
    printf("Operation: %c Num: %ld\n", node->operation, node->num);

    pad++;
    if (node->left != NULL)
        tree_view(node->left, pad);
    if (node->right != NULL)
        tree_view(node->right, pad);

    return(errno);
}

#if !defined(DEBUG_PARSE) && !defined(DEBUG_COMPILE)
int main() {

    // 2 . (20 - 5) / (2 + 1)
    ASTNode *root            = create_node(DIV,  0,  NULL, NULL);
    root->left               = create_node(MUL,  0,  NULL, NULL);
    root->left->left         = create_node(PUSH, 2,  NULL, NULL);
    root->left->right        = create_node(SUB,  0,  NULL, NULL);
    root->left->right->left  = create_node(PUSH, 20, NULL, NULL);
    root->left->right->right = create_node(PUSH, 5,  NULL, NULL);
    root->right              = create_node(ADD,  0,  NULL, NULL);
    root->right->left        = create_node(PUSH, 2,  NULL, NULL);
    root->right->right       = create_node(PUSH, 2,  NULL, NULL);

    tree_view(root, 0);
    discard_tree(root);

    return(errno);
}
#endif
#endif
