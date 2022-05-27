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
extern int yylineno; /* from lexer */
void yyerror(char *s, ...);
/*
Global variable to control debugging priting
*/
int is_debug = 0;

enum datatypes
{
    INT,
    FLOAT,
    CHAR,
    BOOL,
    STRING
};
typedef struct
{
    int int_;
    float float_;
    char char_;
    char string_[MAX_STRING];
    int bool_;
    char t[100];
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

        strcpy(data->string_, (const char *)(datas + 1));
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

        strcpy(data->string_, (const char *)data2.string_);
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
    yyerror("expression doesn't cast to numerical value\n");
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
        yyerror("variable declared before ");
        if (is_debug)
            printf("declared before %s \n", var_name);
        id = get_symbol(var_name)->id;
        return -1;
    }

    symbol_table[id] = malloc(sizeof(*symbol_table[id]));
    strcpy(symbol_table[id]->name, (const char *)var_name);
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
        yyerror("cannot assign value to constant variable\n");
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
    WHILE,
    FOR,
    REPEAT,
    SWITCH,
    BREAK,
    CONT,
    CASE,
    UMINUS,
    TOGGLE_DEBUG,
    PRINT
};

char *NodeTypesOperations[] =
    {
        "PLUS",
        "MINUS",
        "MUL",
        "DIV",
        "MOD",
        "GT",
        "GTE",
        "LT",
        "LTE",
        "EQ",
        "NOTEQ",
        "ASSIGN",
        "AND",
        "OR",
        "NOT",
        "CONSTANT",
        "VARIABLE",
        "LIST",
        "DECLARE",
        "IF",
        "WHILE",
        "FOR",
        "REPEAT",
        "SWITCH",
        "BREAK",
        "CONT",
        "CASE",
        "UMINUS",
        "TOGGLE_DEBUG",
        "PRINT"};
struct ASTNode
{
    int nodetype;
    struct ASTNode *l;
    struct ASTNode *r;
};

struct PrintNode
{
    int nodetype;
    struct ASTNode *expr;
    int datatype;
    char string[MAX_STRING];
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
    char var_name_[MAX_NAME_LENGTH];
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
    struct ASTNode *start;
    struct ASTNode *end;
    struct ASTNode *step;
};

struct CaseNode
{
    int nodetype;
    struct ASTNode *nextCase;
    struct ASTNode *expr;
    struct ASTNode *stmts;
    int is_default;
};

struct ASTNode *
newASTNode(int nodetype, struct ASTNode *l, struct ASTNode *r)
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

struct ASTNode *
newPrintNode(int nodetype, struct ASTNode *expr, int datatype, char *string)
{
    struct PrintNode *a = malloc(sizeof(struct PrintNode));

    if (!a)
    {
        yyerror("out of space");
        exit(0);
    }
    a->nodetype = nodetype;
    a->expr = expr;
    a->datatype = datatype;
    if (string)
        strcpy(a->string, (const char *)string);
    // printf("vvvvv %d \n", ((struct DataNode *)l)->data.int_);
    // printf("vvvvv %d \n", ((struct DataNode *)r)->data.int_);
    return (struct ASTNode *)a;
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
    strcpy(a->var_name_, (const char *)var_name_);
    a->expr = expr;
    a->is_assign = is_assign;
    a->is_const = is_const;
    a->datatype = datatype;
    return (struct ASTNode *)a;
}

struct ASTNode *newFlowControlNode(int nodetype, struct ASTNode *exprBool, struct ASTNode *thenStmt, struct ASTNode *elseStmt, struct ASTNode *start, struct ASTNode *end, struct ASTNode *step)
{
    struct flowControlNode *a = malloc(sizeof(struct flowControlNode));

    if (!a)
    {
        yyerror("no space left on device\n");
        exit(0);
    }
    a->nodetype = nodetype;
    a->exprBool = exprBool;
    a->thenStmt = thenStmt;
    a->elseStmt = elseStmt;
    a->start = start;
    a->end = end;
    a->step = step;
    return (struct ASTNode *)a;
}

struct ASTNode *newCaseNode(int nodetype, struct ASTNode *nextCase, struct ASTNode *expr, struct ASTNode *stmts, int is_default)
{
    struct CaseNode *a = malloc(sizeof(struct CaseNode));

