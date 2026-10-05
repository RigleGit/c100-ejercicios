#include "factorial.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>

int main(void) {
    char entrada[64], *fin;
    printf("Introduce un número entero no negativo: ");
    if (!fgets(entrada, sizeof entrada, stdin)) return 1;
    if (!strchr(entrada, '\n') && !feof(stdin)) return 1;
    errno = 0;
    long valor = strtol(entrada, &fin, 10);
    if (fin == entrada || errno || valor < 0 || valor > INT_MAX) return 1;
    while (isspace((unsigned char)*fin)) ++fin;
    if (*fin) return 1;
    long resultado = factorial((int)valor);
    if (resultado < 0) {
        fprintf(stderr, "El factorial no cabe en el tipo long.\n");
        return 1;
    }
    printf("El factorial de %ld es: %ld\n", valor, resultado);
    return 0;
}
