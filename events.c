#include <stdlib.h>
#include <unistd.h>
#include <poll.h>

#define FD_SIZE 128

#include "events.h"


static struct events *event_entries;
static struct pollfd *fds;

int initialize(int m) {
    event_entries = malloc(sizeof(struct events));
    event_entries->head = NULL;
    event_entries->size = 0;
    fds = malloc(sizeof(struct pollfd) * FD_SIZE);
    int i;
    for(i=0; i<FD_SIZE; i++) {
        fds[i].fd = -1;
    }
    return 0;
}

int add(int fd, short mask, int (*callback)(struct event_entry *, void *data)) {

    int i;
    for(i=0; i<FD_SIZE; i++) {
        if(fds[i].fd == -1) {
            break;
        }
    }

    fds[i].fd = fd;
    fds[i].events = mask;

    struct event_entry *ev = malloc(sizeof(struct event_entry *));
    if(ev == NULL) {
        return -1;
    }

    ev->next = NULL;
    ev->fd = fd;
    ev->callback = callback;

    if(event_entries->head == NULL) {
        event_entries->head = ev;
    } else {
        struct event_entry *current = event_entries->head;
        while(NULL != current->next) {
            current = current->next;
        }
        current->next = ev;
    }
    event_entries->size++;
    return 0;
}

int del(int fd) {

    if(event_entries == NULL) {
        return 0;
    }

    int i=0, found = 0;
    for(; i<FD_SIZE; i++) {
        if (fds[i].fd == fd) {
            found = 1;
        }
    }

    if (found) {
        fds[i].fd = -1;
        fds[i].events = 0;
    }

    struct event_entry *current = event_entries->head;
    if(current->fd == fd) {
        event_entries->head = current->next;
        event_entries->size --;
        free(current);
    } else {
        found = 0;
        struct event_entry *prev = NULL;
        while(current != NULL) {
            if(current->fd == fd) {
                found = 1;
                break;
            }
            prev = current;
            current = current->next;
        }
        if (found) {
            prev->next = current->next;
            event_entries->size--;
            cleanup(current);
        }
    }
    return 0;
}

struct event_entry *find(int fd) {
    if (event_entries == NULL) {
        return NULL;
    }
    struct event_entry *current = event_entries->head;
    while(current) {
        if(current->fd == fd) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int cleanup(struct event_entry *e) {
    if (e == NULL) {
        return -1;
    }
    if(e->fd > 0) {
        close(e->fd);
    }
    free(e);
    return 0;
}

void destroy_event_entries() {
    struct event_entry *current = event_entries->head;
    while(current) {
        cleanup(current);
        current = current->next;
    }
    event_entries->size = 0;
    event_entries->head = NULL;
}


int event_loop(int timeout) {
    int i, ret;
    ret = poll(fds, 128, timeout);
    if (ret > 0) {
        for(i=0; i<128; i++) {
            if(fds[i].fd != -1 && fds[i].revents & POLLIN) {
                struct event_entry *e = find(fds[i].fd);
                e->callback(e, (void *)0);
            }
        }
    }
    return ret;
}