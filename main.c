#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "integer.h"


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
    
    a = new_integer_from_string("b111111111111111", TYPE_BIN);
    b = new_integer_from_string("-b111111111111111", TYPE_BIN);
    result = new_integer(MAX(a.segments, b.segments) + 1);
    
    add_integer(a, b, result);
    
    /*                 01000001010000100100001101000100
                       01000001010000100100001101000100
       000000000000000010000010100001001000011010001000
     */
    
    print_integer(a);
    print_integer(b);
    print_integer(result);
    twos_complement_integer(result);
    print_integer(result);
    
    return 0;
}

