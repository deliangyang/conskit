#include <sys/select.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "event.h"
#include "signal.h"
#include "context.h"
#include "event/select.c"


int add(event_loop_t *event_loop, int fd, int which, handle *h, void *data) {
    if (fd > event_loop->num_fd) {
        return -1;
    }

    event_t *event = &event_loop->events[fd];
    event->which |= which;

    if (which & E_READ) {
        event->handle_read = h;
    }
    if (which & E_WRITE) {
        event->handle_write = h;
    }

    int e;
    e = dialect_api_add_event(event_loop, fd, which);
    if (e != 0) {
        return -1;
    }

    if (fd > event_loop->max_fd) {
        event_loop->max_fd = fd;
    }
    event->data = data;
    return 0;
}

int main_loop(event_loop_t *event_loop, struct timeval *timeout) {

    if(event_loop->pid != getpid()) {
        printf("%d = %d\n", event_loop->pid, getpid());
        return 0;
    }

    event_loop->running = 1;
    int num_events, processed_num_events = 0;

    while (event_loop->running) {
        num_events = dialect_api_poll(event_loop, timeout);
        if (num_events > 0) {
            int i, fd, which;
            for (i = 0; i < num_events; i++) {

                fd = event_loop->fired_events[i].fd;
                which = event_loop->fired_events[i].which;

                event_t *event = &event_loop->events[fd];

                if (event->which & E_READ && event->handle_read) {
                    event->handle_read(event_loop, fd, which, event->data);
                    if (event_loop->pid != getpid()) {
                        return 0;
                    }
                }

                if (event->which & E_WRITE && event->handle_write) {
                    event->handle_write(event_loop, fd, which, event->data);
                    if (event_loop->pid != getpid()) {
                        return 0;
                    }
                }

                processed_num_events++;
            }
        }
    }
    return processed_num_events;
}

void free_event_loop(event_loop_t *event_loop) {
    if (event_loop) {
        free(event_loop->fired_events);
        free(event_loop->events);
        free(event_loop);
    }
}

char *get_name() {
    return dialect_api_name();
}

event_loop_t *init_event_loop(int num_fd) {

    event_loop_t *event_loop;
    event_loop = malloc(sizeof(*event_loop));
    if (NULL == event_loop) {
        return NULL;
    }

    event_loop->running = 0;
    event_loop->max_fd = -1;
    event_loop->num_fd = num_fd;
    event_loop->fired_events = malloc(sizeof(event_t) * num_fd);
    event_loop->events = malloc(sizeof(event_t) * num_fd);

    int c;
    c = dialect_api_create(event_loop);
    if (c != 0) {
        goto err;
    }

    int i;
    for (i = 0; i < event_loop->max_fd; i++) {
        event_loop->events[i].which = E_NONE;
    }
    return event_loop;

    err:
        free(event_loop->events);
        free(event_loop->fired_events);
        free(event_loop);

    return NULL;
}

void stop_event_loop(event_loop_t *event_loop) {
    event_loop->running = 0;
}


