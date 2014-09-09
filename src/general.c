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
#include <string.h>

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

#define is(o, type) ((o).tag == type ## _t)

#define ofalsy(o) (is((o), nil))
#define otruthy(o) (! (ofalsy(o)))

typedef struct general_object object;

static inline bool is_number(object* o) {
  return (is(*o, int) || is(*o, double));
}

struct general_cell {
  object car;
  object* cdr;
};

void ppo(object);
void pl(object*);
void pl_internal(object*, bool);
object* ocopy(object*);

/**
 * Create a Cons cell
 */
object* cons(object a, object* b) {
  cell* c = malloc(sizeof(cell));
  c->car = *ocopy(&a);
  c->cdr = b;
  object* o = malloc(sizeof(object));
  o->tag = cell_t;
  o->value.cell_v = c;
  return o;
}

#define list1(a) (cons(a, NIL))
#define list2(b, a) (cons(b, list1(a)))
#define list3(c, b, a) (cons(c, list2(b, a)))
#define list4(d, c, b, a) (cons(d, list3(c, b, a)))

#define car(o) (cellv(o)->car)
#define cdr(o) (cellv(o)->cdr)

const object nil_global = {
  .tag = nil_t
};

#define NIL ((object*)&nil_global)

const object t_global = {
  .tag = t_t
};

#define T ((object*)&t_global)

#define cellv(o) ((o)->value.cell_v)
#define intv(o) ((o)->value.int_v)
#define doublev(o) ((o)->value.double_v)
#define stringv(o) ((o)->value.string_v)
#define errorv(o) ((o)->value.error_v)
#define bytev(o) ((o)->value.byte_v)

#define numberv(o)                              \
  ({                                            \
    object __x = o;                             \
    double __value = 0;                         \
    if (is(__x, int))                           \
      __value = intv(&__x);                     \
    else                                        \
      __value = doublev(&__x);                  \
    __value;                                    \
  })

#define booly(x) ((x) ? T : NIL)

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

object* oalloc() {
  object* x = malloc(sizeof(object));
  return x;
}

object oadd(object* args) {
  int iout = 0;
  double dout = 0;
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
  double dout = 0;
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

object olength(object* list) {
  object* o = list;
  int length = 0;
  if (is(*o, nil)) {
    return make_int(length);
  }

  while(is(*o, cell)) {
    length++;
    o = cdr(o);
  }

  if (is(*o, nil)) {
    return make_int(length);
  } else {
    //error (1 2 . 3) scenario
    return make_int(-1);
  }
}

#define ofor_each(name, head, thing)                                    \
  for(object* head = (thing); head != NULL && is(*head, cell); head = cdr(head)) \
    for(object* name = &car(head); name != NULL; name = NULL)


object* olast(object* list) {
  object* last = list;
  ofor_each(elm, head, list) {
    last = head;
  }
  return last;
}

object* oappend(object* args) {
  object* copied = ocopy(args);
  ofor_each(list, head, copied) {
    if (!is(*cdr(head), nil)) {
      object* last = olast(list);
      cdr(last) = &car(cdr(head));
    }
  }
  return &car(copied);
}

object* opop(object* list) {
  object* value = ocopy(&car(list));

  if (olength(list).value.int_v > 1) {
    car(list) = car(cdr(list));
    cdr(list) = cdr(cdr(list));
  } else {
    *list = *NIL;
  }
  return value;
}

object* onumber_equal(object* a, object* b) {
  if (is(*a, int) && is(*b, int)) {
    return booly(intv(a) == intv(b));
  } else {
    double ad = numberv(*a);
    double bd = numberv(*b);
    return booly(ad == bd);
  }
  return NIL;
}

object* ostring_equal(object* a, object* b) {
  //todo
  return booly(strcmp(stringv(a), stringv(b)) == 0);
}

object* olist_equal(object* a, object* b) {
  a = a + 0; //remove me
  b = b + 0; //remove me
  while(is(*a, cell)) {
    //todo
  }
  for (object* o = a; ! is((*o), cell); o = cellv(o)->cdr) {

  }
  return NIL;
}

object* ocopy(object* o) {
  if (is(*o, t)) {
    return T;
  } else if (is(*o, nil)) {
    return NIL;
  }

  object* copy = oalloc();
  *copy = *o;
  if (is(*copy, string)) {
    string newString = malloc(sizeof(stringv(o)));
    stringv(copy) = newString;
    strcpy(stringv(copy), stringv(o));
  } else if (is(*copy, cell)) {
    cell* newCell = malloc(sizeof(cell));
    cellv(copy) = newCell;
    newCell->car = cellv(o)->car;
    newCell->cdr = ocopy(cellv(o)->cdr);
  }
  return copy;
}

object* oequal(object* a, object* b) {
  if (is_number(a) && is_number(b)) {

    return onumber_equal(a, b);

  } else {

    if (a->tag != b->tag) {
      return NIL;
    }

    switch (a->tag) {
    case string_t:
      return ostring_equal(a, b);
    case byte_t:
      return booly(bytev(a) == bytev(b));
    case error_t:
      // todo
      return NIL;
    case nil_t:
      return T;
    case t_t:
      return T;
    case cell_t:
      return olist_equal(a, b);
    default:
      return NIL;
    }
  }
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

void pl_internal(object* o, bool inside) {
  printf("(");
  ofor_each(elm, head, o) {
    switch(elm->tag)
      {
      case int_t:
        printf("%d", elm->value.int_v);
        break;
      case double_t:
        printf("%f", elm->value.double_v);
        break;
      case string_t:
        printf("%s", elm->value.string_v);
        break;
      case byte_t:
        printf("%#1x", elm->value.byte_v);
        break;
      case error_t:
        printf("%s", elm->value.string_v);
        break;
      case nil_t:
        printf("nil");
        break;
      case t_t:
        printf("t");
        break;
      case cell_t:
        pl_internal(elm, true);
        break;
      default:
        printf("???");
      }
    if (!is(*cdr(head), nil)) {
      printf(", ");
    }
  }
  if (!inside) {
    putchar('\n');
  }
}

void pl(object* o) {
  pl_internal(o, false);
}

/* general.c ends here */
