#include "comunicacion_udp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        printf("Uso: %s emitir HOST PUERTO MENSAJE | recibir PUERTO\n", argv[0]);
        return 0;
    }
    int emitir = argc == 5 && strcmp(argv[1], "emitir") == 0;
    int recibir = argc == 3 && strcmp(argv[1], "recibir") == 0;
    if (!emitir && !recibir) return 1;
    char *fin;
    errno = 0;
    long puerto = strtol(argv[emitir ? 3 : 2], &fin, 10);
    if (errno || *fin || puerto < 1 || puerto > 65535) return 1;
    resultado_udp_t resultado = emitir
        ? demo_emisor_udp(argv[2], (int)puerto, argv[4])
        : demo_receptor_udp((int)puerto, 1);
    return resultado == UDP_EXITO ? 0 : 1;
}
