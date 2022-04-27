%{
#include "../src/includes/definitions.h"
#define print(s1)  ( printf("\n %s %s \n",s1,yytext))
#define rule(s1,s2)  { print(s1); return s2; }
%}
DIGIT   [0-9]
ZERO    0
DIGIT_NO_ZERO   [1-9]

%%

{ZERO}|{DIGIT_NO_ZERO}{DIGIT}* rule("integer",_integer)

{ZERO}|{DIGIT_NO_ZERO}{DIGIT}*\.{DIGIT}* rule("float",_float)

\".*\" rule("string",_string)

'.' rule("char",_char)

"true"|"false" rule("bool",_bool)

"(" rule("open_bracket",_open_bracket)
")" rule("close_bracket",_close_bracket)

"{" rule("open_curly braces",_open_curly_braces)
"}" rule("close_curly_braces",_close_curly_braces)
";" rule("semicolumn",_semicolumn)

"int" rule("dt_int",_dt_int)
"float" rule("dt_float",_dt_float)
"char" rule("dt_char",_dt_char)
"bool" rule("dt_bool",_dt_bool)
"string" rule("_dt_string",_dt_string)

"const" rule("const",_const)
"=" rule("const",_const)

"-" rule("minsu",_minus)
"+" rule("plus",_plus)
"*" rule("multiply",_multiply)
"/" rule("divide",_divide)
"%" rule("moduluse",_modulus)

"!" rule("not",_logical_not)
"&&" rule("and",_logical_and)
"||" rule("minsu",_logical_or)

">" rule("gt",_gt)
"<" rule("_lt",_lt)
">=" rule("gte",_gte)
"<=" rule("lte",_lte)
"!=" rule("not equal",_not_equal)
"==" rule("equal",_equals)

"if" rule("if",_if)
"else" rule("else",_else)
"while" rule("while",_while)
"repeat" rule("repeat",_repeat)
"until" rule("until",_until)
"for" rule("for",_for)
":" rule("column",_column)
"break" rule("break",_break)
"continue" rule("continue",_continue)
"switch" rule("switch",_switch)
"case" rule("case",_case)
"default" rule("default",_default)

%%