%{
#include <string.h>
#include <stdio.h>
#include<stdlib.h>
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
%left open_bracket_ close_bracket_

%left logical_not_ 
%left logical_and_
%left logical_or_

%type <int_> expr_int
%type <bool_> expr_bool;

%start input

%%

input: /* empty */ 
        |  input line  ;

line: expr_int new_line_  {} |
     expr_bool new_line_  {} |
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
             expr_int  |
             bool_ { $$ = ($1) ;} ;

%%

main(int argc, char **argv)
{
  yyparse();
}

yyerror()
{
        printf("Error detected in parsing\n");
}