#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#include "integer.h"

#define MAX_UNIT ((unit)(-1))
#define LBIT (1 << (sizeof(unit) * 8 - 1))
#define UNIT_BITS  (sizeof(unit)*CHAR_BIT)
#define LOG_2_10    3.3219280948873623478703194294894

integer new_integer(int segments) {
    integer res;
    res.segments = segments;
    res.c = (unit *)malloc(sizeof(unit) * segments);
    return res;
}

void set_zero_integer(integer i) {
    memset(i.c, 0, sizeof(unit) * i.segments);
}

bool is_zero_integer(integer result) {
    int i;
    for(i = 0; i < result.segments; i++) {
        if(result.c[i] != 0) return false;
    }
    
    return true;
}

void trim_integer(integer * result) {
    int neg = (unit)result->c[0] & LBIT;
    if(neg) {
        while(result->c[0] == MAX_UNIT && result->segments > 1 && result->c[1] & LBIT) {
            result->c++;
            result->segments--;
        }
    } else {
        while(result->c[0] == 0) {
            result->c++;
            result->segments--;
        }
    }
}

void copy_integer(integer source, integer target) {
    memmove(target.c, source.c, sizeof(unit)*MIN(source.segments, target. segments));
    if (target.segments > source.segments)
        memset(target.c + source.segments, 0,sizeof(unit)*(target.segments - source.segments));
}

void free_integer(integer f) {
    free(f.c);
}

void twos_complement_integer(integer res) {
    int i;
    for(i = 0; i < res.segments; i++) {
        res.c[i] = ~res.c[i];
    }
    res.c[i-1] += 1;
}

int is_negative(integer res) {
    return res.c[0] & LBIT;
}

void print_integer_binary(integer res) {
    int i;
    unit k = ( 1 << (sizeof(unit) * 8 - 1));
    for(i = 0; i <= res.segments - 1; i++) {
        for(; k != 0 ; k >>= 1) {
            if(res.c[i] & k) printf("1");
            else printf("0");
        }
        k = ( 1 << (sizeof(unit) * 8 - 1));
    }
    printf("\n");
}

void print_integer_decimal(integer x) {
    int i;
    char* result =
    (char*)malloc((int)ceil(UNIT_BITS*x.segments/LOG_2_10) + 2);
    int neg = x.c[0] & LBIT;
    integer ten = new_integer(1);
    ten.c[0] = 10;
    
    if(neg) {
        printf("-");
        twos_complement_integer(x);
    }
    
    if (is_zero_integer(x)) {
        strcpy(result, "0");
    } else {
        for (i = 0; !is_zero_integer(x); i++) {
            result[i] = (char)mod_small_integer(x, 10) + '0';
            divide_small_integer(x, 10, x);
        }
        result[i] = '\0';
    }
    
    for(i = strlen(result) - 1; i >= 0; i--) {
        printf("%c", result[i]);
    }
    printf("\n");
}



integer new_integer_from_string(char *str, int type) {
    bool neg;
    integer result;
    neg = str[0] == '-' ? true : false;
    str = str + 1 + neg;
    
    if(neg) {
        char *full = (char *)malloc(sizeof(char) * (strlen(str) + 2));
        sprintf(full, "1%s", str);
        str = full;
    }
    
    switch(type) {
        case TYPE_BIN:
            result = new_integer_from_binary_string(str, type);
            break;
        case TYPE_HEX:
            result = new_integer_from_hex_string(str, type);
            break;
        case TYPE_DEC:
            result = new_integer_from_dec_string(str, type);
            break;
        case TYPE_OCT:
            result = new_integer_from_oct_string(str, type);
        default: 
            fprintf(stderr, "NO VALID TYPE SET WHEN CALLING new_integer_from_string");
            exit(1);
    }
    
    if(neg) twos_complement_integer(result);
    
    return result;
}

integer new_integer_from_binary_string(char *str, int type) {
    int len = strlen(str);
    int i, k = 1, set = 0;
    integer result = new_integer((int)ceil((float)len/(sizeof(unit)*8)));
    unit tmp = 0;
    
    for(i = len - 1; i >= 0 ; i --) {
        if(str[i] == '1') tmp |= k;
        
        if(k == LBIT || i == sizeof(unit)) {
            k = 1;
            result.c[set] = tmp;
            set ++;
            tmp = 0;
        } else
            k <<= 1;
    }
    
    if(result.c[0] & LBIT)
        twos_complement_integer(result);
    
    return result;
}

integer new_integer_from_hex_string(char *str, int type){}
integer new_integer_from_dec_string(char *str, int type){}
integer new_integer_from_oct_string(char *str, int type){}

void add_integer(integer left, integer right, integer * result) {
    double_unit carry = 0;
    int i;
    
    for(i=0; i < MAX(left.segments, right.segments) || carry != 0; i++) {
        double_unit partial_sum = carry;
        carry = 0;
        if ( i < left.segments )  partial_sum += left.c[left.segments - 1 - i];
        if ( i < right.segments ) partial_sum += right.c[right.segments - 1 - i];
        if (partial_sum > MAX_UNIT) {
            partial_sum &= MAX_UNIT;
            carry = 1;
        }
        result->c[result->segments - i - 1] = (unit)partial_sum;
    }    
    
    trim_integer(result);
}

void subtract_integer(integer left, integer right, integer * result) {
    int borrow = 0;
    int i;

    for(i=0; i<left.segments; i++) {
        double_unit lhs = left.c[i];
        double_unit rhs = (i < right.segments) ? right.c[i] : 0;
        if (borrow) {
            if (lhs <= rhs) {
                /* leave borrow set to 1 */
                lhs += (MAX_UNIT + 1) - 1;
            } else {
                borrow = 0;
                lhs--;
            }
        }
        
        if (lhs < rhs) {
            borrow = 1;
            lhs += MAX_UNIT + 1;
        }
        result->c[result->segments - i - 1] = lhs - rhs;
    }
    trim_integer(result);
}

unit mod_small_integer(integer left, unit right) {
    double_unit mod_two_power = 1;
    double_unit result = 0;
    int i, bit;
    for(i=0; i<left.segments; i++) {
        for(bit=0; bit<UNIT_BITS; bit++) {
            if ((left.c[i] & (1 << bit)) != 0) {
                result += mod_two_power;
                if (result >= right) {
                    result -= right;
                }
            }
            mod_two_power <<= 1;
            if (mod_two_power >= right) {
                mod_two_power -= right;
            }
        }
    }
    return (unit)result;
}

void divide_small_integer(integer left, unit right, integer result) {
    double_unit dividend = 0;
    int i;
    for (i = left.segments - 1; i >= 0; i--) {
        dividend |= left.c[i];
        result.c[i] = dividend/right;
        dividend = (dividend % right) << UNIT_BITS;
    }
}
