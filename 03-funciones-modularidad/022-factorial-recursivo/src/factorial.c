#include "factorial.h"
#include <limits.h>

static long calcular(int n) {
    return n < 2 ? 1 : n * calcular(n - 1);
}
long factorial(int n) {
    long acumulado = 1;
    int limite = 1;
    for (int i = 2; acumulado <= LONG_MAX / i; ++i) {
        acumulado *= i;
        limite = i;
    }
    if (n < 0 || n > limite) return -1;
    return calcular(n);
}
