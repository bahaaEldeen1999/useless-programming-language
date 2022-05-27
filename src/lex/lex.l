%{
//#include "../src/includes/definitions.h"
#include "parse.tab.h"
#include <string.h>
#include <stdio.h>
#include<stdlib.h>
#define print(s1)  ( printf("\n %s %s \n",s1,yytext),0)
/**
1 => int 
2 => float
3 => char/string
4 => bool
*/
#define rule(s1,s2,set_value,can_print)  { if(set_value==1){yylval.int_=atoi(yytext); }else if(set_value==2){ yylval.float_=atof(yytext); }else if(set_value == 3){ yylval.string_ = strndup(yytext,strlen(yytext)); }else if(set_value==4){ if(yytext == "true") yylval.bool_ = 1;else yylval.bool_ = 0; } ;if(can_print)print(s1); return s2; }

// declare print all quadraples 
void printAllQuadraples();
void printSymbolTable();
%}

DIGIT   [0-9]
ZERO    0
DIGIT_NO_ZERO   [1-9]
CHAR    [a-zA-z]

%%
[ \n\t]         ;                   

\/\/.*  ;
({ZERO}|{DIGIT_NO_ZERO}{DIGIT}*)  rule("integer",integer_,1,0)

({ZERO}|{DIGIT_NO_ZERO}{DIGIT}*\.{DIGIT}*) rule("float",float_,2,0)

\".*\" rule("string",string_,3,0)

'.' rule("char",char_,3,0)

"true"|"false" rule("bool",bool_,4,0)

"(" rule("open_bracket",open_bracket_,0,0)
")" rule("close_bracket",close_bracket_,0,0)

"{" rule("open_curly braces",open_curly_braces_,0,0)
"}" rule("close_curly_braces",close_curly_braces_,0,0)
";" rule("semicolumn",semicolumn_,0,0)

"int" rule("dt_int",dt_int_,0,0)
"float" rule("dt_float",dt_float_,0,0)
"char" rule("dt_char",dt_char_,0,0)
"bool" rule("dt_bool",dt_bool_,0,0)
"string" rule("_dt_string",dt_string_,0,0)

"const" rule("const",const_,0,0)
"=" rule("equals",assign_,0,0)

"-" rule("minsu",minus_,0,0)
"+" rule("plus",plus_,0,0)
"*" rule("multiply",multiply_,0,0)
"/" rule("divide",divide_,0,0)
"%" rule("moduluse",modulus_,0,0)

"!" rule("not",logical_not_,0,0)
"&&" rule("and",logical_and_,0,0)
"||" rule("minsu",logical_or_,0,0)

">" rule("gt",gt_,0,0)
"<" rule("_lt",lt_,0,0)
">=" rule("gte",gte_,0,0)
"<=" rule("lte",lte_,0,0)
"!=" rule("not equal",not_equal_,0,0)
"==" rule("equal",equals_,0,0)

"if" rule("if",if_,0,0)
"else" rule("else",else_,0,0)
"while" rule("while",while_,0,0)
"repeat" rule("repeat",repeat_,0,0)
"until" rule("until",until_,0,0)
"for" rule("for",for_,0,0)
":" rule("column",column_,0,0)
"break" rule("break",break_,0,0)
"continue" rule("continue",continue_,0,0)
"switch" rule("switch",switch_,0,0)
"case" rule("case",case_,0,0)
"default" rule("default",default_,0,0)
"toggle_debug" rule("set debug",toggle_debug_,0,0)
"print" rule("print",print_,0,0)
"," rule("comma",comma_,0,0)

{CHAR}({DIGIT}|{CHAR})* rule("var name",var_name_,3,0)

 <<EOF>> {printAllQuadraples();printSymbolTable();yyterminate();return EOF_;}
%%