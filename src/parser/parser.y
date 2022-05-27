%{
#include <string.h>
#include <stdio.h>
#include<stdlib.h>
#include <stdint.h>
#include "../src/includes/parser.h"
int yylex();
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
%token toggle_debug_ 
%token print_
%token comma_


%left plus_ minus_
%left multiply_ divide_ modulus_
%left UMINUS_GRAMMAR




%left logical_not_ 
%left logical_and_
%left logical_or_

%left gt_ gte_ lt_ lte_ equals_ not_equal_

%left open_bracket_ close_bracket_


%type <ast> expr multiline ;
%type <ast> assign;
%type <ast> declare_var declare_var_with_assign; 
%type <ast> if_statement while_loop repeat_until for_loop  case_stmt switch_stmt case_stmts;
%type <ast> line ;

%start input

%%

input: /* empty */ 
        |  input line semicolumn_ {
            int datatype=-1;
            Data v = eval($2,&datatype);
            /*
            switch( datatype){
                case INT:
                case BOOL:
                {
                    printf("int %d \n",v.int_);
                    break;
                }
                case CHAR:
                {
                    printf("char %c \n",v.char_);
                    break;
                }
                case FLOAT:
                {
                    printf("float %f \n",v.float_);
                    break;
                }
                case STRING:
                {
                    printf("string %s \n",v.string_);
                    break;
                }

            }
            */
    
        };
        | input error semicolumn_ {
            yyerrok;
        };

multiline: /* empty */ {$$ = NULL;} |
        line semicolumn_ multiline {
            if ($3 == NULL)
	                $$ = $1;
                      else
			$$ = newASTNode(LIST, $1, $3);
        };
line: expr   {$$ =  newASTNode(LIST,$1,NULL);} |
    declare_var  {$$ =  newASTNode(LIST,$1,NULL);}| 
    assign  {$$ =  newASTNode(LIST,$1,NULL);}| 
    if_statement  {$$ =  newASTNode(LIST,$1,NULL);} | 
    while_loop  {$$ =  newASTNode(LIST,$1,NULL);} | 
    repeat_until  {$$ =  newASTNode(LIST,$1,NULL);} | 
    for_loop  {$$ =  newASTNode(LIST,$1,NULL);} | 
    switch_stmt  {$$ =  newASTNode(LIST,$1,NULL);}; 


expr: expr plus_ expr {$$ =  newASTNode(PLUS,$1,$3);}|
        expr minus_ expr {$$ =  newASTNode(MINUS,$1,$3);} |
        expr multiply_ expr {$$ =  newASTNode(MUL,$1,$3);} |
        expr divide_ expr {$$ =  newASTNode(DIV,$1,$3);} |
        expr modulus_ expr {$$ =  newASTNode(MOD,$1,$3);} |
        minus_  expr %prec UMINUS_GRAMMAR {$$ =  newASTNode(UMINUS,$2,NULL);} |
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
        integer_ { $$ = newDataNode(CONSTANT,INT,$1,0.0f,0,NULL) ;} |
        float_ { $$ = newDataNode(CONSTANT,FLOAT,0,$1,0,NULL) ;} ;|
        bool_ { $$ =  newDataNode(CONSTANT,BOOL,$1,0.0f,0,NULL);} |
        char_ { $$ =  newDataNode(CONSTANT,CHAR,0,0.0f,$1[1],NULL);} ;
        


declare_var_with_assign: dt_int_ var_name_ assign_ expr {$$ = newVariableDataNode(DECLARE, $2,INT,0,1,$4);} 
| dt_float_ var_name_ assign_ expr {$$ = newVariableDataNode(DECLARE, $2,FLOAT,0,1,$4);} 
| dt_bool_ var_name_ assign_ expr {$$ = newVariableDataNode(DECLARE, $2,BOOL,0,1,$4);} 
| dt_char_ var_name_ assign_ expr {$$ = newVariableDataNode(DECLARE, $2,CHAR,0,1,$4);} ;
/* |  dt_string_ var_name_ assign_ string_ {$$ = newVariableDataNode(DECLARE, $2,STRING,0,1,$4);};  */

declare_var: declare_var_with_assign 
| const_ dt_int_ var_name_ assign_ expr {$$ = newVariableDataNode(DECLARE, $3,INT,1,1,$5);} 
|const_ dt_float_ var_name_ assign_ expr {$$ = newVariableDataNode(DECLARE, $3,FLOAT,1,1,$5);} 
|const_ dt_bool_ var_name_ assign_ expr {$$ = newVariableDataNode(DECLARE, $3,BOOL,1,1,$5);} 
| const_ dt_char_ var_name_ assign_ expr {$$ = newVariableDataNode(DECLARE, $3,CHAR,1,1,$5);} 
/* | const_ dt_string_ var_name_ assign_ string_ {$$ = newVariableDataNode(DECLARE, $3,STRING,1,1,$5);};  */


|dt_int_ var_name_  {$$ = newVariableDataNode(DECLARE, $2,INT,0,0,NULL);} 
| dt_float_ var_name_  {$$ = newVariableDataNode(DECLARE, $2,FLOAT,0,0,NULL);} 
| dt_bool_ var_name_  {$$ =newVariableDataNode(DECLARE, $2,BOOL,0,0,NULL);} 
| dt_char_ var_name_{$$ =newVariableDataNode(DECLARE, $2,CHAR,0,0,NULL);} ;
/* |  dt_string_ var_name_ {$$ = newVariableDataNode(DECLARE, $2,STRING,0,0,NULL);};   */


assign:   var_name_ assign_ expr {$$ = newVariableDataNode(ASSIGN, $1,0,0,1,$3);} ;




if_statement: if_ open_bracket_ expr close_bracket_ open_curly_braces_ multiline close_curly_braces_ else_ open_curly_braces_ multiline close_curly_braces_ {
    $$ = newFlowControlNode(IF,$3,$6,$10,NULL,NULL,NULL);}
| if_ open_bracket_ expr close_bracket_ open_curly_braces_ multiline close_curly_braces_ { $$ = newFlowControlNode(IF,$3,$6,NULL,NULL,NULL,NULL);  };

while_loop: while_ open_bracket_ expr close_bracket_ open_curly_braces_ multiline close_curly_braces_ { $$ = newFlowControlNode(WHILE,$3,$6,NULL,NULL,NULL,NULL); }


repeat_until: repeat_ open_curly_braces_ multiline close_curly_braces_ until_ open_bracket_ expr close_bracket_ { $$ = newFlowControlNode(REPEAT,$7,$3,NULL,NULL,NULL,NULL); }

for_loop: for_ open_bracket_ declare_var_with_assign column_ expr column_ expr close_bracket_ open_curly_braces_ multiline close_curly_braces_ {
    $$ = newFlowControlNode(FOR,NULL,$10,NULL,$3,$5,$7); 
};

case_stmt: case_ open_bracket_ expr close_bracket_ open_curly_braces_ multiline close_curly_braces_ {
    $$ = newCaseNode(CASE,NULL,$3,$6,0);
}
| default_ open_curly_braces_ multiline close_curly_braces_ {
    $$ = newCaseNode(CASE,NULL,NULL,$3,1);
}

case_stmts: /* nothing */ {$$ = NULL;} |
            case_stmt semicolumn_ case_stmts {
                if($3 == NULL){
                    $$ =  newASTNode(LIST, $1, NULL);
                }else{
                    $$ = newASTNode(LIST, $1, $3);
                }
            };

switch_stmt: switch_ open_bracket_ expr close_bracket_ open_curly_braces_ case_stmts close_curly_braces_ {
    $$ = newASTNode(SWITCH, $3, $6);
};


%%

int main(int argc, char **argv)
{
  return yyparse();
}

void yyerror(char *s, ...)
{
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}
