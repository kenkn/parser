%{
#include "myFunction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <map>

using namespace std;

// map<char*, double> varStack;

// yaccが定義する内部関数のプロトタイプ宣言
extern int yyerror( const char* ) ;
extern int yyparse( void ) ;
extern int yylex( void ) ;
extern char* yytext ;
extern FILE* yyin ;
%}

%union {
  struct number* str;
}

%token <str> NUMBER
%token <str> CHARACTER
%token CR
%token SBK EBK
%token INC DEC
%token MUL DIV SUR
%token ADD SUB
%token LEFT_SHIFT RIGHT_SHIFT
%token LEFT_CLS RIGHT_CLS LEFT_CLS_EQ RIGHT_CLS_EQ
%token EQ_EQ NOT_EQ
%token EQ ADD_EQ SUB_EQ MUL_EQ DIV_EQ SUR_EQ

%type <str> expr term factor

%left LEFT_SHIFT RIGHT_SHIFT
%left LEFT_CLS RIGHT_CLS LEFT_CLS_EQ RIGHT_CLS_EQ EQ_EQ NOT_EQ
%right EQ ADD_EQ SUB_EQ MUL_EQ DIV_EQ SUR_EQ

%%
input : line
      | input line
      ;

line : expr CR { printf(">> %f\n", $1->val) ; }
     | CR      { exit(0) ; } /* 未入力Enterは終了 */
     ;

expr : term	                  { $$ = $1 ; }
     | expr LEFT_CLS expr     { $$ = Comparing($1, $3, LEFT_CLS) ; }
     | expr RIGHT_CLS expr    { $$ = Comparing($1, $3, RIGHT_CLS) ; }
     | expr LEFT_CLS_EQ expr  { $$ = Comparing($1, $3, LEFT_CLS_EQ) ; }
     | expr RIGHT_CLS_EQ expr { $$ = Comparing($1, $3, RIGHT_CLS_EQ) ; }
     | expr LEFT_SHIFT expr   { $$ = Shiftoperator($1, $3, LEFT_SHIFT) ; }
     | expr RIGHT_SHIFT expr  { $$ = Shiftoperator($1, $3, RIGHT_SHIFT) ; }
     | expr ADD term          { $$ = newVal($1, $3, ADD) ; }
     | expr SUB term          { $$ = newVal($1, $3, SUB) ; }
     ;

term : factor          { $$ = $1 ; }
     | term MUL factor { $$ = newVal($1, $3, MUL) ; }
     | term DIV factor { $$ = newVal($1, $3, DIV) ; }
     | term SUR factor { $$ = newVal($1, $3, SUR) ; }
     ;

factor : NUMBER                  { $$ = $1 ; }
       | CHARACTER               { $$ = Variant($1, NULL) ; }
       | CHARACTER ADD_EQ expr   { $$ = Substitution($1, $3, ADD_EQ) ; }
       | CHARACTER SUB_EQ expr   { $$ = Substitution($1, $3, SUB_EQ) ; }
       | CHARACTER MUL_EQ expr   { $$ = Substitution($1, $3, MUL_EQ) ; }
       | CHARACTER DIV_EQ expr   { $$ = Substitution($1, $3, DIV_EQ) ; }
       | CHARACTER SUR_EQ expr   { $$ = Substitution($1, $3, SUR_EQ) ; }
       | CHARACTER EQ expr       { $$ = Variant($1, $3) ; }
       | INC CHARACTER           { $$ = front_incdec($2, INC) ; }
       | DEC CHARACTER           { $$ = front_incdec($2, DEC) ; }
       | CHARACTER INC           { $$ = back_incdec($1, INC) ; }
       | CHARACTER DEC           { $$ = back_incdec($1, DEC) ; }
       | SBK expr EBK            { $$ = $2 ; }
       ;
%%

// 補助関数の定義
int yyerror( char const* str )
{
	fprintf( stderr , "parser error near %s\n" , yytext ) ;
	return 0 ;
}

int main( void )
{
	yyin = stdin ;
	if ( yyparse() ) {
		fprintf( stderr , "Error ! Error ! Error !\n" ) ;
		exit( 1 ) ;
	}
}