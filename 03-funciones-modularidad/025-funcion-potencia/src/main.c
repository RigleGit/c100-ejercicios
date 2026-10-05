#include "potencia.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

static int fin_valido(const char *fin) {
    while (isspace((unsigned char)*fin)) ++fin;
    return *fin == '\0';
}
static int leer_linea(char *buffer, size_t n) {
    return fgets(buffer, (int)n, stdin) && (strchr(buffer, '\n') || feof(stdin));
}
int main(void) {
    char entrada[128], *fin;
    printf("Introduce la base: ");
    if (!leer_linea(entrada, sizeof entrada)) return 1;
    errno = 0;
    double base = strtod(entrada, &fin);
    if (fin == entrada || errno || !fin_valido(fin) || !isfinite(base)) return 1;
    printf("Introduce el exponente (entero): ");
    if (!leer_linea(entrada, sizeof entrada)) return 1;
    errno = 0;
    long n = strtol(entrada, &fin, 10);
    if (fin == entrada || errno || !fin_valido(fin) || n < INT_MIN || n > INT_MAX) return 1;
    double resultado = potencia(base, (int)n);
    if (!isfinite(resultado)) {
        fprintf(stderr, "Potencia fuera del dominio o del rango representable.\n");
        return 1;
    }
    printf("%.2f elevado a %ld es: %.5f\n", base, n, resultado);
    return 0;
}
