%{
#include <string.h>
#include <stdio.h>
#include<stdlib.h>
#define MAX_NAME_LENGTH 20
#define MAX_VARS 1000
#define MAX_STRING 1000
// 1 => int 
// 2 => float
// 3 => char/
// 4 => bool
// 5 => string

enum datatypes{INT, FLOAT, CHAR,BOOL,STRING};

typedef union {
        int int_;
        float float_;
        char char_;
        char string_[MAX_STRING];
        int bool_;
    } Data;


struct node {
    char name[MAX_NAME_LENGTH];
    int type;
    int id;
    int is_const;
    Data data;
    
};



static struct node* symbol_table[MAX_VARS];
%}

%union{
        char *string_;
        int int_;
        float float_;
        int bool_;
}

%token <int_> integer_
%token <float_> float_
%token <string_> string_
%token <string_> char_
%token <bool_> bool_
%token <string_> var_name_
%token new_line_
%token open_bracket_
%token close_bracket_
%token open_curly_braces_
%token close_curly_braces_
%token semicolumn_
%token dt_int_
%token dt_float_
%token dt_char_
%token dt_bool_
%token dt_string_
%token const_
%token assign_
%token minus_
%token plus_
%token divide_
%token multiply_
%token modulus_
%token logical_not_
%token logical_and_
%token logical_or_
%token gt_
%token lt_
%token gte_
%token lte_
%token not_equal_
%token equals_
%token if_
%token else_
%token while_
%token repeat_
%token until_
%token for_
%token column_
%token break_
%token continue_
%token switch_
%token case_
%token default_



%left plus_ minus_
%left multiply_ divide_ modulus_



%left logical_not_ 
%left logical_and_
%left logical_or_

%left gt_ gte_ lt_ lte_ equals_ not_equal_

%left open_bracket_ close_bracket_


%type <int_> expr_int;
%type <float_> expr_float;
%type <bool_> expr_bool;
%type <int_> assign;

%start input

%%

input: /* empty */ 
        |  input line  ;

line: expr_int new_line_  {} |
     expr_bool new_line_  {} |
     assign new_line_ {}|
        error;

expr_int: expr_int plus_ expr_int {$$ = ($1)+($3);printf("ans: %d\n",$$); }|
        expr_int minus_ expr_int {$$ = ($1)-($3);printf("ans: %d\n",$$);} |
        expr_int multiply_ expr_int {$$ = ($1)*($3);printf("ans: %d\n",$$);} |
        expr_int divide_ expr_int {$$ = ($1)/($3);printf("ans: %d\n",$$);} |
        expr_int modulus_ expr_int {$$ = ($1)%($3);printf("ans: %d\n",$$);} |
        open_bracket_ expr_int close_bracket_ {$$ = ($2);printf("ans: %d\n",$$);} |
             integer_ { $$ = ($1) ;} ;



expr_bool : expr_bool gt_ expr_bool {$$ = $1 > $3;printf("ans: %d\n",$$); }|
 expr_bool gte_ expr_bool {$$ = $1 >= $3;printf("ans: %d\n",$$); }|
  expr_bool lt_ expr_bool {$$ = $1 < $3;printf("ans: %d\n",$$); }|
   expr_bool lte_ expr_bool {$$ = $1 <= $3;printf("ans: %d\n",$$); }|     
    expr_bool equals_ expr_bool {$$ = $1 == $3;printf("ans: %d\n",$$); }|
     expr_bool not_equal_ expr_bool {$$ = $1 != $3;printf("ans: %d\n",$$); }|
      expr_bool logical_and_ expr_bool {$$ = $1 && $3;printf("ans: %d\n",$$); }|
    logical_not_ expr_bool {$$ = !$2  ;printf("ans: %d\n",$$); }|
        expr_bool logical_or_ expr_bool {$$ = $1 || $3;printf("ans or: %d\n",$$); }|
             expr_int  |
             bool_ { $$ = ($1) ;} ;


assign: dt_int_ var_name_ assign_ expr_int {$$ = assign_value($2,INT,$4);printf("assign: %d\n",$$);} 
| dt_float_ var_name_ assign_ expr_int {$$ = assign_value($2,FLOAT,$4);printf("assign: %d\n",$$);} 
| dt_bool_ var_name_ assign_ expr_bool {$$ = assign_value($2,BOOL,$4);printf("assign: %d\n",$$);} 
| dt_char_ var_name_ assign_ char_ {$$ = assign_value($2,CHAR,$4[1]);printf("assign: %d\n",$$);} 
|  dt_string_ var_name_ assign_ string_ {$$ = assign_value($2,STRING,$4);printf("assign: %d\n",$$);}; 

%%

main(int argc, char **argv)
{
  yyparse();
}

yyerror()
{
        printf("Error detected in parsing\n");
}

int assign_value(char* var_name,int datatype,void* data){
    // check empty position 
    for(int i=0;i<MAX_VARS;i++){
        if(!symbol_table[i]){
            //printf("1\n");
            symbol_table[i] = malloc(sizeof(*symbol_table[i]));
            //printf("2\n");
            strcpy(symbol_table[i]->name , var_name);
            //printf("3\n");
            symbol_table[i]->id = i;
            //printf("4\n");
            symbol_table[i]->type = datatype;
            //printf("5\n");

            if(datatype == INT){
                //printf("6\n");
               // printf("data %d \n",(int*)data);
                symbol_table[i]->data.int_ = ((int*)data);
               // printf("7\n");
                printf("name %s id %d type %d value %d \n",symbol_table[i]->name,symbol_table[i]->id ,symbol_table[i]->type ,symbol_table[i]->data.int_ );
            }else if(datatype == FLOAT){
               // printf("data %f \n",data);
                symbol_table[i]->data.float_ = *((double*)&data);
                 printf("name %s id %d type %d value %f \n",symbol_table[i]->name,symbol_table[i]->id ,symbol_table[i]->type ,symbol_table[i]->data.float_ );
            }else if(datatype == BOOL){
                symbol_table[i]->data.bool_ = ((int*)data);
                 printf("name %s id %d type %d value %d \n",symbol_table[i]->name,symbol_table[i]->id ,symbol_table[i]->type ,symbol_table[i]->data.bool_ );
            }else if(datatype == CHAR){
                symbol_table[i]->data.char_ = *((char*)&data);
                 printf("name %s id %d type %d value %c \n",symbol_table[i]->name,symbol_table[i]->id ,symbol_table[i]->type ,symbol_table[i]->data.char_ );
            }else if(datatype == STRING){
                //symbol_table[i]->data.string_ = ((char**)data);
                strcpy(symbol_table[i]->data.string_ ,  ((char*)data));
                 printf("name %s id %d type %d value %s \n",symbol_table[i]->name,symbol_table[i]->id ,symbol_table[i]->type ,symbol_table[i]->data.string_ );
            }

           
            return 0;

        }
    }
    return -1;
}