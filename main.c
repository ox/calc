#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "integer.h"

#define MIN(x,y)  ((x)<(y) ? (x) : (y))
#define MAX(x,y)  ((x)>(y) ? (x) : (y))

int main (int argc, const char * argv[])
{
    integer a, b, result;
    
/*    if(argc == 0 || argc > 4 || strcmp(argv[1], "-h") == 0) {
        print_help();
    } else if  {
        op = strcpy(op, argv[1]);
    }
 */
    
    a = new_integer_from_string("-b001000000010101101110110011011001001100100101111010010000100011111110010010001111101000101111100011010", TYPE_BIN);
    b = new_integer_from_string("-b1010010010110100010100101010101000000011111001011100011101011110100010010101010010100000101111111101001010010000001111111110000101001010100011", TYPE_BIN);
    
    result = new_integer(MAX(a.segments, b.segments) + 1);
    
    print_integer_decimal(a);
    print_integer_decimal(b);
    
    simple_add_integer(a, b, &result);
    
    printf("-----------------\n");
    print_integer_decimal(result);
    
    return 0;
}

