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

#include "../src/general.c"
#include "greatest/greatest.h"

GREATEST_MAIN_DEFS();

TEST adding_integers_type () {
  object a = make_int(3);
  object b = make_int(4);
  object* args = cons(a, cons(b, NIL));
  object result = oadd(args);
  ASSERTm("Adding ints, returns int.", is(result, int));
  PASS();
}

TEST adding_integers_value () {
  object a = make_int(3);
  object b = make_int(4);
  object* args;
  object result;

  args = cons(a, cons(b, NIL));
  result = oadd(args);
  ASSERT_EQm("adding two ints", intv(&result), 7);

  args = cons(a, NIL);
  result = oadd(args);
  ASSERT_EQm("adding one int", intv(&result), 3);

  args = NIL;
  result = oadd(args);
  ASSERT_EQm("adding no ints", intv(&result), 0);
  PASS();
}

TEST adding_doubles_type () {
  object a = make_double(3.1);
  object b = make_double(4.5);
  object* args;
  object result;

  args = cons(a, cons(b, NIL));
  result = oadd(args);
  ASSERTm("Adding doubles, returns double.", is(result, double));

  args = NIL;
  result = oadd(args);
  ASSERTm("Adding no doubles returns int.", is(result, int));
  PASS();
}

TEST adding_doubles_value () {
  object a = make_double(3.1);
  object b = make_double(4.5);
  object* args;
  object result;

  args = cons(a, cons(b, NIL));
  result = oadd(args);
  ASSERT_EQm("adding two doubles", doublev(&result), 7.6);

  args = cons(a, NIL);
  result = oadd(args);
  ASSERT_EQm("adding one double", doublev(&result), 3.1);

  args = NIL;
  result = oadd(args);
  ASSERT_EQm("adding no doubles", intv(&result), 0);
  PASS();
}

TEST list_length () {
  object* l0 = NIL;
  object l0length = olength(l0);
  ASSERT_EQ(intv(&l0length), 0);

  object* l1 = list1(*T);
  object l1length = olength(l1);
  ASSERT_EQ(intv(&l1length), 1);

  object* l2 = list2(*T, *T);
  object l2length = olength(l2);
  ASSERT_EQ(intv(&l2length), 2);

  object* l3 = list3(*T, *T, *T);
  object l3length = olength(l3);
  ASSERT_EQ(intv(&l3length), 3);

  object* l4 = list4(*T, *T, *T, *T);
  object l4length = olength(l4);
  ASSERT_EQ(intv(&l4length), 4);
  PASS();
}

TEST list_append () {
  object* l3 = list3(*T, *T, *T);
  object* l4 = list4(*T, *T, *T, *T);

  object* l7 = oappend(list2(*l3, *l4));
  object l7length = olength(l7);
  ASSERT_EQ(intv(&l7length), 7);

  PASS();
}

TEST list_push () {
  SKIP();
}

TEST list_pop () {
  SKIP();
}

TEST booly_test() {
  ASSERT(is(*booly(3), t));
  ASSERT(is(*booly(0), nil));
  ASSERT(is(*booly(true), t));
  ASSERT(is(*booly(false), nil));
  PASS();
}

TEST string_equal () {
  object s0 = make_string("Hello");
  object s1 = make_string("Goodbye");
  object s2 = make_string("Hello");

  ASSERT(is(*ostring_equal(&s0, &s1), nil));
  ASSERT(is(*ostring_equal(&s0, &s2), t));
  ASSERT(is(*ostring_equal(&s2, &s0), t));
  ASSERT(is(*ostring_equal(&s2, &s2), t));
  PASS();
}

SUITE(unit_math) {

  RUN_TEST(booly_test);

  RUN_TEST(adding_integers_type);
  RUN_TEST(adding_integers_value);

  RUN_TEST(adding_doubles_type);
  RUN_TEST(adding_doubles_value);

  RUN_TEST(list_length);
  RUN_TEST(list_append);
  RUN_TEST(list_push);
  RUN_TEST(list_pop);

  RUN_TEST(string_equal);
}

int main (int argc, char** argv) {
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(unit_math);
  GREATEST_MAIN_END();

}
