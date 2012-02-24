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
    if(is_negative(*result)) {
        while( result->c[result->segments - 1] == MAX_UNIT && (result->c[result->segments - 2] & LBIT) > 0 && result->segments > 1) {
            result->segments --;
        }
    } else {
        
    }
}

void copy_integer(integer source, integer target) {
    memmove(target.c, source.c, sizeof(unit)*MIN(source.segments, target.segments));
    if (target.segments > source.segments)
        memset(target.c + source.segments, is_negative(source) * MAX_UNIT,sizeof(unit)*(target.segments - source.segments));
}

void free_integer(integer f) {
    free(f.c);
}

void twos_complement_integer(integer res) {
    int i;
    integer one = new_integer(1);
    
    for(i = 0; i < res.segments; i++) {
        res.c[i] = ~res.c[i];
    }
    
    one.c[0] = 1;
    add_integer(res, one, &res);
    free_integer(one);
}

int is_negative(integer res) {
    return (res.c[res.segments - 1] & LBIT) != 0 ? 1 : 0;
}

int compare_integers(integer left, integer right) {
    int i = MAX(left.segments - 1, right.segments - 1);
    for ( ; i >= 0; i--) {
        unit left_comp =
        (i < left.segments) ? left.c[i] : 0;
        unit right_comp =
        (i < right.segments) ? right.c[i] : 0;
        if (left_comp < right_comp)
            return -1;
        else if (left_comp > right_comp)
            return  1;
    }
    return 0;
}

void print_integer_binary(integer res) {
    int i;
    unit k = LBIT;
    for(i = res.segments - 1; i >= 0 ; i--) {
        for(; k != 0 ; k >>= 1) {
            if(res.c[i] & k) printf("1");
            else printf("0");
        }
        k = LBIT;
    }
    printf("\n");
}

void print_integer_decimal(integer res) {
    int i;
    integer x = new_integer(res.segments);
    char* result =
    (char*)malloc((int)ceil(UNIT_BITS*x.segments/LOG_2_10) + 2);
    integer ten = new_integer(1);
    ten.c[0] = 10;
    
    copy_integer(res, x);
    
    if(is_negative(x)) {
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
    free_integer(ten);
    free_integer(x);
}



integer new_integer_from_string(char *str, int type) {
    bool neg;
    integer result;
    char *full;
    
    neg = str[0] == '-' ? true : false;
    str = str + 1 + neg;
    
    full = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    sprintf(full, "%i%s", neg, str);
    str = full;
    
    switch(type) {
        case TYPE_BIN:
            result = new_integer_from_binary_string(str, type);
            break;
        /*case TYPE_HEX:
            result = new_integer_from_hex_string(str, type);
            break;
        case TYPE_DEC:
            result = new_integer_from_dec_string(str, type);
            break;
        case TYPE_OCT:
            result = new_integer_from_oct_string(str, type);*/
        default: 
            fprintf(stderr, "NO VALID TYPE SET WHEN CALLING new_integer_from_string");
            exit(1);
    }
        
    return result;
}

integer new_integer_from_binary_string(char *str, int type) {
    int len;
    int i, k = 1, set = 0;
    bool neg;
    integer result;
    unit tmp = 0;
    
    if(str[0] == '1') {
        str = str + 1;
        neg = true;
    } else {
        neg = false;
    }
    
    len = strlen(str);
    result = new_integer((int)ceil((float)len/(sizeof(unit)*8)));
    
    for(i = len - 1; i >= 0 ; i --) {
        if(str[i] == '1') tmp |= k;
        
        if(k == LBIT || i == 0) {
            k = 1;
            result.c[set] = tmp;
            set ++;
            tmp = 0;
        } else
            k <<= 1;
    }
    
    if(neg) {
        twos_complement_integer(result);
    }
        
    
    return result;
}

/*integer new_integer_from_hex_string(char *str, int type){}
integer new_integer_from_dec_string(char *str, int type){}
integer new_integer_from_oct_string(char *str, int type){}*/

void add_integer(integer left, integer right, integer * result) {
    double_unit carry = 0;
    int i;
    
    for(i=0; i<left.segments || i<right.segments || carry != 0; i++) {
        double_unit partial_sum = carry;
        carry = 0;
        if (i < left.segments)  partial_sum += left.c[i];
        if (i < right.segments) partial_sum += right.c[i];
        if (partial_sum > MAX_UNIT) {
            partial_sum &= MAX_UNIT;
            carry = 1;
        }
        result->c[i] = (unit)partial_sum;
    }
    trim_integer(result);
}

void simple_add_integer(integer left, integer right, integer *result) {
    integer lproper, rproper;
    bool rgl_segments = right.segments - left.segments;
    bool rgl;
    bool lneg = is_negative(left), rneg = is_negative(right);
    
    if(lneg) twos_complement_integer(left);
    if(rneg) twos_complement_integer(right);
    rgl = compare_integers(left, right);
    if(lneg) twos_complement_integer(left);
    if(rneg) twos_complement_integer(right);
        
    if(rgl_segments < 0) {
        rproper = new_integer(left.segments);
        copy_integer(right, rproper);
        lproper = left;
    } else if(rgl_segments > 1) {
        lproper = new_integer(right.segments);
        copy_integer(left, lproper);
        rproper = right;
    }
    
    if(!lneg && !rneg) {
        add_integer(lproper, rproper, result);
    } else if (lneg && rneg) {
        twos_complement_integer(lproper);
        twos_complement_integer(rproper);
        add_integer(lproper, rproper, result);
        twos_complement_integer(*result);
    } else if (!lneg && rneg && rgl == -1) {
        twos_complement_integer(rproper);
        subtract_integer(rproper, lproper, result);
        twos_complement_integer(*result);
    } else if (lneg && !rneg && rgl == 1) {
        twos_complement_integer(lproper);
        subtract_integer(lproper, rproper, result);
        twos_complement_integer(*result);
    } else if (lneg && !rneg && rgl == -1) {
        twos_complement_integer(rproper);
        subtract_integer(lproper, rproper, result);
    } else if (!lneg && rneg && rgl == 1) {
        twos_complement_integer(lproper);
        subtract_integer(rproper, lproper, result);
    }
    
    free_integer(lproper);
    free_integer(rproper);
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
        result->c[i] = lhs - rhs;
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

void shift_integer_right(integer x) {
    
}
