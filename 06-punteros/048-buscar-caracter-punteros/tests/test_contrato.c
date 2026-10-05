#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
char *buscarCaracter(char *, char);
char *buscarUltimoCaracter(char *, char);
char *buscarCaracterDesde(char *, char, int);
char *buscarCaracterInsensible(char *, char);
int contarCaracter(char *, char);
int encontrarTodasPosiciones(char *, char, int *, int);
int reemplazarCaracter(char *, char, char);
int main(void) {
    char s[] = "baAba";
    assert(buscarCaracter(s, 'a') == s + 1);
    assert(buscarUltimoCaracter(s, 'a') == s + 4);
    assert(buscarCaracter(s, 'z') == NULL);
    assert(buscarCaracterDesde(s, 'a', 2) == s + 4);
    assert(buscarCaracterInsensible(s, 'B') == s);
    assert(contarCaracter(s, 'a') == 2);
    int p[3] = {-1, -1, 1234};
    assert(encontrarTodasPosiciones(s, 'a', p, 2) == 2 && p[0] == 1 && p[1] == 4 && p[2] == 1234);
    assert(reemplazarCaracter(s, 'a', 'X') == 2 && strcmp(s, "bXAbX") == 0);
    return 0;
}
