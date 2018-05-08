#include <amqp.h>
#include <stdlib.h>

#define amq_bytes(s) amqp_cstring_bytes(s)
#define new_channel(conn,n) amqp_channel_open(conn,n)
#define bytes_to_chars(b) stringify_bytes(b);


char *stringify_bytes(amqp_bytes_t b);