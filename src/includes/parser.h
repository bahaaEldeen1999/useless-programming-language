#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#define MAX_NAME_LENGTH 20
#define MAX_VARS 1000
#define MAX_STRING 1000
// 1 => int
// 2 => float
// 3 => char/
// 4 => bool
// 5 => string
enum datatypes
{
    INT,
    FLOAT,
    CHAR,
    BOOL,
    STRING
};
typedef union
{
    int int_;
    float float_;
    char char_;
    char string_[MAX_STRING];
    int bool_;
} Data;

struct node
{
    char name[MAX_NAME_LENGTH];
    int type;
    int id;
    int is_const;
    Data data;
};

static struct node *symbol_table[MAX_VARS];

struct node *get_symbol(char *var_name)
{
    for (int i = 0; i < MAX_VARS; i++)
    {
        if (symbol_table[i] && (strcmp(symbol_table[i]->name, var_name) == 0))
        {
            return symbol_table[i];
        }
    }
    return NULL;
}

int declare_variable(char *var_name, int datatype, int is_const, int is_assign, int datai, float dataf, char datac, char *datas)
{

    // check empty position
    int id = -1;
    for (int i = 0; i < MAX_VARS; i++)
    {
        if (!symbol_table[i])
        {
            id = i;
            break;
        }
    }
    // check not declared before
    if (get_symbol(var_name))
    {
        // error declared beofer
        return -1;
    }

    symbol_table[id] = malloc(sizeof(*symbol_table[id]));
    strcpy(symbol_table[id]->name, var_name);
    symbol_table[id]->id = id;
    symbol_table[id]->type = datatype;
    symbol_table[id]->is_const = is_const;

    //  if const then must assign ,
    //  if not const then assign is optional
    if (is_const || is_assign)
    {
        if (datatype == INT)
        {

            symbol_table[id]->data.int_ = datai;
        }
        else if (datatype == FLOAT)
        {
            symbol_table[id]->data.float_ = dataf;
        }
        else if (datatype == BOOL)
        {
            symbol_table[id]->data.bool_ = datai;
        }
        else if (datatype == CHAR)
        {
            symbol_table[id]->data.char_ = datac;
        }
        else if (datatype == STRING)
        {

            strcpy(symbol_table[id]->data.string_, datas);
        }
    }

    return 0;
}

int assign_value(char *var_name, int datai, float dataf, char datac, char *datas)
{
    struct node *symbol = get_symbol(var_name);
    if (!symbol || symbol->is_const)
    {
        // no variable declared
        // variable is const cant assign
        return -1;
    }
    int datatype = symbol->type;
    if (datatype == INT)
    {

        symbol->data.int_ = datai;
    }
    else if (datatype == FLOAT)
    {
        symbol->data.float_ = dataf;
    }
    else if (datatype == BOOL)
    {
        symbol->data.bool_ = datai;
    }
    else if (datatype == CHAR)
    {
        symbol->data.char_ = datac;
    }
    else if (datatype == STRING)
    {

        strcpy(symbol->data.string_, datas);
    }
    return 0;
}

enum NodeTypes
{
    PLUS,
    MINUS,
    MUL,
    DIV,
    MOD,
    GT,
    GTE,
    LT,
    LTE,
    EQ,
    NOTEQ,
    ASSIGN,
    AND,
    OR,
    NOT,
    CONSTANT,
    VARIABLE,
    LIST,
    DECLARE,
    IF,
};

struct ASTNode
{
    int nodetype;
    struct ASTNode *l;
    struct ASTNode *r;
};

struct DataNode
{
    int nodetype;
    int datatype;
    Data data;
};

struct VariableDataNode
{
    int nodetype;
    char *var_name_[MAX_NAME_LENGTH];
    int datatype;
    struct ASTNode *expr;
    int is_const;
    int is_assign;
};

struct flowControlNode
{
    int nodetype;
    struct ASTNode *exprBool;
    struct ASTNode *thenStmt;
    struct ASTNode *elseStmt;
};

struct ASTNode *newASTNode(int nodetype, struct ASTNode *l, struct ASTNode *r)
{
    struct ASTNode *a = malloc(sizeof(struct ASTNode));

    if (!a)
    {
        yyerror("out of space");
        exit(0);
    }
    a->nodetype = nodetype;
    a->l = l;
    a->r = r;
    // printf("vvvvv %d \n", ((struct DataNode *)l)->data.int_);
    // printf("vvvvv %d \n", ((struct DataNode *)r)->data.int_);
    return a;
}

