#ifndef TASK_WORKER_BROKER_H
#define TASK_WORKER_BROKER_H

#endif

#include "message.h"


struct broker_t {
    const char *name;
    void *data;
    int (*consume)(struct broker_t *broker ,
                   void (*handle)(struct message_t *m)
    );
    int (*cleanup)(struct broker_t *broker);
    int (*initialize)(struct broker_t *broker);
    int max_run;
};

struct broker_t *get_configured_broker();