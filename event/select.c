#include <sys/select.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct api_dialect_select {
    fd_set rs;
    fd_set ws;

    fd_set _rs;
    fd_set _ws;
} api_dialect_select;


static int dialect_api_create(event_loop_t *event_loop) {
    api_dialect_select *d = malloc(sizeof(api_dialect_select));
    if (!d) {
        return -1;
    }
    FD_ZERO(&d->ws);
    FD_ZERO(&d->rs);
    event_loop->data = d;
    return 0;
}

static void dialect_api_free(event_loop_t *event_loop) {
    if(event_loop->data) {
        free(event_loop->data);
    }
}

static int dialect_api_add_event(event_loop_t *event_loop, int fd, int which) {

    api_dialect_select *d = event_loop->data;
    if (fd > FD_SETSIZE) {
        return -1;
    }

    if (which & E_READ) {
        FD_SET(fd, &d->rs);
    }
    if (which & E_WRITE) {
        FD_SET(fd, &d->ws);
    }
    return 0;
}

static int dialect_api_del_event(event_loop_t *event_loop, int fd, int which) {

    api_dialect_select *d = event_loop->data;

    if (which & E_READ) {
        FD_CLR(fd, &d->rs);
    }
    if (which & E_WRITE) {
        FD_CLR(fd, &d->ws);
    }
    return 0;
}

static int dialect_api_poll(event_loop_t *event_loop, struct timeval *tv) {

    if(event_loop->max_fd == -1) {
        return 0;
    }

    api_dialect_select *dialect = event_loop->data;
    memcpy(&dialect->_rs, &dialect->rs, sizeof(fd_set));
    memcpy(&dialect->_ws, &dialect->ws, sizeof(fd_set));

    int num, i, j=0;
    num = select(event_loop->max_fd + 1, &dialect->_rs, &dialect->_ws, NULL, NULL);
    if(num > 0) {
        for(i=0; i<=event_loop->max_fd; i++) {
            int which = E_NONE;
            event_t *e = &event_loop->events[i];
            if(e->which == E_NONE) {
                continue;
            }
            if(e->which & E_READ && FD_ISSET(i, &dialect->_rs)) {
                which |= E_READ;
            }
            if(e->which & E_WRITE && FD_ISSET(i, &dialect->_ws)) {
                which |= E_WRITE;
            }
            event_loop->fired_events[j].fd = i;
            event_loop->fired_events[j].which = which;
            j++;
        }
    }
    return j;
}

static char *dialect_api_name() {
    return "select";
}