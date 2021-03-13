/**
 * math.h
 * Alvin W., Josep M., Rehagana K.C.S.
 * 20 Februari 2020
 *
 * Header yang berisi fungsi-fungsi matematika
 */

#include "math.h"

int div(int dividend, int divisor)
{
    int quotient = 1;

    int neg = 1;
    if ((dividend>0 &&divisor<0)||(dividend<0 && divisor>0))
        neg = -1;

    // Convert to positive
    unsigned int tempdividend = (dividend < 0) ? -dividend : dividend;
    unsigned int tempdivisor = (divisor < 0) ? -divisor : divisor;

    if (tempdivisor == tempdividend) {
        return 1*neg;
    }
    else if (tempdividend < tempdivisor) {
        return 0;
    }
    while (tempdivisor<<1 <= tempdividend)
    {
        tempdivisor = tempdivisor << 1;
        quotient = quotient << 1;
    }

    // Call division recursively
    if(dividend < 0)
        quotient = quotient*neg + div(-(tempdividend-tempdivisor), divisor);
    else
        quotient = quotient*neg + div(tempdividend-tempdivisor, divisor);
    return quotient;
 }

int mod(int a, int b) // fix for negative numbers
{
    int hasil = a/b;
    return a - b*hasil;
}
