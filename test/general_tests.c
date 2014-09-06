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
  ppo(result);
  ASSERT_EQm("adding two doubles", doublev(&result), 7.6);

  args = cons(a, NIL);
  result = oadd(args);
  ASSERT_EQm("adding one double", doublev(&result), 3.1);

  args = NIL;
  result = oadd(args);
  ASSERT_EQm("adding no doubles", intv(&result), 0);
  PASS();
}

SUITE(unit_math) {
  RUN_TEST(adding_integers_type);
  RUN_TEST(adding_integers_value);

  RUN_TEST(adding_doubles_type);
  RUN_TEST(adding_doubles_value);
}

int main (int argc, char** argv) {
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(unit_math);
  GREATEST_MAIN_END();

}
