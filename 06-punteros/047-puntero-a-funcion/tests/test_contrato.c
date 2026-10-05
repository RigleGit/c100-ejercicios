#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
int sumar(int, int);
int restar(int, int);
int multiplicar(int, int);
int dividir(int, int);
int calculadora(int, int, int (*)(int, int), const char *);
int calculadora_dinamica(int, int, char);
int comparar(int, int);
void ordenar_con_funcion(int *, int, int (*)(int, int));
int procesar_numero(int, int (*)(int, int));
int main(void) {
    assert(calculadora(7, -3, sumar, "sumar") == 4);
    assert(calculadora_dinamica(7, 3, '-') == 4);
    assert(calculadora_dinamica(7, 3, '*') == 21);
    assert(calculadora_dinamica(7, 3, '/') == 2);
    assert(calculadora_dinamica(7, 3, '%') == 1);
    assert(calculadora_dinamica(2, 5, '^') == 32);
    assert(calculadora_dinamica(48, 18, 'g') == 6);
    int a[] = {5, -2, 5, 0, 1};
    const int expected[] = {-2, 0, 1, 5, 5};
    ordenar_con_funcion(a, 5, comparar);
    assert(memcmp(a, expected, sizeof a) == 0);
    assert(procesar_numero(4, restar) == -4);
    assert(procesar_numero(-3, multiplicar) == 18);
    return 0;
}
