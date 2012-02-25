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
    
    if(is_negative(res)) {
        printf("-");
        twos_complement_integer(res);
    }
    
    printf("b");
    
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
    
    copy_integer(res, x);
    
    if(is_negative(x)) {
        printf("-");
        twos_complement_integer(x);
    }
    
    printf("d");
    
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
    free_integer(x);
}

void print_integer_hexadecimal(integer x) {
    int i, dec;
    char t;
    unit k = 0xf000;
    dec = 12;
    
    if(is_negative(x)) {
        printf("-");
        twos_complement_integer(x);
    }
    printf("x");
    
    for(i = x.segments - 1; i >= 0 ; i--) {
        for (dec = 12; dec >= 0; dec -= 4) {
            t = ((x.c[i] & k) >> dec);
            t <= 9 ? (t += 48) : (t += 55);
            printf("%c", t);
            k >>= 4;
        }
        k = 0xf000;
    }
    printf("\n");
}

void print_integer_octal(integer x) {
    int i;
    char* result =
    (char*)malloc((int)ceil(UNIT_BITS*x.segments/LOG_2_10) + 2);
        
    if(is_negative(x)) {
        printf("-");
        twos_complement_integer(x);
    }
    
    printf("o");
    
    if (is_zero_integer(x)) {
        strcpy(result, "0");
    } else {
        for (i = 0; !is_zero_integer(x); i++) {
            result[i] = (char)mod_small_integer(x, 8) + '0';
            divide_small_integer(x, 8, x);
        }
        result[i] = '\0';
    }
    
    for(i = strlen(result) - 1; i >= 0; i--) {
        printf("%c", result[i]);
    }
    printf("\n");
    free_integer(x);
}


integer new_integer_from_string(char *str, int type) {
    bool neg;
    integer result;
    char *full;
    int i;
    
    neg = str[0] == '-' ? true : false;
    str = str + 1 + neg;
    
    full = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    sprintf(full, "%i%s", neg, str);
    str = full;
    
    switch(type) {
        case TYPE_BIN:
            for(i = 0; i < strlen(str); i++) 
                if( str[i] < 48 || str[i] > 49 ) print_error(str, type);
            result = new_integer_from_binary_string(str);
            break;
        case TYPE_HEX:
            for(i = 0; i < strlen(str); i++) 
                if( str[i] < 48 || (str[i] > 57 && str[i] < 65) || str[i] > 70) print_error(str, type);
            result = new_integer_from_hex_string(str);
            break;
        case TYPE_DEC:
            for(i = 0; i < strlen(str); i++) 
                if( str[i] < 48 || str[i] > 57) print_error(str, type);
            result = new_integer_from_dec_string(str);
            break;
        case TYPE_OCT:
            for(i = 0; i < strlen(str); i++) 
                if( str[i] < 48 || str[i] > 55) print_error(str, type);
            result = new_integer_from_oct_string(str);
            break;
        default: 
            fprintf(stderr, "ERROR: no such formating type (%c) when formating (%s)\n", type, str);
            exit(1);

    }
        
    return result;
}

void print_error(char *str, int type) {
    fprintf(stderr, "ERROR: incorrect formating type (%c) when formating (%s)\n", type, str);
    exit(1);
}

integer new_integer_from_binary_string(char *str) {
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

integer new_integer_from_hex_string(char *str){
    int len;
    int i, k = 0, set = 0;
    bool neg;
    integer result;
    unit tmp = 0;
    
    if(str[0] == '1')
        neg = true;
    else
        neg = false;
    
    str = str + 1;

    len = strlen(str);
    result = new_integer((int)ceil((float)(len*4)/(sizeof(unit)*8)) + 1);

    for(i = len - 1; i >= 0 ; i --) {
        if(str[i] >= 48 && str[i] <= 57) {
            tmp |= (str[i] - 48) << k;
        } else if(str[i] >= 65 && str[i] <= 70) {
            tmp |= (str[i] - 55) << k;
        }
        
        if(k == 12 || i == 0) {
            k = 0;
            result.c[set] = tmp;
            set ++;
            tmp = 0;
        } else
            k += 4;
    }
    
    if(neg) {
        twos_complement_integer(result);
    }
    
    
    return result;
}

integer new_integer_from_dec_string(char *str){
    bool neg;
    integer result, digit;
    int i;
    
    if(str[0] == '1')
        neg = true;
    else
        neg = false;
    
    str = str + 1;
    
    result = new_integer((int)ceil(LOG_2_10*strlen(str)/UNIT_BITS));
    digit = new_integer(1);
    
    set_zero_integer(result);
    for (i = 0; str[i] != '\0'; i++) {
        multiply_small_integer(result, 10, result);
        digit.c[0] = str[i] - '0';
        add_integer(result, digit, &result);
    }
    
    if(neg) twos_complement_integer(result);
    
    free_integer(digit);
    return result;
}

integer new_integer_from_oct_string(char *str){
    int i;
    bool neg;
    integer result, digit;
    
    if(str[0] == '1')
        neg = true;
    else
        neg = false;
    
    str = str + 1;
    
    result = new_integer((int)ceil(LOG_2_10*strlen(str)/UNIT_BITS));
    digit = new_integer(1);
    
    set_zero_integer(result);
    for (i = 0; str[i] != '\0'; i++) {
        multiply_small_integer(result, 8, result);
        digit.c[0] = str[i] - 48;
        add_integer(result, digit, &result);
    }

    if(neg) twos_complement_integer(result);
    free_integer(digit);
    
    print_integer_decimal(result);
    
    return result;
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
    } else {
        lproper = left;
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

void add_integer(integer left, integer right, integer * result) {
    int n = 0;
    double_unit carry = 0;
    
    for(n=0; n<left.segments || n<right.segments || carry != 0; n++) {
        double_unit partial_sum = carry;
        carry = 0;
        if (n < left.segments)  partial_sum += left.c[n];
        if (n < right.segments) partial_sum += right.c[n];
        if (partial_sum > MAX_UNIT) {
            partial_sum &= MAX_UNIT;
            carry = 1;
        }
        result->c[n] = (unit)partial_sum;
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

void multiply_small_integer(integer left, unit right, integer result) {
    double_unit carry = 0;
    int i;
    for(i=0; i<left.segments || carry != 0; i++) {
        double_unit partial_sum = carry;
        carry = 0;
        if (i < left.segments)  partial_sum += left.c[i]*right;
        carry = partial_sum >> UNIT_BITS;
        result.c[i] = (unit)(partial_sum & MAX_UNIT);
    }
    for ( ; i < result.segments; i++) { result.c[i] = 0; }
}

void shift_integer_left(integer x) {
    int i;
    int neg = is_negative(x);
    if(neg) twos_complement_integer(x);
    
    x.c[x.segments - 1] <<= 1;
    for (i = x.segments - 2; i >= 0; i--) {
        x.c[i + 1] |= x.c[i] >> (UNIT_BITS - 1);
        x.c[i] <<= 1;
    }
    if(neg) twos_complement_integer(x);
}


void shift_integer_right(integer x) {
    int i;
    int neg = is_negative(x);
    if(neg) twos_complement_integer(x);
    
    x.c[0] >>= 1;
    for (i = 1; i < x.segments; i++) {
        x.c[i - 1] |= (x.c[i] & 1) << (UNIT_BITS - 1);
        x.c[i] >>= 1;
    }
    if(neg) twos_complement_integer(x);
}

