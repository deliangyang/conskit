#ifndef EVENT_H
#define EVENT_H

#include <stdlib.h>

#define E_NONE 0
#define E_READ 1
#define E_WRITE 2


struct event_loop_t;

typedef void handle(
    struct event_loop_t *loop,
    int fd,
    int which,
    void *data
);


typedef struct event_t {
    int which;                /* 事件 E_(NONE|READ|WRITE|BARRIER) */
    void *data;               /* 特定的IO多路接口的数据 */
    handle *handle_write;     /* 处理写事件 */
    handle *handle_read;      /* 处理读事件 */
} event_t;

typedef struct fired_event_t {
    int which;      /* 事件 E_(NONE|READ|WRITE|BARRIER) */
    int fd;         /* 文件描述符 */
} fired_event_t;

typedef struct event_loop_t {

    int max_fd;         /* 监控的最大文件描述符数量 */
    int num_fd;         /* 监控的文件描述符数量 */
    int running;
    void *data;         /* 特定的IO多路接口的数据 */

    event_t *events;
    fired_event_t *fired_events;
    pid_t pid;

} event_loop_t;

event_loop_t *init_event_loop(int num_fd);
void free_event_loop(event_loop_t *event_loop);
int add(event_loop_t *event_loop, int fd, int which, handle *h, void *data);
int del(event_loop_t *event_loop, int fd, int which);
int main_loop(event_loop_t *event_loop, struct timeval *timeout);

void stop_event_loop(event_loop_t *event_loop);

char *get_name();

#endif
