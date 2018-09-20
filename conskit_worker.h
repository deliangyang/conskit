#ifndef GERALT_WORKER_H
#define GERALT_WORKER_H

#include "conskit_event.h"

int cleanup_worker();
int create_worker();



void handle_worker_signal(
        struct event_loop_t *loop,
        int fd,
        int which,
        void *data
);


#endif
