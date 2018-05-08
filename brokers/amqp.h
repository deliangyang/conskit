#include <amqp.h>

#include "../broker.h"
#include "amqp/utils.h"


struct amq_connection_parameter {
    char *host;
    int port;
    char *username;
    char *password;
    char *exchange;
    char *vh;
};


struct broker_t_amq {
    amqp_connection_state_t connection;
    amqp_socket_t *sock;
    amqp_channel_t channel;
    amqp_envelope_t envelope;

    struct amq_connection_parameter *parameter;
    int run:1;
};


int amq_init(struct broker_t *);
int amq_cleanup(struct broker_t *);
int amq_consume(struct broker_t *, void (*handle)(struct message_t *m));

