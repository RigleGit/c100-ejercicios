#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
void copiarCadena(char *, const char *);
char *copiarCadena_retorno(char *, const char *);
size_t copiarCadena_segura(char *, const char *, size_t);
char *copiarCadena_overlap(char *, const char *);
char *invertirCadena(char *);
int main(void) {
    char dst[16];
    memset(dst, 'X', sizeof dst);
    copiarCadena(dst, "C 42");
    assert(strcmp(dst, "C 42") == 0 && dst[5] == 'X');
    assert(copiarCadena_retorno(dst, "") == dst && dst[0] == '\0');
    char small[5] = {'?', '?', '?', '?', 'Z'};
    assert(copiarCadena_segura(small, "abcdef", 4) == 3);
    assert(strcmp(small, "abc") == 0 && small[4] == 'Z');
    char overlap[10] = {'a', 'b', 'c', 'd', 'e', 'f', 0, 'Q', 'Z', 'Q'};
    assert(copiarCadena_overlap(overlap + 2, overlap) == overlap + 2);
    assert(memcmp(overlap, "ababcdef", 8) == 0 && overlap[8] == '\0' && overlap[9] == 'Q');
    char text[] = "ab cd";
    assert(invertirCadena(text) == text && strcmp(text, "dc ba") == 0);
    return 0;
}
