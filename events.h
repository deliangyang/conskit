#ifndef TASK_WORKER_EVENTS_H
#define TASK_WORKER_EVENTS_H

#endif //TASK_WORKER_EVENTS_H


struct event_entry {
    struct event_entry *next;
    int fd;
    int (*callback)(struct event_entry *e, void *data);
};

struct events {
    struct event_entry *head;
    int size;
};

int initialize(int max_fd);
int add(int fd, short mask, int (*callback)(struct event_entry *e, void *data));
int del(int fd);
int cleanup(struct event_entry *e);
int event_loop(int timeout);
struct event_entry *find(int fd);

