#include "cliente_tcp.h"
#include <assert.h>
int main(void) {
    alarm(10);
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    assert(listener >= 0);
    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    socklen_t length = sizeof(address);
    assert(bind(listener, (struct sockaddr *)&address, sizeof(address)) == 0);
    assert(getsockname(listener, (struct sockaddr *)&address, &length) == 0);
    assert(listen(listener, 1) == 0);
    cliente_tcp_t *client = cliente_crear("127.0.0.1", ntohs(address.sin_port));
    assert(client);
    assert(cliente_set_timeouts(client, 1, 1) == CLIENTE_OK);
    assert(cliente_enviar(client, "hola", 4) == CLIENTE_ERROR_CONEXION);
    assert(cliente_conectar(client) == CLIENTE_OK);
    int peer = accept(listener, NULL, NULL);
    assert(peer >= 0);
    struct timeval timeout = {1, 0};
    assert(setsockopt(peer, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == 0);
    assert(cliente_enviar(client, "hola", 4) == 4);
    char buffer[16];
    assert(recv(peer, buffer, sizeof(buffer), 0) == 4);
    assert(memcmp(buffer, "hola", 4) == 0);
    assert(send(peer, "ok", 2, 0) == 2);
    assert(cliente_recibir(client, buffer, sizeof(buffer)) == 2 && strcmp(buffer, "ok") == 0);
    assert(client->bytes_enviados == 4 && client->bytes_recibidos == 2);
    assert(cliente_recibir(client, buffer, sizeof(buffer)) == CLIENTE_ERROR_TIMEOUT);
    close(peer);
    assert(cliente_recibir(client, buffer, sizeof(buffer)) == 0);
    assert(!cliente_esta_conectado(client));
    cliente_destruir(client);
    close(listener);
    assert(cliente_crear(NULL, 9) == NULL);
    assert(cliente_crear("127.0.0.1", 65536) == NULL);
    assert(cliente_enviar(NULL, "x", 1) == CLIENTE_ERROR_PARAMETROS);
    return 0;
}
