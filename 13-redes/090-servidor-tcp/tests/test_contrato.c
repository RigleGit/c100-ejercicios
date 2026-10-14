#include "servidor_tcp.h"
#include <assert.h>
int main(void) {
    alarm(10);
    config_servidor_t cfg = CONFIG_SERVIDOR_DEFECTO;
    strcpy(cfg.host, "127.0.0.1");
    cfg.puerto = 0;
    cfg.timeout_cliente_seg = 1;
    servidor_tcp_t *server = crear_servidor(&cfg);
    assert(server);
    assert(iniciar_servidor(server) == SERVIDOR_EXITO);
    struct sockaddr_in address;
    socklen_t length = sizeof(address);
    assert(getsockname(server->socket_servidor, (struct sockaddr *)&address, &length) == 0);
    int peer = socket(AF_INET, SOCK_STREAM, 0);
    assert(peer >= 0);
    assert(connect(peer, (struct sockaddr *)&address, sizeof(address)) == 0);
    struct timeval timeout = {1, 0};
    assert(setsockopt(peer, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == 0);
    info_cliente_t client;
    assert(aceptar_cliente(server, &client) == SERVIDOR_EXITO);
    assert(send(peer, "eco", 3, 0) == 3);
    assert(procesar_cliente_eco(server, &client) == SERVIDOR_EXITO);
    char buffer[16];
    assert(recv(peer, buffer, sizeof(buffer), 0) == 3);
    assert(memcmp(buffer, "eco", 3) == 0);
    estadisticas_servidor_t stats;
    obtener_estadisticas_servidor(server, &stats);
    assert(stats.mensajes_procesados == 1 && stats.bytes_transferidos == 6);
    shutdown(peer, SHUT_WR);
    assert(procesar_cliente_eco(server, &client) == SERVIDOR_SHUTDOWN);
    desconectar_cliente(server, &client);
    close(peer);
    destruir_servidor(server);
    assert(iniciar_servidor(NULL) == SERVIDOR_ERROR_PARAMETRO);
    assert(procesar_cliente_eco(NULL, NULL) == SERVIDOR_ERROR_PARAMETRO);
    return 0;
}
