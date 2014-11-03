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

/**
 * Copy an object.
 */
object* ocopy(object*);

/**
 * Allocate an object
 */
object* oalloc(void);

#endif
