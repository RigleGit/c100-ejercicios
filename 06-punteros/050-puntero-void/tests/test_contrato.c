#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
void copiarMemoria(void *, const void *, size_t);
int compararMemoria(const void *, const void *, size_t);
void intercambiarMemoria(void *, void *, size_t);
void llenarMemoria(void *, int, size_t);
void *buscarPatron(const void *, size_t, const void *, size_t);
void procesarArray(void *, size_t, size_t, void (*)(void *, size_t, const char *), const char *);
static void increment(void *element, size_t size, const char *name) {
    assert(size == sizeof(int));
    assert(strcmp(name, "int") == 0);
    ++*(int *)element;
}
int main(void) {
    const unsigned char source[] = {0, 128, 255, 65};
    unsigned char dest[6] = {9, 9, 9, 9, 9, 9};
    copiarMemoria(dest + 1, source, 4);
    const unsigned char expected[] = {9, 0, 128, 255, 65, 9};
    assert(memcmp(dest, expected, 6) == 0);
    unsigned char a[] = {0, 128}, b[] = {1, 128};
    assert(compararMemoria(a, b, 2) < 0);
    assert(compararMemoria(a, a, 2) == 0);
    intercambiarMemoria(a, b, 2);
    assert(a[0] == 1 && a[1] == 128 && b[0] == 0 && b[1] == 128);
    llenarMemoria(dest + 1, 0x1AA, 4);
    assert(dest[0] == 9 && dest[5] == 9);
    for (int i = 1; i < 5; ++i)
        assert(dest[i] == 0xAA);
    unsigned char data[] = {7, 0, 255, 7, 0, 255}, pattern[] = {0, 255}, missing[] = {8};
    assert(buscarPatron(data, 6, pattern, 2) == data + 1);
    assert(buscarPatron(data, 6, missing, 1) == NULL);
    int ints[] = {-1, 0, 9};
    procesarArray(ints, 3, sizeof(int), increment, "int");
    assert(ints[0] == 0 && ints[1] == 1 && ints[2] == 10);
    return 0;
}
