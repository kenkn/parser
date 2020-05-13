#ifndef MYFUNCTION_H
#define MYFUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>

// トークン識別番号利用のため include
#include "y.tab.h"

using namespace std;

// 変数用スタック
extern map<string, double> varStack;

// 変数の種別一覧
enum numberType {
  T_NULL = 0,
  T_INT,
  T_DOUBLE,
  T_STR,
  T_BOOL
} ;

// 入力用構造体
struct number {
  enum numberType type;
  union {
    char sval[100];
    double val;
  } ;
} ;

bool isVarFound (struct number* data) ;
struct number* newVal (struct number* p1, struct number* p2, int op) ;
struct number* Variant (struct number* name, struct number* value) ;
struct number* back_incdec (struct number* variant, int op) ;
struct number* front_incdec (struct number* variant, int op) ;
struct number* Substitution (struct number* variant, struct number* num, int op) ;
struct number* Shiftoperator (struct number* source, struct number* target, int op) ;
struct number* Comparing (struct number* p1, struct number* p2, int op) ;

#endif /* end of include guard: MYFUNCTION_H */