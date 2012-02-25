#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "integer.h"

#define MIN(x,y)  ((x)<(y) ? (x) : (y))
#define MAX(x,y)  ((x)>(y) ? (x) : (y))

int main (int argc, const char * argv[])
{
    integer a, b, result;
        
    if(argc == 1 || argc > 5 || strcmp(argv[1], "-h") == 0) {
       fprintf(stderr, "ERROR: Incorrect usage\n\ncalc usage:\n\tcalc <+/-> <?-(b|d|o|x)number> <?-(b|d|o|x)number> (b|d|o|x)\n");
        return 0;
    }
    
        
    a = new_integer_from_string((char *)argv[2], (argv[2][0] == '-' ? argv[2][1] : argv[2][0]));
    b = new_integer_from_string((char *)argv[3], (argv[3][0] == '-' ? argv[3][1] : argv[3][0])); 
    result = new_integer(MAX(a.segments, b.segments) + 1);
    
    if(argv[1][0] == '-') twos_complement_integer(b);
    
    simple_add_integer(a, b, &result);
    
    switch(argv[4][0]) {
        case 'd':
            print_integer_decimal(result);
            break;
        case 'x':
            print_integer_hexadecimal(result);
            break;
        case 'b':
            print_integer_binary(result);
            break;
        case 'o':
            print_integer_octal(result);
            break;
        default:
            fprintf(stderr, "ERROR: %c is not a valid output format.\n", argv[4][0]);
    }
    
    return 0;
}

