%{
//#include "../src/includes/definitions.h"
#include "parse.tab.h"
#include <string.h>
#include <stdio.h>
#include<stdlib.h>
#define print(s1)  ( printf("\n %s %s \n",s1,yytext))
#define rule(s1,s2)  { print(s1); return s2; }
%}

DIGIT   [0-9]
ZERO    0
DIGIT_NO_ZERO   [1-9]

%%

{ZERO}|{DIGIT_NO_ZERO}{DIGIT}* {yylval.val = atoi(yytext);return integer_;}

{ZERO}|{DIGIT_NO_ZERO}{DIGIT}*\.{DIGIT}* rule("float",float_)

\".*\" rule("string",string_)

'.' rule("char",char_)

"true"|"false" rule("bool",bool_)

"(" rule("open_bracket",open_bracket_)
")" rule("close_bracket",close_bracket_)

"{" rule("open_curly braces",open_curly_braces_)
"}" rule("close_curly_braces",close_curly_braces_)
";" rule("semicolumn",semicolumn_)

"int" rule("dt_int",dt_int_)
"float" rule("dt_float",dt_float_)
"char" rule("dt_char",dt_char_)
"bool" rule("dt_bool",dt_bool_)
"string" rule("_dt_string",dt_string_)

"const" rule("const",const_)
"=" rule("const",const_)

"-" rule("minsu",minus_)
"+" rule("plus",plus_)
"*" rule("multiply",multiply_)
"/" rule("divide",divide_)
"%" rule("moduluse",modulus_)

"!" rule("not",logical_not_)
"&&" rule("and",logical_and_)
"||" rule("minsu",logical_or_)

">" rule("gt",gt_)
"<" rule("_lt",lt_)
">=" rule("gte",gte_)
"<=" rule("lte",lte_)
"!=" rule("not equal",not_equal_)
"==" rule("equal",equals_)

"if" rule("if",if_)
"else" rule("else",else_)
"while" rule("while",while_)
"repeat" rule("repeat",repeat_)
"until" rule("until",until_)
"for" rule("for",for_)
":" rule("column",column_)
"break" rule("break",break_)
"continue" rule("continue",continue_)
"switch" rule("switch",switch_)
"case" rule("case",case_)
"default" rule("default",default_)

%%

