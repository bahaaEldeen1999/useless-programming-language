%{
#include <string.h>
#include <stdio.h>
#include<stdlib.h>
%}

%union{
        char *str;
        int val;
}

%token <val> integer_
%token float_
%token string_
%token char_
%token bool_
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

%type <val> expr

%start input

%%

input: /* empty */ 
        | input line;

line: expr '\n' {} |
        error;

expr: expr plus_ expr {$$ = ($1)+($3);printf("ans: %d\n",$$);}
        | integer_ { $$ = ($1) ;} ;

%%

main(int argc, char **argv)
{
  yyparse();
}

yyerror()
{
        printf("Error detected in parsing\n");
}