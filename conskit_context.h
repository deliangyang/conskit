#ifndef GERALT_CONTEXT_H
#define GERALT_CONTEXT_H

#include <stdlib.h>
#include "conskit_event.h"

typedef struct geralt_context_t {
    pid_t process_id;
    event_loop_t *event_loop;
} geralt_context_t;


void init_context();

extern geralt_context_t geralt_context;

#endif
