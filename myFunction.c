#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <map>
#include "myFunction.h"
#include <vector>

bool isVarFound (struct number* data) {
  if (varStack.find(data->sval) != varStack.end()) {
    // 存在する
    return true;
  } else {
    // 存在しない
    return false;
  }
}

struct number* newVal (struct number* p1, struct number* p2, int op) {

  struct number* res = new struct number ;

  switch (op) {
    case ADD: {
      res->val = p1->val + p2->val;
      break;
    }
    case SUB: {
      res->val = p1->val - p2->val;
      break;
    }
    case MUL: {
      res->val = p1->val * p2->val;
      break;
    }
    case DIV: {
      res->val = p1->val / p2->val;
      break;
    }
    case SUR: {
      res->val = (int)p1->val % (int)p2->val;
      break;
    }
    default: break;
  }

  if ( (p1->type != T_DOUBLE) && (p2->type != T_DOUBLE) ) {
    res->val = (double)((int)res->val);
  }

  return res;
}

struct number* Variant(struct number* name, struct number* value) {

  struct number* res = new struct number;

  // 代入式か
  if (value != NULL)
  {
    varStack.insert(make_pair(name->sval, value->val));
  }

  // 変数は登録されているか
  if (isVarFound(name))
  { // 登録されている
    res->type = T_DOUBLE;
    res->val = varStack[name->sval];
  }
  else
  { // 登録されていない
    fprintf(stderr, "%s is undefined.\n", name->sval);
    exit(1);
  }

  return res;
}

struct number* back_incdec (struct number* variant, int op) {

  // インクリメント前の値保持
  double before_inc_val = 0.0;

  // 表示用構造体を定義
  struct number* res = new struct number;

  res->type = T_DOUBLE;

  if ( isVarFound(variant) )
  {
    before_inc_val = varStack[variant->sval];
    res->val = before_inc_val;

    double tmp = before_inc_val;

    // 変数スタックから削除
    varStack.erase(variant->sval);

    if (INC == op) {
      tmp = tmp + 1;
    } else if (DEC == op) {
      tmp = tmp - 1;
    }

    varStack.insert(make_pair(variant->sval, tmp));
  }
  else
  {
    fprintf(stderr, "%s is undefined\n", variant->sval);
    exit(1);
  }

  return res;
}

struct number* front_incdec (struct number* variant, int op) {

  // 表示用構造体を定義
  struct number* res = new struct number;
  res->type = T_DOUBLE;

  if ( isVarFound(variant) )
  {
    double tmp = varStack[variant->sval];

    // 変数スタックから削除
    varStack.erase(variant->sval);

    if (INC == op) {
      tmp = tmp + 1;
    } else if (DEC == op) {
      tmp = tmp - 1;
    }

    varStack.insert(make_pair(variant->sval, tmp));

    res->val = tmp;
  }
  else
  {
    fprintf(stderr, "%s is undefined\n", variant->sval);
    exit(1);
  }

  return res;
}

struct number* Substitution (struct number* variant, struct number* num, int op) {

  struct number* res = new struct number;
  res->type = T_DOUBLE;

  double before_val = 0;

  if (isVarFound(variant)) {
    before_val = varStack[variant->sval];
  } else {
    fprintf(stderr, "%s is undefined.\n", variant->sval);
    exit(1);
  }

  double effect_num = num->val;

  switch (op) {
    case ADD_EQ:
      res->val = before_val + effect_num;
      break;
    case SUB_EQ:
      res->val = before_val - effect_num;
      break;
    case MUL_EQ:
      res->val = before_val * effect_num;
      break;
    case DIV_EQ:
      res->val = before_val / effect_num;
      break;
    case SUR_EQ:
      res->val = (int)before_val % (int)effect_num;
      break;
    default: break;
  }

  varStack.erase(variant->sval);
  varStack.insert(make_pair(variant->sval, res->val));
  return res;
}

struct number* Shiftoperator (struct number* source, struct number* target, int op) {

  // ローカル関数参照用変数
  static int s_op;
  s_op = op;

  // 計算用ローカル関数を定義
  struct {
    double operator()(double p0, double p1) {
      double res = 0;
      switch (s_op) {
        case LEFT_SHIFT: {
          // printf("LEFT\n");
          res = (int)p0 << (int)p1;
          break;
        }
        case RIGHT_SHIFT: {
          // printf("RIGHT\n");
          res = (int)p0 >> (int)p1;
          break;
        }
        default: {
          // printf("DEFAULT\n");
          break;
        }
      }
      return res;
    };
  } Calc;

  // 結果格納構造体の定義
  struct number* res = new struct number;
  res->type = T_DOUBLE;

  // シフトされる構造体が文字列以外
  if (source->type != T_STR)
  {
    // シフトする分の構造体が文字列以外
    if (target->type != T_STR)
    {
      // シフト演算
      res->val = Calc(source->val, target->val);
    }
    else
    {
      if (isVarFound(target))
      {
        // シフトする分の変数取得
        double target_val = varStack[target->sval];
        // シフト演算
        res->val = Calc(source->val, target_val);
      }
      else
      {
        // 変数未定義ならエラーで終了
        fprintf(stderr, "%s is undefined.\n", target->sval);
        exit(1);
      }
    }
  }
  else
  {
    double source_val = 0.0;

    // シフトする変数が存在するか
    if (isVarFound(source))
    {
      // シフトされる値を格納
      source_val = varStack[source->sval];
    }
    else
    {
      // 変数未定義ならエラーで終了
      fprintf(stderr, "%s is undefined.\n", source->sval);
      exit(1);
    }

    // シフト量構造体が文字列以外
    if (target->type != T_STR)
    {
      // シフト演算
      res->val = Calc(source_val, target->val);
    }
    else
    {
      // シフトされる変数が存在するか
      if (isVarFound(target))
      {
        // 変数に格納してシフト演算
        double target_val = varStack[target->sval];
        res->val = Calc(source_val, target_val);
      }
      else
      {
        // 変数が存在しなかったらエラーで終了
        fprintf(stderr, "%s is undefined.\n", target->sval);
        exit(1);
      }
    }
  }

  return res;
}

struct number* Comparing (struct number* p1, struct number* p2, int op) {

  struct number* res = new struct number;
  res->type = T_BOOL;

  switch (op) {
    case LEFT_CLS:
      res->val = p1->val < p2->val;
      break;
    case RIGHT_CLS:
      res->val = p1->val > p2->val;
      break;
    case LEFT_CLS_EQ:
      res->val = p1->val <= p2->val;
      break;
    case RIGHT_CLS_EQ:
      res->val = p1->val >= p2->val;
      break;
    default: break;
  }

  return res;
}