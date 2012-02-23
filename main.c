#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "integer.h"

#define MIN(x,y)  ((x)<(y) ? (x) : (y))
#define MAX(x,y)  ((x)>(y) ? (x) : (y))

int main (int argc, const char * argv[])
{
    char *op = (char *)malloc(2 * sizeof(char));
    integer a, b, result;
    
/*    if(argc == 0 || argc > 4 || strcmp(argv[1], "-h") == 0) {
        print_help();
    } else if  {
        op = strcpy(op, argv[1]);
    }
 */
    op = "+";
    
    a = new_integer_from_string("b01001111", TYPE_BIN);
    b = new_integer_from_string("-b00100011", TYPE_BIN);
    result = new_integer(MAX(a.segments, b.segments) + 1);
    
    subtract_integer(a, b, &result);
    print_integer_binary(a);
    print_integer_decimal(a);
    print_integer_binary(b);
    print_integer_decimal(b);
    print_integer_binary(result);
    print_integer_decimal(result);


    return 0;
}

