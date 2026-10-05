#include "servidor_tcp_multicliente.h"
#include <assert.h>
static void bounded(int fd) {
    struct timeval timeout = {1, 0};
    assert(setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == 0);
}
int main(void) {
    alarm(10);
    ConfigServidor cfg;
    configurar_servidor_por_defecto(&cfg);
    cfg.log_detallado = 0;
    cfg.max_conexiones = 2;
    assert(validar_configuracion_servidor(&cfg) == SERVER_EXITO);
    ConfigServidor bad = cfg;
    bad.puerto = -1;
    assert(validar_configuracion_servidor(&bad) == SERVER_ERROR_CONFIGURACION);
    assert(inicializar_servidor(NULL, &cfg) == SERVER_ERROR_CONFIGURACION);
    ContextoServidor ctx;
    assert(inicializar_servidor(&ctx, &cfg) == SERVER_EXITO);
    int a[2], b[2], c[2];
    assert(socketpair(AF_UNIX, SOCK_STREAM, 0, a) == 0);
    assert(socketpair(AF_UNIX, SOCK_STREAM, 0, b) == 0);
    assert(socketpair(AF_UNIX, SOCK_STREAM, 0, c) == 0);
    bounded(a[1]);
    bounded(b[1]);
    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(1);
    InfoCliente *first = registrar_cliente(&ctx, a[0], &address);
    address.sin_port = htons(2);
    InfoCliente *second = registrar_cliente(&ctx, b[0], &address);
    assert(first && second && first != second);
    assert(ctx.stats.conexiones_activas == 2);
    assert(registrar_cliente(&ctx, c[0], &address) == NULL);
    assert(ctx.stats.conexiones_rechazadas == 1);
    close(c[1]);
    assert(enviar_broadcast(&ctx, "chat", 4, first) == 1);
    char buffer[8];
    assert(recv(b[1], buffer, sizeof(buffer), 0) == 4);
    assert(memcmp(buffer, "chat", 4) == 0);
    assert(recv(a[1], buffer, sizeof(buffer), 0) == -1 &&
           (errno == EAGAIN || errno == EWOULDBLOCK));
    assert(send(a[1], "hola", 4, 0) == 4);
    assert(recibir_de_cliente(first, buffer, sizeof(buffer)) == 4 &&
           memcmp(buffer, "hola", 4) == 0);
    assert(enviar_a_cliente(first, buffer, 4) == 4);
    assert(recv(a[1], buffer, sizeof(buffer), 0) == 4 && memcmp(buffer, "hola", 4) == 0);
    desregistrar_cliente(&ctx, first);
    desregistrar_cliente(&ctx, second);
    assert(ctx.stats.conexiones_activas == 0);
    close(a[1]);
    close(b[1]);
    limpiar_servidor(&ctx);
    return 0;
}
