#ifndef bignum_h
#define bignum_h

#define bool int
#define true 1
#define false 0

#define TYPE_DEC 0
#define TYPE_BIN 1
#define TYPE_OCT 2
#define TYPE_HEX 3

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

void print_integer_binary(integer res);
void print_integer_decimal(integer x);


integer new_integer_from_string(char *str, int type);
integer new_integer_from_binary_string(char *str, int type);
integer new_integer_from_hex_string(char *str, int type);
integer new_integer_from_dec_string(char *str, int type);
integer new_integer_from_oct_string(char *str, int type);


void add_integer(integer left, integer right, integer *result);
void subtract_integer(integer left, integer right, integer *result);
unit mod_small_integer(integer left, unit right);
void divide_small_integer(integer left, unit right, integer result);

#endif
