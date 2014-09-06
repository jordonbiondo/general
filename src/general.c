/* The MIT License (MIT)
 *
 * Copyright (c) 2014 Jordon Biondo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

enum general_tag {
  int_t = 0,
  double_t,
  string_t,
  byte_t,
  nil_t,
  t_t,
  cell_t,
  error_t
};

const char* tag_string(enum general_tag t) {
  switch(t)
    {
    case int_t:
      return "int";
    case double_t:
      return "double";
    case string_t:
      return "string";
    case byte_t:
      return "byte";
    case error_t:
      return "error";
    case nil_t:
      return "nil";
    case t_t:
      return "t";
    case cell_t:
      return "cell";
    }
  return "unknown";
}

typedef char* string;
typedef char byte;

struct general_cell;
typedef struct general_cell cell;

union general_values {
  int int_v;
  double double_v;
  string string_v; /* also the error */
  byte byte_v;
  cell* cell_v;
};

struct general_object {
  enum general_tag tag;
  union general_values value;
};

#define is(o, type) (o.tag == type ## _t)
#define is_number(o) ({                                         \
      object __internal_o = o;                                  \
      (is(__internal_o, int) || is(__internal_o, double));      \
    })
typedef struct general_object object;

struct general_cell {
  object car;
  object* cdr;
};

void ppo(object);

/**
 * Create a Cons cell
 */
object* cons(object a, object* b) {
  cell* c = malloc(sizeof(cell));
  c->car = a;
  c->cdr = b;
  object* o = malloc(sizeof(object));
  o->tag = cell_t;
  o->value.cell_v = c;
  return o;
}

object nil = {
  .tag = nil_t
};

object* NIL = &nil;

#define cellv(o) ((o)->value.cell_v)
#define intv(o) ((o)->value.int_v)
#define doublev(o) ((o)->value.double_v)
#define stringv(o) ((o)->value.string_v)
#define errorv(o) ((o)->value.error_v)
#define bytev(o) ((o)->value.byte_v)

/* #define for_cell(name, object)                                   \ */

object make_int(int x) {
  object o;
  o.tag = int_t;
  o.value.int_v = x;
  return o;
}

object make_double(double x) {
  object o;
  o.tag = double_t;
  o.value.double_v = x;
  return o;
}

object make_byte(byte x) {
  object o;
  o.tag = byte_t;
  o.value.byte_v = x;
  return o;
}

object make_string(string x) {
  object o;
  o.tag = string_t;
  o.value.string_v = x;
  return o;
}

object oadd(object* args) {
  int iout = 0;
  int dout = 0;
  bool is_int = true;
  for (object* o = args; ! is((*o), nil); o = cellv(o)->cdr) {
    cell* c = cellv(o);
    if (is(c->car, int)) {
      iout += c->car.value.int_v;
      dout += c->car.value.int_v;
    } else if (is(c->car, double)) {
      is_int = false;
      dout += c->car.value.double_v;
    } else {
      ppo((*o));
      //error
    }
  }
  return is_int ? make_int(iout) : make_double (dout);
}

/**
 * Subtract args, if only one, negate.
 */
object ominus(object* args) {
  int iout = 0;
  int dout = 0;
  bool is_int = true;
  cell list = *args->value.cell_v;
  if (is(list.car, int)) {
    iout = list.car.value.int_v;
    dout = list.car.value.int_v;
  } else if (is(list.car, double)) {
    is_int = false;
    dout = list.car.value.double_v;
  }

  if ((!list.cdr) || is((list.cdr->value.cell_v->car), nil)) {
    iout = -iout;
    dout = -dout;
  } else {
    for (object* o = args; ! is((*o), nil); o = cellv(o)->cdr) {
      cell* c = cellv(o);
      if (is(c->car, int)) {
        iout -= c->car.value.int_v;
        dout -= c->car.value.int_v;
      } else if (is(c->car, double)) {
        is_int = false;
        dout -= c->car.value.double_v;
      } else {
        //error
      }
    }
  }
  return is_int ? make_int(iout) : make_double (dout);
}


/**
 * Pretty Print Object.
 */
void ppo(object o) {
  printf("object <%s>\n  value: ", tag_string(o.tag));
  switch(o.tag)
    {
    case int_t:
      printf("%d", o.value.int_v);
      break;
    case double_t:
      printf("%f", o.value.double_v);
      break;
    case string_t:
      printf("%s", o.value.string_v);
      break;
    case byte_t:
      printf("%#1x", o.value.byte_v);
      break;
    case error_t:
      printf("%s", o.value.string_v);
      break;
    case nil_t:
      printf("nil");
      break;
    case t_t:
      printf("nil");
      break;
    case cell_t:
      printf("car: \n");
      ppo(cellv((&o))->car);
      break;
    default:
      printf("???");
    }
  putchar('\n');
}

/* general.c ends here */
