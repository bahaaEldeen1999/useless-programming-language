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

int set_data_value(Data *data, int datatype, int datai, float dataf, char datac, char *datas)
{
    if (datatype == INT)
    {

        data->int_ = datai;
        return INT;
    }
    else if (datatype == FLOAT)
    {
        data->float_ = dataf;
        return FLOAT;
    }
    else if (datatype == BOOL)
    {
        data->bool_ = datai;
        return BOOL;
    }
    else if (datatype == CHAR)
    {
        data->char_ = datac;
        return CHAR;
    }
    else if (datatype == STRING)
    {

        strcpy(data->string_, datas);
        return STRING;
    }
    return -1;
}

int set_data_value_from_other(Data *data, int datatype, Data data2)
{
    if (datatype == INT)
    {

        data->int_ = data2.int_;
    }
    else if (datatype == FLOAT)
    {
        data->float_ = data2.float_;
    }
    else if (datatype == BOOL)
    {
        data->bool_ = data2.bool_;
    }
    else if (datatype == CHAR)
    {
        data->char_ = data2.char_;
    }
    else if (datatype == STRING)
    {

        strcpy(data->string_, data2.string_);
    }
    return 0;
}

float get_math_value(Data data, int datatype)
{
    if (datatype == INT)
    {

        return (float)data.int_;
    }
    else if (datatype == FLOAT)
    {
        return (float)data.float_;
    }
    else if (datatype == BOOL)
    {
        return (float)data.bool_;
    }
    else if (datatype == CHAR)
    {
        return (float)data.char_;
    }
    // error
}

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
        set_data_value(&symbol_table[id]->data, datatype, datai, dataf, datac, datas);
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
    set_data_value(&symbol->data, datatype, datai, dataf, datac, datas);
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
    set_data_value(&a->data, datatype, datai, dataf, datac, datas);
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

int compatible_types(int datatype1, int datatype2, int is_mod)
{
    if (is_mod && (datatype1 != INT || datatype2 != INT))
        return 0;
    if ((datatype1 == STRING || datatype2 == STRING))
        return 0;
    return 1;
}

