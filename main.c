#include <amqp.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <time.h>

#include "brokers/amqp.h"
#include "events.h"
#include "signals.h"

#define FD_SIZE 128


int add_event(int, short);


struct global_context {
    int run;
    pid_t pid;
    struct timespec start_at;
};

static struct global_context *context;

static struct pollfd *fds;


void handle(struct message_t *m) {
    printf("%d => %s  \n", getpid(), m->data);
}

void run(amqp_channel_t channel) {
    struct amq_connection_parameter p = {
            .host="localhost",
            .port=5672,
            .vh="/",
            .username="guest",
            .password="guest",
            .exchange="default"
    };

    struct broker_t *t = get_configured_broker();

    struct broker_t_amq b;
    b.parameter = &p;
    b.run = 1;
    b.channel = channel;
    t->max_run = 3;
    t->data = (void *)&b;
    t->initialize(t);
    t->consume(t, handle);
    exit(0);
}

pid_t create_worker() {
    printf("start worker\n");
    pid_t  pid = fork();
    if(pid == 0) {
        run((amqp_channel_t )getpid());
        return 0;
    } else {
        return pid;
    }
}

int handle_event(struct event_entry *e, void *data) {
    char buf[1];
    read(e->fd, buf, 1);
    printf("worker '%s' exit.\n", buf);
    create_worker();
    return 0;
}

int run_master() {

    initialize(128);

    context = malloc(sizeof(struct global_context));
    context->run = 1;
    context->pid = getpid();
    clock_gettime(CLOCK_REALTIME, &context->start_at);

    fds = malloc(sizeof(struct pollfd *) * FD_SIZE);
    int i=0;
    for(; i<128; i++) {
        fds[i].fd = -1;
    }
    int fd = get_signal_fd();
    int idx = add_event(fd, POLLIN);
    add(fd, POLLIN, handle_event);
    int ret, k, n = idx+1;
    printf("n: %d\n", n);
    while(context->run) {
        printf("pid: %d\n", getpid());
        ret = poll(fds, n, -1);
        if (ret > 0) {
            for(k=0; k<n; k++) {
                if(fds[k].revents & POLLIN) {
                    struct event_entry *e = find(fds[k].fd);
                    e->callback(e, (void *)0);
                }
            }
        }
    }
    return 0;
}

int add_event(int fd, short events) {
    int i = 0;
    for(; i<FD_SIZE; i++) {
        if(fds[i].fd == -1) {
            break;
        }
    }

    fds[i].fd = fd;
    fds[i].events = events;
    return i;
}

int main() {
    init_master_signal();
    int i=0;
    pid_t pid;
    for(; i<3; i++) {
        pid = fork();
        if (pid == 0) {
            run(i+1);
            continue;
        }
    }
    run_master();
}
