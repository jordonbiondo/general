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
#include <math.h>

#include "object.h"

object make_int(int x) {
  object o;
  o.tag = int_ot;
  o.value.int_v = x;
  return o;
}

object make_double(double x) {
  object o;
  o.tag = double_ot;
  o.value.double_v = x;
  return o;
}

object make_byte(byte x) {
  object o;
  o.tag = byte_ot;
  o.value.byte_v = x;
  return o;
}

object make_string(string x) {
  object o;
  o.tag = string_ot;
  o.value.string_v = x;
  return o;
}


object* cons(object a, object* b) {
  cell* c = malloc(sizeof(cell));
  c->car = *ocopy(&a);
  c->cdr = b;
  object* o = oalloc();
  o->tag = cell_ot;
  o->value.cell_v = c;
  return o;
}

object* oalloc() {
  static bool did = false;
  if (!did) {
    did = true;
    printf("setting up stuff\n");
    allocated_objects = malloc(sizeof(object*) * 50);
    allocated_objects_length = 50;
  }

  object* x = malloc(sizeof(object));

  if (objects_allocated >= allocated_objects_length) {
    long int new_size = round(allocated_objects_length * 1.5);
    object** new_list = malloc(sizeof(object*) * new_size);
    memcpy(new_list, allocated_objects, allocated_objects_length);
    allocated_objects_length = new_size;
    object** old = allocated_objects;
    free(old);
    allocated_objects = new_list;
  }

  allocated_objects[objects_allocated] = x;
  objects_allocated ++;

  x->tag = int_ot;
  x->value.int_v = 0;
  return x;
}

int ofree(object* o) {
  if (!o) {
    return 0;
  } else {
    int c = 0;
    object* next = NULL;
    do {
      next = NULL;
      if (is(*o, cell)) {
        next = cdr(o);
      }
      if ((is(*o, string) || is(*o, error)) && stringv(o)) {
        free(stringv(o));
      }
      if (o != NIL && o != T) {
        free(o);
        c += 1;
      }
      o = next;
    } while (o);

    return c;
  }
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

object* opush(object elm, object* list) {
  cdr(list) = cons(car(list), cdr(list));
  car(list) = elm;
  pl(list);
  return list;
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
  return booly(strcmp(stringv(a), stringv(b)) == 0);
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
    case string_ot:
      return ostring_equal(a, b);
    case byte_ot:
      return booly(bytev(a) == bytev(b));
    case error_ot:
      return ostring_equal(a, b);
      return NIL;
    case nil_ot:
      return T;
    case t_ot:
      return T;
    case cell_ot: {
      if (is(*oequal(&car(a), &car(b)), t)) {
        return oequal(cdr(a), cdr(b));
      } else {
        return NIL;
      }
    }
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
    case int_ot:
      printf("%d", o.value.int_v);
      break;
    case double_ot:
      printf("%f", o.value.double_v);
      break;
    case string_ot:
      printf("%s", o.value.string_v);
      break;
    case byte_ot:
      printf("%#1x", o.value.byte_v);
      break;
    case error_ot:
      printf("%s", o.value.string_v);
      break;
    case nil_ot:
      printf("nil");
      break;
    case t_ot:
      printf("nil");
      break;
    case cell_ot:
      printf("car: \n");
      ppo(cellv((&o))->car);
      break;
    default:
      printf("???");
    }
  putchar('\n');
}

void pl_internal(object* o, bool inside) {
  putchar('(');
  ofor_each(elm, head, o) {
    switch(elm->tag)
      {
      case int_ot:
        printf("%d", elm->value.int_v);
        break;
      case double_ot:
        printf("%f", elm->value.double_v);
        break;
      case string_ot:
        printf("%s", elm->value.string_v);
        break;
      case byte_ot:
        printf("%#1x", elm->value.byte_v);
        break;
      case error_ot:
        printf("%s", elm->value.string_v);
        break;
      case nil_ot:
        printf("nil");
        break;
      case t_ot:
        printf("t");
        break;
      case cell_ot:
        pl_internal(elm, true);
        break;
      default:
        printf("???");
      }
    if (!is(*cdr(head), nil)) {
      printf(", ");
    }
  }
  putchar(')');
  if (!inside) {
    putchar('\n');
  }
}

void pl(object* o) {
  pl_internal(o, false);
}

/* general.c ends here */
