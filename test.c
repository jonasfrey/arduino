#include <stdio.h>

void i_do_print_a_string(){

    printf(" ! a string ! ");

}


#include "i_do_print_another_string.c"

void main()
{
   /* Our first simple C basic program */
   printf("Hello World! ");

    i_do_print_a_string();


    i_do_print_another_string();

}

