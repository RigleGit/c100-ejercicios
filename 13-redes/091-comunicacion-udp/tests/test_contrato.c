#include "comunicacion_udp.h"
#include <assert.h>
int main(void) {
    alarm(10);
    config_udp_t cfg = CONFIG_UDP_DEFECTO;
    cfg.puerto = 0;
    cfg.tipo = UDP_RECEPTOR;
    cfg.timeout_segundos = 1;
    udp_context_t *receiver = crear_contexto_udp(&cfg);
    assert(receiver);
    assert(inicializar_udp(receiver) == UDP_EXITO);
    struct sockaddr_in address;
    socklen_t length = sizeof(address);
    assert(getsockname(receiver->socket_fd, (struct sockaddr *)&address, &length) == 0);
    cfg.tipo = UDP_EMISOR;
    udp_context_t *sender = crear_contexto_udp(&cfg);
    assert(sender);
    assert(enviar_mensaje_udp(sender, "dato", "127.0.0.1", ntohs(address.sin_port)) == UDP_EXITO);
    char buffer[16];
    struct sockaddr_in source;
    assert(recibir_mensaje_udp_timeout(receiver, buffer, sizeof(buffer), &source, 500) == 4);
    assert(strcmp(buffer, "dato") == 0 && source.sin_addr.s_addr == htonl(INADDR_LOOPBACK));
    estadisticas_udp_t stats;
    obtener_estadisticas_udp(receiver, &stats);
    assert(stats.mensajes_recibidos == 1 && stats.bytes_recibidos == 4);
    obtener_estadisticas_udp(sender, &stats);
    assert(stats.mensajes_enviados == 1 && stats.bytes_enviados == 4);
    assert(recibir_mensaje_udp_timeout(receiver, buffer, sizeof(buffer), NULL, 50) ==
           UDP_ERROR_TIMEOUT);
    assert(enviar_mensaje_udp(NULL, "dato", "127.0.0.1", 9) == UDP_ERROR_PARAMETRO);
    assert(recibir_mensaje_udp(NULL, buffer, sizeof(buffer), NULL) == UDP_ERROR_PARAMETRO);
    destruir_contexto_udp(sender);
    destruir_contexto_udp(receiver);
    return 0;
}
