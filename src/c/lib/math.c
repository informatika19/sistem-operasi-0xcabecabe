/**
 * math.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 20 Februari 2020
 *
 * Header yang berisi fungsi-fungsi matematika
 */

#include "math.h"

int div(int tempdividend, int tempdivisor) {
    int quotient = 1;
    if (tempdivisor == tempdividend) {
        return 1;
    } else if (tempdividend < tempdivisor) {
        return 0;
    }   

    do{

        tempdivisor = tempdivisor << 1;
        quotient = quotient << 1;

     } while (tempdivisor <= tempdividend);


     /* Call division recursively */
    quotient = quotient + division(tempdividend - tempdivisor, tempdivisor);

    return quotient;
} 

int mod(int a, int b)
{
    int hasil = div(a,b);
    return a - b*hasil;
}