    if (!a)
    {
        yyerror("no space left on device");
        exit(0);
    }
    a->nodetype = nodetype;
    a->expr = expr;
    a->nextCase = nextCase;
    a->is_default = is_default;
    a->stmts = stmts;
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

// global break and continue
int is_break = 0;
int is_cont = 0;

// global index of temp variable
unsigned int tIndx = 0;
// global label index
unsigned int labelIndx = 0;

// array of quads to hold all quads
typedef struct
{
    char op[100];
    char a[100]; // arg1
    char b[100]; // arg2
    char t[100]; // result
} Quadraple;

void printQuadraple(Quadraple q)
{
    printf("%s \t, %s \t, %s \t, %s ;\n", q.op, q.a, q.b, q.t);
}

Quadraple quadraples[10000];
int currQuad = 0;
int currSwitch = 0;

void printAllQuadraples()
{
    for (int i = 0; i < currQuad; i++)
    {
        printQuadraple(quadraples[i]);
    }
}

Data eval(struct ASTNode *a, int *datatype)
{
    Data v;
    if (is_break || is_cont)
    {
        return v;
    }
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

        Quadraple q;
        sprintf(q.op, "assign");
        switch (t->datatype)
        {
        case INT:
            sprintf(q.a, "%d", v.int_);
            break;
        case FLOAT:
            sprintf(q.a, "%f", v.float_);
            break;
        case CHAR:
            sprintf(q.a, "'%c'", v.char_);
            break;
        case BOOL:
            sprintf(q.a, "%d", v.bool_);
            break;

        default:
            break;
        }

        sprintf(q.b, "null");
        sprintf(q.t, "t%d", tIndx);
        sprintf(v.t, "t%d", tIndx);
        tIndx++;
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);

