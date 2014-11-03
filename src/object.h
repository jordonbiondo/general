#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>

/**
 * Type Specifiers
 */
enum general_tag {
  int_ot = 0,
  double_ot = 1,
  string_ot = 2,
  byte_ot = 3,
  nil_ot = 4,
  t_ot = 5,
  cell_ot = 6,
  error_ot = 7
};

/**
 * tag -> string helper
 */
const char* tag_string(enum general_tag t) {
  switch(t)
    {
    case int_ot:
      return "int";
    case double_ot:
      return "double";
    case string_ot:
      return "string";
    case byte_ot:
      return "byte";
    case error_ot:
      return "error";
    case nil_ot:
      return "nil";
    case t_ot:
      return "t";
    case cell_ot:
      return "cell";
    }
  return "unknown";
}


/**
 * Nice names
 */
typedef char* string;
typedef char byte;


/**
 * Cell struct
 */
struct general_cell;
typedef struct general_cell cell;

/**
 * Object struct
 */
struct general_object;
typedef struct general_object object;

/**
 * object value union
 */
union general_values {
  int int_v;
  double double_v;
  string string_v; /* also the error */
  byte byte_v;
  cell* cell_v;
};


/**
 * Object definition
 */
struct general_object {
  enum general_tag tag;
  union general_values value;
};


/**
 * Cell definition
 */
struct general_cell {
  object car;
  object* cdr;
};


/**
 * is(object, type)??
 * example: is(var, cell)
 */
#define is(o, type) ((o).tag == type ## _ot)

/**
 * Is the object nil?
 */
#define ofalsy(o) (is((o), nil))


/**
 * Is the object non nil?
 */
#define otruthy(o) (! (ofalsy(o)))


/**
 * Is the object a number type?
 */
bool is_number(object* o) {
  return (is(*o, int) || is(*o, double));
}


/* **************************************************************
 * Exposed functions
 * ************************************************************** */

/**
 * Pretty print object.
 */
void ppo(object);

/**
 * Print a list object.
 */
void pl(object*);

/* **************************************************************
 * List macros
 * ************************************************************** */
#define list1(a)          (cons(a, NIL))
#define list2(b, a)       (cons(b, list1(a)))
#define list3(c, b, a)    (cons(c, list2(b, a)))
#define list4(d, c, b, a) (cons(d, list3(c, b, a)))

/* **************************************************************
 * Because lisp
 * ************************************************************** */
#define car(o)     (cellv(o)->car)
#define cdr(o)     (cellv(o)->cdr)
#define cadr(o)    (car(cdr(o)))
#define caddr(o)   (car(cdr(cdr(o))))
#define cadddr(o)  (car(cdr(cdr(cdr(o)))))
#define caadr(o)   (car(cadr(o)))
#define caaadr(o)  (car(caadr(o)))
#define cdar(o)    (cdr(car(o)))
#define cddar(o)   (cdr(cdr(car(o))))
#define cdddar(o)  (cdr(cdr(cdr(car(o)))))
#define caar(o)    (car(car(o)))
#define cddr(o)    (cdr(cdr(o)))

#define ofirst(o)  (car(o))
#define osecond(o) (cadr(o))
#define othird(o)  (caddr(o))
#define ofourth(o) (cadddr(o))

#define ofor_each(name, head, thing)            \
  for(object* head = (thing);                   \
      head != NULL && is(*head, cell);          \
      head = cdr(head))                         \
    for(object* name = &car(head);              \
        name != NULL;                           \
        name = NULL)

/* **************************************************************
 * nil and t globals for use in C
 * ************************************************************** */
const object nil_global = {
  .tag = nil_ot
};

const object t_global = {
  .tag = t_ot
};

#define NIL ((object*)&nil_global)
#define T ((object*)&t_global)

/* **************************************************************
 * Value macros
 * ************************************************************** */
#define cellv(o)   ((o)->value.cell_v)
#define intv(o)    ((o)->value.int_v)
#define doublev(o) ((o)->value.double_v)
#define stringv(o) ((o)->value.string_v)
#define errorv(o)  ((o)->value.error_v)
#define bytev(o)   ((o)->value.byte_v)

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

object make_int(int);

object make_double(double);

object make_byte(byte);

object make_string(string);

long int objects_allocated = 0;
object** allocated_objects;
long int allocated_objects_length = 0;


/**
 * Copy an object.
 */
object* ocopy(object*);

/**
 * Allocate an object
 */
object* oalloc(void);

/**
 * Free object
 */
int ofree(object*);

/**
 * Create a cons cell
 */
object* cons(object, object*);

/**
 * Add args
 */
object oadd(object*);

/**
 * Subtract args, if only one, negate.
 */
object ominus(object*);

object olength(object*);

object* olast(object*);

object* oappend(object*);

object* opop(object*);

object* opush(object, object*); 

object* onumber_equal(object*, object*);

object* ostring_equal(object*, object*);

object* oequal(object*, object*);

#endif
