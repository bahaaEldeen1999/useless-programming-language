%{
#include <string.h>
#include <stdio.h>
#include<stdlib.h>
#include <stdint.h>
#include "../src/includes/parser.h"

%}

%union{
        struct ASTNode* ast;
        char *string_;
        int int_;
        double float_;
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


%type <ast> expr;
%type <ast> assign;
%type <ast> declare_var;
%type <ast> if_statement;
%type <float_> line ;

%start input

%%

input: /* empty */ 
        |  input line  ;

line: expr semicolumn_  {$$ = eval($1); printf("ans: %f \n",$$);} |

     declare_var semicolumn_ {$$ = eval($1); printf("declare: %f \n",$$);}| 
     assign semicolumn_ {$$ = eval($1); printf("assign: %f \n",$$);}| 
     /* if_statement {}| */
        error;

expr: expr plus_ expr {$$ =  newASTNode(PLUS,$1,$3);}|
        expr minus_ expr {$$ =  newASTNode(MINUS,$1,$3);} |
        expr multiply_ expr {$$ =  newASTNode(MUL,$1,$3);} |
        expr divide_ expr {$$ =  newASTNode(DIV,$1,$3);} |
        expr modulus_ expr {$$ =  newASTNode(MOD,$1,$3);} |
        open_bracket_ expr close_bracket_ {$$ = $2;} |
        
        expr gt_ expr {$$ = newASTNode(GT,$1,$3); }|
        expr gte_ expr {$$ = newASTNode(GTE,$1,$3); }|
        expr lt_ expr {$$ = newASTNode(LT,$1,$3); }|
        expr lte_ expr {$$ = newASTNode(LTE,$1,$3);}|     
        expr equals_ expr {$$ =newASTNode(EQ,$1,$3); }|
        expr not_equal_ expr {$$ = newASTNode(NOTEQ,$1,$3); }|
        expr logical_and_ expr {$$ = newASTNode(AND,$1,$3); }|
        logical_not_ expr {$$ = newASTNode(NOT,$2,NULL); }|
        expr logical_or_ expr {$$ =newASTNode(OR,$1,$3);}|
        
        var_name_ {$$ = newVariableDataNode(VARIABLE,$1,0,0,0,NULL);} |
        integer_ { $$ = newDataNode(CONSTANT,INT,$1,0.0f,NULL,NULL) ;} |
        float_ { $$ = newDataNode(CONSTANT,FLOAT,NULL,$1,NULL,NULL) ;} ;|
        bool_ { $$ =  newDataNode(CONSTANT,BOOL,$1,0.0f,NULL,NULL);} ;


declare_var: dt_int_ var_name_ assign_ expr {$$ = newVariableDataNode(DECLARE, $2,INT,0,1,$4);} 
| dt_float_ var_name_ assign_ expr {$$ = newVariableDataNode(DECLARE, $2,FLOAT,0,1,$4);} 
| dt_bool_ var_name_ assign_ expr {$$ = newVariableDataNode(DECLARE, $2,BOOL,0,1,$4);} 
| dt_char_ var_name_ assign_ char_ {$$ = newVariableDataNode(DECLARE, $2,CHAR,0,1,$4);} 
|  dt_string_ var_name_ assign_ string_ {$$ = newVariableDataNode(DECLARE, $2,STRING,0,1,$4);}; 


| const_ dt_int_ var_name_ assign_ expr {$$ = newVariableDataNode(DECLARE, $3,INT,1,1,$5);} 
|const_ dt_float_ var_name_ assign_ expr {$$ = newVariableDataNode(DECLARE, $3,FLOAT,1,1,$5);} 
|const_ dt_bool_ var_name_ assign_ expr {$$ = newVariableDataNode(DECLARE, $3,BOOL,1,1,$5);} 
| const_ dt_char_ var_name_ assign_ char_ {$$ = newVariableDataNode(DECLARE, $3,CHAR,1,1,$5);} 
| const_ dt_string_ var_name_ assign_ string_ {$$ = newVariableDataNode(DECLARE, $3,STRING,1,1,$5);}; 


|dt_int_ var_name_  {$$ = newVariableDataNode(DECLARE, $2,INT,0,0,NULL);} 
| dt_float_ var_name_  {$$ = newVariableDataNode(DECLARE, $2,FLOAT,0,0,NULL);} 
| dt_bool_ var_name_  {$$ =newVariableDataNode(DECLARE, $2,BOOL,0,0,NULL);} 
| dt_char_ var_name_{$$ =newVariableDataNode(DECLARE, $2,CHAR,0,0,NULL);} 
|  dt_string_ var_name_ {$$ = newVariableDataNode(DECLARE, $2,STRING,0,0,NULL);};  


assign:   var_name_ assign_ expr {$$ = newVariableDataNode(ASSIGN, $1,0,0,1,$3);} ;
/*



if_statement: if_ open_bracket_ expr close_bracket_ open_curly_braces_ input close_curly_braces_ else_ open_curly_braces_ input close_curly_braces_ {}

| if_ open_bracket_ expr close_bracket_ open_curly_braces_ input close_curly_braces_ {  };
*/

%%

main(int argc, char **argv)
{
  return yyparse();
}

yyerror()
{
        printf("Error detected in parsing\n");
}