int eval_math_ops(int op, Data *v, float num1, float num2)
{
    switch (op)
    {
    case PLUS:
        return set_data_value(v, FLOAT, 0, num1 + num2, 0, NULL);
        break;
    case MINUS:
        return set_data_value(v, FLOAT, 0, num1 - num2, 0, NULL);
        break;
    case MUL:
        return set_data_value(v, FLOAT, 0, num1 * num2, 0, NULL);
        break;
    case DIV:
        return set_data_value(v, FLOAT, 0, num1 / num2, 0, NULL);
        break;
    case MOD:
        return set_data_value(v, INT, (int)num1 % (int)num2, 0, 0, NULL);
        break;
    case GT:
        return set_data_value(v, INT, num1 > num2, 0, 0, NULL);
        break;
    case GTE:
        return set_data_value(v, INT, num1 >= num2, 0, 0, NULL);
        break;
    case LT:
        return set_data_value(v, INT, num1 < num2, 0, 0, NULL);
        break;
    case LTE:
        return set_data_value(v, INT, num1 <= num2, 0, 0, NULL);
        break;
    case NOTEQ:
        return set_data_value(v, INT, num1 != num2, 0, 0, NULL);
        break;
    case EQ:
        return set_data_value(v, INT, num1 == num2, 0, 0, NULL);
        break;
    case NOT:
        return set_data_value(v, INT, !num1, 0, 0, NULL);
        break;
    case AND:
        return set_data_value(v, INT, num1 && num2, 0, 0, NULL);
        break;
    case OR:
        return set_data_value(v, INT, num1 || num2, 0, 0, NULL);
        break;

    default:
        break;
    }
}
Data eval(struct ASTNode *a, int *datatype)
{
    Data v;
    switch (a->nodetype)
    {
    case LIST:
    {
        v = eval(a->l, datatype);
        if (a->r)
            v = eval(a->r, datatype);
        break;
    }
    case CONSTANT:
    {
        struct DataNode *t = (struct DataNode *)a;
        set_data_value_from_other(&v, t->datatype, t->data);
        *datatype = t->datatype;
        break;
    }
    case VARIABLE:
    {
        struct VariableDataNode *t = (struct VariableDataNode *)a;
        struct node *symbol = get_symbol(t->var_name_);
        if (!symbol)
        {
            // error;
            printf("no symbol \n");
            *datatype = -1;
            return v;
        }
        set_data_value_from_other(&v, symbol->type, symbol->data);
        *datatype = symbol->type;
        break;
    }
    case PLUS:
    case MINUS:
    case MUL:
    case DIV:
    case GT:
    case GTE:
    case LT:
    case LTE:
    case EQ:
    case NOTEQ:
    case AND:
    case OR:
    {
        int datatype1 = -1;
        int datatype2 = -1;
        Data v1 = eval(a->l, &datatype1);
        Data v2 = eval(a->r, &datatype2);
        if (compatible_types(datatype1, datatype2, 0))
        {
            float num1 = get_math_value(v1, datatype1);
            float num2 = get_math_value(v2, datatype2);
            *datatype = eval_math_ops(a->nodetype, &v, num1, num2);
        }
        else
        {
            // error
            v.int_ = 0;
            *datatype = -1;
        }
        break;
    }
    case MOD:
    {
        int datatype1 = -1;
        int datatype2 = -1;
        Data v1 = eval(a->l, &datatype1);
        Data v2 = eval(a->r, &datatype2);
        if (compatible_types(datatype1, datatype2, 1))
        {
            int num1 = (int)get_math_value(v1, datatype1);
            int num2 = (int)get_math_value(v2, datatype2);
            *datatype = eval_math_ops(a->nodetype, &v, num1, num2);
        }
        else
        {
            // error
            v.int_ = 0;
            *datatype = -1;
        }
        break;
    }
    case NOT:
    {
        int datatype1 = -1;
        Data v1 = eval(a->l, &datatype1);
        if (compatible_types(datatype1, FLOAT, 0))
        {
            int num1 = (int)get_math_value(v1, datatype1);
            *datatype = eval_math_ops(a->nodetype, &v, num1, 0);
        }
        else
        {
            // error
            v.int_ = 0;
            *datatype = -1;
        }
        break;
    }
    case DECLARE:
    {
        struct VariableDataNode *t = (struct VariableDataNode *)a;
        if (t->is_assign || t->is_const)
        {
            v = eval(t->expr, datatype);
        }
        else
        {
            v.int_ = 0;
            *datatype = t->datatype;
        }
        switch (*datatype)
        {
        case INT:
            declare_variable(t->var_name_, t->datatype, t->is_const, t->is_assign, v.int_, v.int_, v.int_, NULL);
            break;
        case FLOAT:
            declare_variable(t->var_name_, t->datatype, t->is_const, t->is_assign, v.float_, v.float_, v.float_, NULL);
            break;
        case BOOL:
            declare_variable(t->var_name_, t->datatype, t->is_const, t->is_assign, v.bool_, v.bool_, v.bool_, NULL);
            break;
        case CHAR:
            declare_variable(t->var_name_, t->datatype, t->is_const, t->is_assign, v.char_, v.char_, v.char_, NULL);
            break;
        case STRING:
            declare_variable(t->var_name_, t->datatype, t->is_const, t->is_assign, 0, 0, 0, v.string_);
            break;

        default:
            break;
        }
        //*datatype = t->datatype;
        // printf("datatype %d\n", *datatype);

        // declare_variable(t->var_name_, t->datatype, t->is_const, t->is_assign, v.int_, v.float_, v.char_, NULL);
        break;
    }
    case ASSIGN:
    {
        struct VariableDataNode *t = (struct VariableDataNode *)a;

        v = eval(t->expr, datatype);
        switch (*datatype)
        {
        case INT:
            assign_value(t->var_name_, v.int_, v.int_, v.int_, NULL);
            break;
        case FLOAT:
            assign_value(t->var_name_, v.float_, v.float_, v.float_, NULL);
            break;
        case BOOL:
            assign_value(t->var_name_, v.bool_, v.bool_, v.bool_, NULL);
            break;
        case CHAR:
            assign_value(t->var_name_, v.char_, v.char_, v.char_, NULL);
            break;
        case STRING:
            assign_value(t->var_name_, 0, 0, 0, v.string_);
            break;

        default:
            break;
        }

        break;
    }
    case IF:
    {
        struct flowControlNode *t = (struct flowControlNode *)a;
        printf("in if\n");
        v = eval(t->exprBool, datatype);
        if (get_math_value(v, *datatype))
        {
            printf("in then \n");
            if (t->thenStmt)
                eval(t->thenStmt, datatype);
            v.int_ = 1;
        }
        else
        {

            if (t->elseStmt)
            {
                printf("in else\n");
                eval(t->elseStmt, datatype);
            }
            v.int_ = 0;
        }
    }
    default:
        break;
    }
    return v;
}
