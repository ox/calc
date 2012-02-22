#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "integer.h"

#define MAX_UNIT ((unit)(-1))
#define bool int
#define true 1
#define false 0

integer new_integer(int segments) {
    integer res;
    res.segments = segments;
    res.c = (unit *)malloc(sizeof(unit) * segments);
    return res;
}

void set_zero_integer(integer i) {
    memset(i.c, 0, sizeof(unit) * i.segments);
}

void copy_integer(integer source, integer target) {
    memmove(target.c, source.c, sizeof(unit)*MIN(source.segments, target. segments));
    if (target.segments > source.segments)
        memset(target.c + source.segments, 0,sizeof(unit)*(target.segments - source.segments));
}

void free_integer(integer f) {
    free(f.c);
}

void print_integer(integer res) {
    int i, k = ( 1 << (sizeof(unit) * 8 - 1));
    for(i = 0; i <= res.segments - 1; i++) {
        for(; k != 0 ; k >>= 1) {
            if(res.c[i] & k) printf("1");
            else printf("0");
        }
        k = ( 1 << (sizeof(unit) * 8 - 1));
    }
    printf("\n");
}

void twos_complement_integer(integer res) {
    int i;
    for(i = 0; i < res.segments; i++) {
        res.c[i] = ~res.c[i];
    }
    res.c[i-1] += 1;
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
    return result;
}

integer new_integer_from_binary_string(char *str, int type) {
    int len = strlen(str);
    int i, k = 1, set;
    integer result = new_integer(ceil((float)len/(sizeof(unit)*8)));
    set = result.segments - 1;
    
    for(i = len - 1; i >= 0 ; i --) {
        if(str[i] == '1') result.c[set] |= k;
        
        if(k == (1 << 15)) {
            k = 1;
            set--;
        } else
            k <<= 1;
    }
    
    return result;
}

integer new_integer_from_hex_string(char *str, int type){}
integer new_integer_from_dec_string(char *str, int type){}
integer new_integer_from_oct_string(char *str, int type){}

void add_integer(integer left, integer right, integer result) {
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
        result.c[result.segments - i - 1] = (unit)partial_sum;
    }
}


