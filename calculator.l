%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// yacc header
#include "y.tab.h"

// myfunction header
#include "myFunction.h"

map<string, double> varStack;

extern "C" int yywrap( void ){ return 1 ; }
%}

/* 優先順位順に記述（上: 優先度高, 下: 優先度低） */
/* yylval: キャッチした値を保持(計算用？) */ 
%%
" "  {}
"\n" return CR ;

"("  return SBK ;
")"  return EBK ;

"++" return INC ;
"--" return DEC ;

"*"	 return MUL ;
"/"	 return DIV ;
"%"  return SUR ;

"+"	 return ADD ;
"-"	 return SUB ;

"<<" return LEFT_SHIFT ;
">>" return RIGHT_SHIFT ;

"<"  return LEFT_CLS ;
">"  return RIGHT_CLS ;
"<=" return LEFT_CLS_EQ ;
">=" return RIGHT_CLS_EQ ;

"==" return EQ_EQ ;
"!=" return NOT_EQ ;

"="  return EQ ;
"+=" return ADD_EQ ;
"-=" return SUB_EQ ;
"*=" return MUL_EQ ;
"/=" return DIV_EQ ;
"%=" return SUR_EQ ;

[0-9]+ {
  yylval.str = new struct number;
  yylval.str->type = T_INT;
  yylval.str->val = atof(yytext);
  return NUMBER;
}

[0-9]+\.[0-9]+ {
  yylval.str = new struct number;
  yylval.str->type = T_DOUBLE;
  yylval.str->val = atof(yytext);
  return NUMBER;
}

[_a-zA-Z]*[_a-zA-Z0-9]+ {
  yylval.str = new struct number;
  yylval.str->type = T_STR;
  // yylval.str->sval = yytext;
  // memcpy(yylval.str->sval, yytext);
  memcpy(yylval.str->sval, yytext, strlen(yytext)+1);
  return CHARACTER;
}

%%