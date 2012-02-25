#ifndef bignum_h
#define bignum_h

#define bool int
#define true 1
#define false 0

#define TYPE_DEC 100
#define TYPE_BIN 98
#define TYPE_OCT 111
#define TYPE_HEX 120

#define MIN(x,y)  ((x)<(y) ? (x) : (y))
#define MAX(x,y)  ((x)>(y) ? (x) : (y))

/*  lets refactor the code. The first thing is to set a consistent
    size for the segments */

typedef unsigned short unit;
typedef unsigned long double_unit;

typedef struct integer {
    unit* c;
    int segments;
} integer;

integer new_integer(int segments);
void set_zero_integer(integer i);
bool is_zero_integer(integer result);
void trim_integer(integer * result);
void copy_integer(integer source, integer target);
void free_integer(integer f);
void twos_complement_integer(integer res);
int is_negative(integer res);
int compare_integers(integer left, integer right);

void print_integer_binary(integer x);
void print_integer_decimal(integer x);
void print_integer_hexadecimal(integer x);
void print_integer_octal(integer x);
void print_error(char *str, int type);

integer new_integer_from_string(char *str, int type);
integer new_integer_from_binary_string(char *str);
integer new_integer_from_hex_string(char *st);
integer new_integer_from_dec_string(char *st);
integer new_integer_from_oct_string(char *st);


void add_integer(integer left, integer right, integer *result);
void simple_add_integer(integer top, integer bottom, integer *result);

void subtract_integer(integer left, integer right, integer *result);
unit mod_small_integer(integer left, unit right);
void divide_small_integer(integer left, unit right, integer result);
void multiply_small_integer(integer left, unit right, integer result);

void shift_integer_left(integer x);
void shift_integer_right(integer x);

#endif
