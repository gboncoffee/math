#include"ast.h"
#include"mathvm.h"

// tokens
#define TOKEN_ADD     ADD
#define TOKEN_SUB     SUB
#define TOKEN_MUL     MUL
#define TOKEN_DIV     DIV
#define TOKEN_OPEN_B  '('
#define TOKEN_CLOSE_B ')'

// compilation errors
#define SUCCESS_COMPILATION          0
#define ERR_EXPECTED_CLOSING_BRACKET 1
#define ERR_EXPECTED_OPENING_BRACKET 2
#define ERR_EXPECTED_OPERATOR        3
#define ERR_EXPECTED_OPERAND         4

// array of arrays with the operation precedence BACKWARDS
static char precedence[][3] = {
    { TOKEN_ADD, TOKEN_SUB, '\0' },
    { TOKEN_DIV, TOKEN_MUL, '\0' },
    { '\0' },
};

int inner_parse(char *exp, ASTNode **base);
int init_compile(ASTNode *root, signed long *text);
signed long *inner_compile(ASTNode *root, signed long *text);
signed long ast_text_size(ASTNode *root, signed long size);

#define get_ast_text_size(root) \
    ast_text_size(root, 0)