struct ASTNode *newDataNode(int nodeType, int datatype, int datai, float dataf, char datac, char *datas)
{
    struct DataNode *a = malloc(sizeof(struct DataNode));
    a->nodetype = nodeType;
    a->datatype = datatype;
    if (datatype == INT)
    {
        a->data.int_ = datai;
    }
    else if (datatype == FLOAT)
    {
        a->data.float_ = dataf;
    }
    else if (datatype == BOOL)
    {
        a->data.bool_ = datai;
    }
    else if (datatype == CHAR)
    {
        a->data.char_ = datac;
    }
    else if (datatype == STRING)
    {
        strcpy(a->data.string_, datas);
    }
    return (struct ASTNode *)a;
}

struct ASTNode *newVariableDataNode(int nodeType, char *var_name_, int datatype, int is_const, int is_assign, struct ASTNode *expr)
{
    struct VariableDataNode *a = malloc(sizeof(struct VariableDataNode));
    a->nodetype = nodeType;
    strcpy(a->var_name_, var_name_);
    a->expr = expr;
    a->is_assign = is_assign;
    a->is_const = is_const;
    a->datatype = datatype;
    return (struct ASTNode *)a;
}

struct ASTNode *newFlowControlNode(int nodetype, struct ASTNode *exprBool, struct ASTNode *thenStmt, struct ASTNode *elseStmt)
{
    struct flowControlNode *a = malloc(sizeof(struct flowControlNode));

    if (!a)
    {
        // yyerror("out of space");
        exit(0);
    }
    a->nodetype = nodetype;
    a->exprBool = exprBool;
    a->thenStmt = thenStmt;
    a->elseStmt = elseStmt;
    return (struct ASTNode *)a;
}

double eval(struct ASTNode *a)
{
    double v;
    switch (a->nodetype)
    {

    case CONSTANT:
    {
        struct DataNode *t = (struct DataNode *)a;
        switch (t->datatype)
        {
        case INT:
            v = t->data.int_;
            break;
        case FLOAT:
            v = t->data.float_;
            break;
        case BOOL:
            v = t->data.bool_;
            break;

        default:
            break;
        }

        break;
    }
    case VARIABLE:
    {
        struct VariableDataNode *t = (struct VariableDataNode *)a;
        struct node *symbol = get_symbol(t->var_name_);
        if (!symbol)
        {
            // error;
        }
        // printf("t datatype %d\n", symbol->type);
        switch (symbol->type)
        {
        case INT:
            v = symbol->data.int_;
            break;
        case FLOAT:
            v = symbol->data.float_;
            break;
        case BOOL:
            v = symbol->data.bool_;
            break;

        default:
            break;
        }

        break;
    }
    case PLUS:
    {
        v = eval(a->l) + eval(a->r);
        break;
    }
    case MINUS:
    {
        v = eval(a->l) - eval(a->r);
        break;
    }
    case MUL:
    {
        v = eval(a->l) * eval(a->r);
        break;
    }
    case DIV:
    {
        v = eval(a->l) / eval(a->r);
        break;
    }
    case MOD:
    {
        v = (int)eval(a->l) % (int)eval(a->r);
        break;
    }
    case GT:
    {
        v = (int)(eval(a->l) > eval(a->r));
        break;
    }
    case GTE:
    {
        v = (int)(eval(a->l) >= eval(a->r));
        break;
    }
    case LT:
    {
        v = (int)(eval(a->l) < eval(a->r));
        break;
    }
    case LTE:
    {
        v = (int)(eval(a->l) <= eval(a->r));
        break;
    }
    case NOTEQ:
    {
        v = (int)(eval(a->l) != eval(a->r));
        break;
    }
    case EQ:
    {
        v = (int)(eval(a->l) == eval(a->r));
        break;
    }
    case AND:
    {
        v = (int)(eval(a->l) && eval(a->r));
        break;
    }
    case NOT:
    {
        v = (int)(!eval(a->l));
        break;
    }
    case OR:
    {
        v = (int)(eval(a->l) || eval(a->r));
        break;
    }
    case DECLARE:
    {
        struct VariableDataNode *t = (struct VariableDataNode *)a;
        if (t->is_assign || t->is_const)
        {
            v = eval(t->expr);
        }
        else
        {
            v = 0;
        }
        declare_variable(t->var_name_, t->datatype, t->is_const, t->is_assign, v, v, 0, NULL);
        break;
    }
    case ASSIGN:
    {
        struct VariableDataNode *t = (struct VariableDataNode *)a;

        v = eval(t->expr);
        assign_value(t->var_name_, (int)v, (float)v, 0, NULL);
        break;
    }
    default:
        break;
    }
    return v;
}