        break;
    }
    case VARIABLE:
    {
        struct VariableDataNode *t = (struct VariableDataNode *)a;
        struct node *symbol = get_symbol(t->var_name_);
        if (!symbol)
        {
            // error;
            yyerror("no variable declared with %s name\n", t->var_name_);
            if (is_debug)
                printf("no symbol \n");
            *datatype = -1;
            return v;
        }
        set_data_value_from_other(&v, symbol->type, symbol->data);
        *datatype = symbol->type;

        sprintf(v.t, "%s", symbol->name);

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
            Quadraple q;
            sprintf(q.op, NodeTypesOperations[a->nodetype]);
            sprintf(q.a, "%s", v1.t); // only compute int
            sprintf(q.b, "%s", v2.t);
            sprintf(q.t, "t%d", tIndx);
            sprintf(v.t, "t%d", tIndx);
            tIndx++;
            quadraples[currQuad] = q;
            currQuad++;
            // printQuadraple(q);
        }
        else
        {
            // error
            yyerror("cant perform expression on 2 incompatible datatypes\n");
            v.int_ = 0;
            *datatype = -1;
        }
        break;
    }
    case UMINUS:
    {
        v = eval(a->l, datatype);
        if (compatible_types(*datatype, FLOAT, 0))
        {
            float neg = -get_math_value(v, *datatype);
            set_data_value(&v, *datatype, neg, neg, neg, NULL);
            Quadraple q;
            sprintf(q.op, NodeTypesOperations[a->nodetype]);
            sprintf(q.a, "%s", v.t); // only compute int
            sprintf(q.b, "null");
            sprintf(q.t, "t%d", tIndx);
            sprintf(v.t, "t%d", tIndx);
            tIndx++;
            quadraples[currQuad] = q;
            currQuad++;
            // printQuadraple(q);
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
            Quadraple q;
            sprintf(q.op, NodeTypesOperations[a->nodetype]);
            sprintf(q.a, "%s", v1.t);
            sprintf(q.b, "%s", v2.t);
            sprintf(q.t, "t%d", tIndx);
            sprintf(v.t, "t%d", tIndx);
            tIndx++;
            quadraples[currQuad] = q;
            currQuad++;
            // printQuadraple(q);
        }
        else
        {
            // error
            yyerror("can only perfrom mod on 2 ints\n");
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
            Quadraple q;
            sprintf(q.op, NodeTypesOperations[a->nodetype]);
            sprintf(q.a, "%s", v1.t); // only compute int
            sprintf(q.b, "null");
            sprintf(q.t, "t%d", tIndx);
            sprintf(v.t, "t%d", tIndx);
            tIndx++;
            quadraples[currQuad] = q;
            currQuad++;
            // printQuadraple(q);
        }
        else
        {
            // error
            yyerror("cant apply not to non numerical datatype\n");
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

            // assume mus assign [TODO]
            Quadraple q;
            sprintf(q.op, "assign");
            sprintf(q.a, "%s", v.t); // only compute int
            sprintf(q.b, "null");
            sprintf(q.t, "%s", t->var_name_);
            sprintf(v.t, "%s", t->var_name_);
            quadraples[currQuad] = q;
            currQuad++;
            // printQuadraple(q);
        }
        else
        {
            sprintf(v.t, "%s", t->var_name_);
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

        break;
    }
    case ASSIGN:
    {
        struct VariableDataNode *t = (struct VariableDataNode *)a;

        v = eval(t->expr, datatype);
        Quadraple q;
        sprintf(q.op, "assign");
        sprintf(q.a, "%s", v.t);
        sprintf(q.b, "null");
        sprintf(q.t, "%s", t->var_name_);
        sprintf(v.t, "%s", t->var_name_);
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);
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
        if (is_debug)
            printf("in if\n");
        v = eval(t->exprBool, datatype);

        // jnt to else label
        // generate all then
        // gemerate labelelse
        // generate all else
        if (t->thenStmt)
        {
            Quadraple q;
            sprintf(q.op, "JNT");
            sprintf(q.a, "label%d", labelIndx);
            sprintf(q.b, "null");
            sprintf(q.t, "null");
            quadraples[currQuad] = q;
            currQuad++;
            // printQuadraple(q);
            eval(t->thenStmt, datatype);

            // jmp to end if
            // jump to end of if if then is executed
            sprintf(q.op, "JMP");
            sprintf(q.a, "label%d", labelIndx + 1);
            sprintf(q.b, "null");
            sprintf(q.t, "null");
            quadraples[currQuad] = q;
            currQuad++;
            // printQuadraple(q);
            //
            sprintf(q.op, "label");
            sprintf(q.a, "label%d", labelIndx);
            sprintf(q.b, "null");
            sprintf(q.t, "null");
            quadraples[currQuad] = q;
            currQuad++;
            // printQuadraple(q);
            labelIndx++;
        }
        if (t->elseStmt)
        {
            eval(t->elseStmt, datatype);
        }
        // add end if label
        Quadraple q;
        sprintf(q.op, "label");
        sprintf(q.a, "label%d", labelIndx);
        sprintf(q.b, "null");
        sprintf(q.t, "null");
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);
        labelIndx++;

        break;
    }
    case WHILE:
    {
        struct flowControlNode *t = (struct flowControlNode *)a;
        if (is_debug)
            printf("in while\n");

        // generate loop by jumps and condition
        // if exprBool then jump label

        // first add while label
        Quadraple q;
        sprintf(q.op, "label");
        sprintf(q.a, "label%d", labelIndx);
        sprintf(q.b, "null");
        sprintf(q.t, "null");
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);
        labelIndx++;

        // add expression
        v = eval(t->exprBool, datatype);

        // add JNT to end of while
        sprintf(q.op, "JNT");
        sprintf(q.a, "label%d", labelIndx);
        sprintf(q.b, "null");
        sprintf(q.t, "null");
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);

        // add while body
        eval(t->thenStmt, datatype);

        // add jmp to while start label
        sprintf(q.op, "JMP");
        sprintf(q.a, "label%d", labelIndx - 1);
        sprintf(q.b, "null");
        sprintf(q.t, "null");
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);

        // add end label of while
        sprintf(q.op, "label");
        sprintf(q.a, "label%d", labelIndx);
        sprintf(q.b, "null");
        sprintf(q.t, "null");
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);
        labelIndx++;

        break;
    }
    case REPEAT:
    {
        struct flowControlNode *t = (struct flowControlNode *)a;
        if (is_debug)
            printf("in repeat\n");

        // same as while loop only difference in jt
        // first add while label
        Quadraple q;
        sprintf(q.op, "label");
        sprintf(q.a, "label%d", labelIndx);
        sprintf(q.b, "null");
        sprintf(q.t, "null");
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);
        labelIndx++;

        // add expression
        v = eval(t->exprBool, datatype);

        // add JT to end of loop
        sprintf(q.op, "JT");
        sprintf(q.a, "label%d", labelIndx);
        sprintf(q.b, "null");
        sprintf(q.t, "null");
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);

        // add while body
        eval(t->thenStmt, datatype);

        // add jmp to loop start label
        sprintf(q.op, "JMP");
        sprintf(q.a, "label%d", labelIndx - 1);
        sprintf(q.b, "null");
        sprintf(q.t, "null");
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);

        // add end label of loop
        sprintf(q.op, "label");
        sprintf(q.a, "label%d", labelIndx);
        sprintf(q.b, "null");
        sprintf(q.t, "null");
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);
        labelIndx++;

        break;
    }
    case FOR:
    {
        struct flowControlNode *t = (struct flowControlNode *)a;
        if (is_debug)
            printf("in FOR\n");

        Quadraple q;
        // initializatio of for
        Data startData = eval(t->start, datatype);
        // add condtion label
        sprintf(q.op, "label");
        sprintf(q.a, "label%d", labelIndx);
        sprintf(q.b, "null");
        sprintf(q.t, "null");
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);
        // condition
        Data endData = eval(t->end, datatype);
        sprintf(q.op, "LT");
        sprintf(q.a, "%s", startData.t);
        sprintf(q.b, "%s", endData.t);
        sprintf(q.t, "t%d", tIndx);
        tIndx++;
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);
        // jump to end label if not true
        sprintf(q.op, "JNT");
        sprintf(q.a, "label%d", labelIndx + 1);
        sprintf(q.b, "null");
        sprintf(q.t, "null");
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);
        // do for statement
        v = eval(t->thenStmt, datatype);
        // do step statement
        Data stepData = eval(t->step, datatype);
        sprintf(q.op, "PLUS");
        sprintf(q.a, "%s", startData.t);
        sprintf(q.b, "%s", stepData.t);
        sprintf(q.t, "%s", startData.t);
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);
        // jump to condition check
        sprintf(q.op, "JMP");
        sprintf(q.a, "label%d", labelIndx);
        sprintf(q.b, "null");
        sprintf(q.t, "null");
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);
        // add end label
        sprintf(q.op, "label");
        sprintf(q.a, "label%d", labelIndx + 1);
        sprintf(q.b, "null");
        sprintf(q.t, "null");
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);

        labelIndx += 2;
        break;
    }
    case SWITCH:
    {

        // printf("node type %d\n", a->l->nodetype);
        // float data = get_math_value(v, *datatype);
        struct ASTNode *case_stmts = a->r;
        // printf("switch conditoin %d direct_access %d datatype %d\n", data, v.int_, *datatype);

        // first switch expression evalauation
        v = eval(a->l, datatype);
        // then for each case
        Quadraple q;
        int caseIndx = 0;
        while (case_stmts->l)
        {
            struct CaseNode *case_stmt = (struct CaseNode *)case_stmts->l;
            //  printf("is default %d\n", case_stmt->is_default);
            if (case_stmt->is_default)
            {
                // if default
                // generate label
                sprintf(q.op, "label");
                sprintf(q.a, "label%d", labelIndx + caseIndx);
                sprintf(q.b, "null");
                sprintf(q.t, "null");
                quadraples[currQuad] = q;
                currQuad++;
                // printQuadraple(q);
                // eval
                if (case_stmt->stmts)
                    v = eval(case_stmt->stmts, datatype);
                // exit
                break;
            }

            // if not default
            // 1 - generate case label
            sprintf(q.op, "label");
            sprintf(q.a, "label%d", labelIndx + caseIndx);
            sprintf(q.b, "null");
            sprintf(q.t, "null");
            quadraples[currQuad] = q;
            currQuad++;
            // printQuadraple(q);
            // eval case expression

            Data t = eval(case_stmt->expr, datatype);

            // check if same data
            sprintf(q.op, "EQ");
            sprintf(q.a, "%s", v.t);
            sprintf(q.b, "%s", t.t);
            sprintf(q.t, "null");
            quadraples[currQuad] = q;
            currQuad++;
            // printQuadraple(q);
            // if not samedata jump to next case label
            sprintf(q.op, "JNT");
            sprintf(q.a, "label%d", labelIndx + caseIndx + 1);
            sprintf(q.b, "null");
            sprintf(q.t, "null");
            quadraples[currQuad] = q;
            currQuad++;
            // printQuadraple(q);
            // evaluate case statement
            if (case_stmt->stmts)
                eval(case_stmt->stmts, datatype);
            // jump to end label
            sprintf(q.op, "JMP");
            sprintf(q.a, "endlabelswitch%d", currSwitch);
            sprintf(q.b, "null");
            sprintf(q.t, "null");
            quadraples[currQuad] = q;
            currQuad++;
            // printQuadraple(q);

            if (!case_stmts->r)
                break;
            case_stmts = case_stmts->r;
            caseIndx++;
        }
        // add end label swtich
        sprintf(q.op, "label");
        sprintf(q.a, "endlabelswitch%d", currSwitch);
        sprintf(q.b, "null");
        sprintf(q.t, "null");
        quadraples[currQuad] = q;
        currQuad++;
        // printQuadraple(q);
        labelIndx += caseIndx + 1;
        currSwitch++;
        break;
    }
    default:
        break;
    }
    return v;
}
