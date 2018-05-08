#include <stdio.h>
#include <unistd.h>

#include <amqp_tcp_socket.h>
#include <amqp_framing.h>

#include "amqp.h"


static struct broker_t amq = {
        .name="amqp",
        .initialize=amq_init,
        .consume=amq_consume,
        .cleanup=amq_cleanup,
        .data=NULL,
};


int amq_init(struct broker_t *b) {
    struct broker_t_amq *t = (struct broker_t_amq *)b->data;
    struct amq_connection_parameter *p = t->parameter;

    int ret;
    t->connection = amqp_new_connection();
    t->sock = amqp_tcp_socket_new(t->connection);
    ret = amqp_socket_open(t->sock, p->host, p->port);
    if (ret != 0) {
        return -1;
    }

    amqp_rpc_reply_t reply;
    reply = amqp_login(
            t->connection,
            p->vh,
            0,
            AMQP_DEFAULT_FRAME_SIZE,
            0,
            AMQP_SASL_METHOD_PLAIN,
            p->username,
            p->password
    );
    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        return -1;
    }

    char tag[16];
    sprintf(tag, "%s.%d", "worker", rand());
    new_channel(t->connection, t->channel);

    amqp_basic_consume(
            t->connection,
            t->channel,
            amq_bytes("aa"),
            amq_bytes(tag),
            0, 1, 0, amqp_empty_table
    );

    sprintf(tag, "%s.%d", "w2", rand());
    amqp_basic_consume(
            t->connection,
            t->channel,
            amq_bytes("bb"),
            amq_bytes(tag),
            0, 1, 0, amqp_empty_table
    );

    return 0;
}

int amq_cleanup(struct broker_t *b) {
    struct broker_t_amq *t = (struct broker_t_amq *)b->data;
    amqp_connection_close(t->connection, AMQP_REPLY_SUCCESS);
    return 0;
}

int amq_consume(struct broker_t *b, void (*handle)(struct message_t *)) {

    struct broker_t_amq *t = (struct broker_t_amq *)b->data;

    amqp_rpc_reply_t ret;
    struct message_t m;
    int n = 0;

    while(t->run) {
        amqp_maybe_release_buffers(t->connection);
        ret = amqp_consume_message(t->connection, &t->envelope, NULL, 0);
        if (ret.reply_type == AMQP_RESPONSE_NORMAL) {
            m.data = bytes_to_chars(t->envelope.message.body);
            handle(&m);
            n++;
            if (n == b->max_run) {
                t->run = 0;
            }
        }
    }
    b->cleanup(b);
    return n;
}

struct broker_t *get_configured_broker() {
    return &amq;
}