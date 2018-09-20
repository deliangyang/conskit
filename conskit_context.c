#include <unistd.h>

#include "conskit_context.h"
#include "conskit_signal.h"
#include "conskit_worker.h"


struct geralt_context_t geralt_context = {
        .event_loop = NULL,
        .process_id = 0,
};


void init_context() {
    geralt_context.process_id = getpid();
    event_loop_t *event_loop = init_event_loop(128);
    event_loop->pid = geralt_context.process_id;
    geralt_context.event_loop = event_loop;
    init_master_signal();
    printf("%d\n", get_signal_fd());
    add(event_loop, get_signal_fd(), E_READ, handle_worker_signal, (void *)0);
}

