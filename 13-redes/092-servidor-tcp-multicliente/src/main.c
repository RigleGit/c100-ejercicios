#include "servidor_tcp_multicliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        printf("Uso: %s eco|chat PUERTO\n", argv[0]);
        return 0;
    }
    if (argc != 3 || (strcmp(argv[1], "eco") && strcmp(argv[1], "chat"))) return 1;
    char *fin;
    errno = 0;
    long puerto = strtol(argv[2], &fin, 10);
    if (errno || *fin || puerto < 1 || puerto > 65535) return 1;
    return (strcmp(argv[1], "eco") == 0
        ? demo_servidor_eco((int)puerto) : demo_servidor_chat((int)puerto)) == SERVER_EXITO ? 0 : 1;
}
