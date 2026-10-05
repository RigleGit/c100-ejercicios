#include "cliente_http.h"
#include <assert.h>
int main(void) {
    alarm(10);
    url_parseada_t url;
    assert(cliente_http_parsear_url("http://127.0.0.1:8081/a?b=2#c", &url) == CLIENTE_HTTP_OK);
    assert(strcmp(url.host, "127.0.0.1") == 0 && strcmp(url.puerto, "8081") == 0);
    assert(strcmp(url.path, "/a") == 0 && strcmp(url.query, "b=2") == 0 &&
           strcmp(url.fragment, "c") == 0);
    assert(cliente_http_parsear_url(NULL, &url) == CLIENTE_HTTP_ERROR_PARAMETRO);
    peticion_http_t request;
    cliente_http_init_peticion(&request);
    request.version = HTTP_VERSION_1_1;
    strcpy(request.host, "example.test");
    strcpy(request.path, "/a?b=2");
    char buffer[1024];
    ssize_t length = cliente_http_construir_peticion(&request, buffer, sizeof(buffer));
    assert(length > 0 && strstr(buffer, "GET /a?b=2 HTTP/1.1\r\n") == buffer);
    assert(strstr(buffer, "Host: example.test\r\n"));
    assert(cliente_http_construir_peticion(&request, buffer, 4) == -1);
    respuesta_http_t response;
    cliente_http_init_respuesta(&response);
    assert(cliente_http_parsear_headers("HTTP/1.1 404 Not Found\r\nContent-Length: 3", &response) ==
           CLIENTE_HTTP_OK);
    assert(response.codigo_estado == 404 && strcmp(response.razon_estado, "Not Found") == 0);
    cliente_http_liberar_respuesta(&response);
    /* Real socket delivery and peer EOF, with independently constructed HTTP bytes. */
    int sockets[2];
    assert(socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == 0);
    const char *raw = "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nabc";
    size_t raw_length = strlen(raw);
    assert(send(sockets[0], raw, 12, 0) == 12);
    assert(send(sockets[0], raw + 12, raw_length - 12, 0) == (ssize_t)(raw_length - 12));
    shutdown(sockets[0], SHUT_WR);
    cliente_http_init_respuesta(&response);
    assert(cliente_http_recibir_respuesta(sockets[1], &response, 1) == CLIENTE_HTTP_OK);
    assert(response.codigo_estado == 200 && response.bytes_recibidos == raw_length);
    assert(response.tamaño_contenido == 3 && memcmp(response.contenido, "abc", 3) == 0);
    cliente_http_liberar_respuesta(&response);
    close(sockets[0]);
    close(sockets[1]);
    return 0;
}
