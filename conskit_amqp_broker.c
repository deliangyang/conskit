#include <stdlib.h>

#include "conskit_amqp_broker.h"


char *bytes_as_string(amqp_bytes_t bytes) {

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

amqp_connection_state_t make_rb_connection(
        char *host,
        int port
) {

    amqp_connection_state_t connect;
    amqp_socket_t *sock;

    connect = amqp_new_connection();
    sock = amqp_tcp_socket_new(connect);
    amqp_socket_open(sock, host, port);

    return connect;
}


amqp_rpc_reply_t rb_auth(
        amqp_connection_state_t connection,
        char *username,
        char *password,
        char *vhost
) {

    amqp_rpc_reply_t reply;
    reply = amqp_login(
            connection,
            vhost,
            0,
            AMQP_DEFAULT_FRAME_SIZE,
            0,
            AMQP_SASL_METHOD_PLAIN,
            username,
            password
    );

    return reply;
}


amqp_channel_open_ok_t *open_rb_channel(
        amqp_connection_state_t connection,
        int channel
) {
    return amqp_channel_open(connection, channel);
}


void rb_basic_consume(
        amqp_connection_state_t connection,
        int channel,
        char *q,
        char *tag
) {
    amqp_table_t table = {0, NULL};
    amqp_basic_consume(
            connection, channel, amqp_cstring_bytes(q), amqp_cstring_bytes(tag),
            0, 1, 0, table
    );

}


int rb_consume(
        amqp_connection_state_t connection,
        int (*handle)(amqp_envelope_t *envelople),
        int consumed
) {

    amqp_envelope_t envelope;
    amqp_rpc_reply_t reply;

    int n;
    for(n=0;;n++) {
        amqp_maybe_release_buffers(connection);
        reply = amqp_consume_message(connection, &envelope, NULL, 0);
        if (reply.reply_type == AMQP_RESPONSE_NORMAL) {
            handle(&envelope);
        } else {
            return -1;
        }
        if (n >= consumed) {
            amqp_destroy_connection(connection);
            exit(0);
        }
    }

    return 0;
}
