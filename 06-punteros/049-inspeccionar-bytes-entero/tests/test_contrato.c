#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
uint32_t extraerBits(int32_t, int, int);
int32_t intercambiarBytes(int32_t);
void modificarByte(int32_t *, int, unsigned char);
int main(void) {
    assert(extraerBits(INT32_C(0x12345678), 4, 12) == UINT32_C(0x567));
    assert(extraerBits(INT32_C(0x12345678), 0, 32) == UINT32_C(0x12345678));
    assert(extraerBits(-1, 0, 32) == UINT32_MAX);
    assert(extraerBits(INT32_MIN, 31, 1) == 1);
    assert(extraerBits(123, -1, 8) == 0);
    assert(extraerBits(123, 31, 2) == 0);
    assert((uint32_t)intercambiarBytes(INT32_C(0x12345678)) == UINT32_C(0x78563412));
    int32_t value = 0;
    modificarByte(&value, 1, 0xAB);
    unsigned char bytes[4];
    memcpy(bytes, &value, 4);
    assert(bytes[0] == 0 && bytes[1] == 0xAB && bytes[2] == 0 && bytes[3] == 0);
    return 0;
}
