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

TEST number_equal_int() {
  object a = make_int(1);
  object b = make_int(90000);
  ASSERT(ofalsy(*onumber_equal(&a, &b)));

  object c = make_int(100001);
  object d = make_int(100001);
  ASSERT(otruthy(*onumber_equal(&d, &c)));
  ASSERT(otruthy(*onumber_equal(&c, &d)));

  object e = make_int(3993);
  object f = make_int(intv(&e));
  ASSERT(otruthy(*onumber_equal(&e, &f)));
  f = oadd(list2(e, f));
  ASSERT(ofalsy(*onumber_equal(&e, &f)));

  object g = make_int(-9000000);
  object h = make_int(-9000000);
  ASSERT(otruthy(*onumber_equal(&h, &g)));
  intv(&h) = intv(&h) * -1;
  ASSERT(ofalsy(*onumber_equal(&h, &g)));

  PASS();
}

TEST number_equal_double() {
  object a = make_double(1.0);
  object b = make_double(90000.0);
  ASSERT(ofalsy(*onumber_equal(&a, &b)));

  object c = make_double(100.001);
  object d = make_double(100.001);
  ASSERT(otruthy(*onumber_equal(&d, &c)));
  ASSERT(otruthy(*onumber_equal(&c, &d)));

  object e = make_double(39.93);
  object f = make_double(doublev(&e));
  ASSERT(otruthy(*onumber_equal(&e, &f)));
  f = oadd(list2(e, f));
  ASSERT(ofalsy(*onumber_equal(&e, &f)));

  object g = make_double(-9000000.0000001);
  object h = make_double(-9000000.0000001);
  ASSERT(otruthy(*onumber_equal(&h, &g)));
  doublev(&h) = doublev(&h) * -1;
  ASSERT(ofalsy(*onumber_equal(&h, &g)));

  PASS();
}

TEST number_equal_mixed() {
  object a = make_double(1);
  object b = make_int(90000);
  ASSERT(ofalsy(*onumber_equal(&a, &b)));

  object c = make_double(100001);
  object d = make_int(100001);
  ASSERT(otruthy(*onumber_equal(&d, &c)));
  ASSERT(otruthy(*onumber_equal(&c, &d)));

  object e = make_int(3993);
  object f = make_double(intv(&e));
  ASSERT(otruthy(*onumber_equal(&e, &f)));
  f = oadd(list2(e, f));
  ASSERT(ofalsy(*onumber_equal(&e, &f)));

  object g = make_int(-9000);
  object h = make_double(-9000);
  ASSERT(otruthy(*onumber_equal(&h, &g)));
  doublev(&h) *= -1;
  ASSERT(ofalsy(*onumber_equal(&h, &g)));

  object i = make_int(5);
  object j = make_double(5.000000001);
  ASSERT(ofalsy(*onumber_equal(&i, &j)));
  doublev(&j) -= 0.000000001;
  ASSERT(otruthy(*onumber_equal(&i, &j)));
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

  object* l5 = list4(*T, *T, *T, *T);
  cdr(cdr(cdr(cdr(l5)))) = l4;
  object l5length = olength(l5);
  ASSERT_EQ(intv(&l5length), 8);
  PASS();
}

TEST list_append () {
  object* l3 = list3(make_int(1), make_int(2), make_int(3));
  object* l4 = list4(make_int(4), make_int(5), make_int(6), make_int(7));

  object* l7 = oappend(list2(*l3, *l4));
  object l7length = olength(l7);
  ASSERT_EQ(intv(&l7length), 7);

  int i = 0;
  ofor_each(elm, head, l7) {
    i++;
    ASSERT(intv(elm) == i);
  }

  object* lbig = oappend(list4(*l4, *l4, *l3, *l4));
  object lbiglength = olength(lbig);
  ASSERT_EQ(intv(&lbiglength), 15);

  int i2 = 0;
  ofor_each(elm, head, lbig) {
    i2 += intv(elm);
  }
  ASSERT(i2 == ((4 + 5 + 6 + 7) * 3) + (1 + 2 + 3));

  PASS();
}

TEST list_push () {
  object* l1 = list1(make_int(5));
  object* l2 = opush(make_int(33), l1);
  l2 += 0;
  ASSERT(olength(l1).value.int_v == 2);
  ASSERT(olength(l2).value.int_v == 2);
  ASSERT(l1 == l2);
  ASSERT(intv(&car(l2)) == 33);
  ASSERT(intv(&car(cdr(l2))) == 5);
  ASSERT(is(*cdr(cdr(l2)), nil));
  PASS();
}

