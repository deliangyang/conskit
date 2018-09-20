#include <amqp_tcp_socket.h>

#define DEFAULT_AMQP_HOST "localhost"
#define DEFAULT_AMQP_PORT 5672
#define DEFAULT_AMQP_USERNAME "guest"
#define DEFAULT_AMQP_PASSWORD "guest"
#define DEFAULT_AMQP_VHOST "/"


struct amqp_connection_parameters {
    char *host;
    char *username;
    char *password;
    char *vhost;
    int port;
};


char *bytes_as_string(
        amqp_bytes_t bytes
);


amqp_connection_state_t make_rb_connection(
        char *host,
        int port
);


amqp_rpc_reply_t rb_auth(
        amqp_connection_state_t connection,
        char *username,
        char *password,
        char *vhost
);


amqp_channel_open_ok_t *open_rb_channel(
        amqp_connection_state_t connection,
        int channel
);


void rb_basic_consume(
        amqp_connection_state_t connection,
        int channel,
        char *q,
        char *tag
);


int rb_consume(
        amqp_connection_state_t connection,
        int (*handle)(amqp_envelope_t *envelople),
        int consumed
);