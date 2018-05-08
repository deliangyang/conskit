#include "utils.h"


// @reference https://stackoverflow.com/questions/36430875/converting-amqp-cstring-bytes-back-to-c-string

char *stringify_bytes(amqp_bytes_t bytes)
{
    char *res = malloc(bytes.len * 4 + 1);
    uint8_t *data = bytes.bytes;
    char *p = res;
    size_t i;

    for (i = 0; i < bytes.len; i++) {
        if (data[i] >= 32 && data[i] != 127) {
            *p++ = data[i];
        } else {
            *p++ = '\\';
            *p++ = '0' + (data[i] >> 6);
            *p++ = '0' + (data[i] >> 3 & 0x7);
            *p++ = '0' + (data[i] & 0x7);
        }
    }

    *p = 0;
    return res;
}