TEST list_pop () {
  object* l3 = list3(make_int(5), make_int(4), make_int(3));
  object* l3head = opop(l3);
  ASSERT(intv(l3head) == 5);
  ASSERT(olength(l3).value.int_v == 2);
  ASSERT(intv(&car(l3)) = 4);
  ASSERT(intv(&car(cdr(l3))) = 3);

  object* l1 = list1(make_string("hi"));
  object* l1head = opop(l1);
  ASSERT(olength(l1).value.int_v == 0);
  ASSERT(is(*l1, nil));
  ASSERT(is(*l1head, string));
  object s = make_string("hi");
  ASSERT(otruthy(*ostring_equal(l1head, &s)));
  PASS();
}

TEST list_last() {
  object* l3 = list3(make_int(3), make_int(4), make_int(5));
  object* l3last = olast(l3);
  ASSERT(olength(l3last).value.int_v == 1);
  ASSERT(intv(&car(l3last)) == 5);
  ASSERT(is(*olast(NIL), nil));
  PASS();
}

TEST booly_test() {
  ASSERT(is(*booly(3), t));
  ASSERT(is(*booly(0), nil));
  ASSERT(is(*booly(true), t));
  ASSERT(is(*booly(false), nil));
  PASS();
}

TEST for_each_test() {
  object* list = list4(make_int(1), make_int(2), make_double(3), make_double(4));
  int i = 0;
  double x = 0;
  ofor_each(value, head, list) {
    i++;
    x += numberv(*value);
  }
  ASSERT(olength(list).value.int_v == 4);
  ASSERT(i == 4);
  ASSERT(x == 10);
  PASS();
}

TEST object_copy() {
  object i = make_int(3);
  object d = make_double(5);
  object s = make_string("Hello");
  object b = make_byte('c');
  object* l = cons(make_int(3), NIL);
  object* nil = NIL;
  object* t = T;

  object* icopy = ocopy(&i);
  ASSERT(is(*icopy, int));
  ASSERT(intv(icopy) == intv(&i));
  ASSERT(icopy != &i);

  object* dcopy = ocopy(&d);
  ASSERT(is(*dcopy, double));
  ASSERT(doublev(dcopy) == doublev(&d));
  ASSERT(dcopy != &d);

  object* scopy = ocopy(&s);
  ASSERT(is(*scopy, string));
  ASSERT(stringv(scopy) != stringv(&i));
  ASSERT(otruthy(*ostring_equal(scopy, &s)));
  ASSERT(icopy != &i);

  object* bcopy = ocopy(&b);
  ASSERT(is(*bcopy, byte));
  ASSERT(bytev(bcopy) == bytev(&b));
  ASSERT(bcopy != &b);

  object* lcopy = ocopy(l);
  ASSERT(is(*lcopy, cell));
  ASSERT(lcopy != l);
  ASSERT(cellv(lcopy) != cellv(l));
  printf("todo when object equals is done, object_equal\n");
  //ASSERT(otruthy(*oequal(lcopy, l)));
  object lcopy_length = olength(lcopy);
  object l_length = olength(l);
  ASSERT_EQ(intv(&lcopy_length), intv(&l_length));

  object* superl = list4(make_int(3), make_string("foo"), make_byte('d'), *T);
  superl = cons(*list1(make_double(3.3)), superl);
  object* superlcopy = ocopy(superl);
  ASSERT(is(car(superlcopy), cell));
  ASSERT(is(car(&car(superlcopy)), double));
  ASSERT(olength(&car(superlcopy)).value.int_v == 1);
  ASSERT(is(car(cdr(superlcopy)), int));
  ASSERT(is(car(cdr(cdr(superlcopy))), string));
  ASSERT(is(car(cdr(cdr(cdr(superlcopy)))), byte));
  ASSERT(is(car(cdr(cdr(cdr(cdr(superlcopy))))), t));
  ASSERT(olength(superlcopy).value.int_v == 5);

  object* nilcopy = ocopy(nil);
  ASSERT(is(*nilcopy, nil));
  ASSERT(nilcopy == NIL);

  object* tcopy = ocopy(t);
  ASSERT(is(*tcopy, t));
  ASSERT(tcopy == T);

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
  RUN_TEST(adding_integers_type);
  RUN_TEST(adding_integers_value);

  RUN_TEST(adding_doubles_type);
  RUN_TEST(adding_doubles_value);

  RUN_TEST(number_equal_int);
  RUN_TEST(number_equal_double);
  RUN_TEST(number_equal_mixed);

}

SUITE(unit_list) {
  RUN_TEST(list_length);
  RUN_TEST(list_last);
  RUN_TEST(list_append);
  RUN_TEST(list_push);
  RUN_TEST(list_pop);
}

SUITE(unit_string) {
  RUN_TEST(string_equal);
}

SUITE(unit_object) {
  RUN_TEST(booly_test);
  RUN_TEST(for_each_test);
  RUN_TEST(object_copy);
}

int main (int argc, char** argv) {
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(unit_math);
  RUN_SUITE(unit_list);
  RUN_SUITE(unit_string);
  RUN_SUITE(unit_object);
  GREATEST_MAIN_END();

}
