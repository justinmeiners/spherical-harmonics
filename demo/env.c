#include <assert.h>
#include "env.h"

static float fact_table[] =
{
    1.0,
    1.0, 	2.0, 	6.0, 	24.0, 	120.0,
    720.0, 	5040.0, 	40320.0, 	362880.0, 	3628800.0,
    39916800.0, 	479001600.0, 	6227020800.0, 	87178291200.0, 	1307674368000.0,
    20922789888000.0, 	355687428096000.0, 	6.402373705728e+15, 	1.21645100408832e+17,
};

float fast_fact(int n)
{
    assert(n >= 0 && n < sizeof(fact_table) / sizeof(float));
    return fact_table[n];
}


