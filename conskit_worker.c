#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>


#include "conskit_worker.h"


void handle_worker_signal(
        event_loop_t *event_loop, int fd, int which, void *data) {
    char c;
    ssize_t n;
    do {
        do {
            n = read(fd, &c, 1);
        } while (n == -1 && errno == EINTR);

        if (n < 0) {
            break;
        }
        switch (c) {
            case 'c':
                cleanup_worker();
                break;

            default:
                break;
        }
        if(event_loop->pid != getpid()) {
            break;
        }
    } while (1);
}


int cleanup_worker() {

    pid_t pid;
    int status;

    while((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {

        if(WIFEXITED(status)) {
            char buf[128];
            int sl;
            sl = sprintf(buf, "cleanup worker %d, exit status: %d\n", pid, WEXITSTATUS(status));
            write(STDOUT_FILENO, buf, sl);

            pid_t worker_pid;
            worker_pid = fork();
            if (worker_pid == 0) {
                printf("New worker %d, pid: %d\n", getpid(), getppid());
                break;
            }

        } else if (WIFSIGNALED(status)) {
            // todo
        } else if (WIFSTOPPED(status)) {
            // todo
        }
    }
    return 1;
}
