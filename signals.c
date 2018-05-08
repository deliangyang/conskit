#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>


#include "signals.h"

static int sfd[2];


void handle_master_signal(int n) {
    if (n == SIGCHLD) {
        pid_t pid;
        int status;
        char *s = "c";
        while(0 < (pid = (waitpid(-1, &status, WUNTRACED | WNOHANG)))) {
            write(sfd[1], s, 1);
        }
    }
}

void handle_worker_signal(int n) {
    // handle worker signal
}


void init_master_signal() {
    struct sigaction act;
    memset(&act, 0, sizeof(act));

    act.sa_handler = handle_master_signal;
    sigfillset(&act.sa_mask);

    sigaction(SIGUSR1, &act, 0);
    sigaction(SIGUSR2, &act, 0);
    sigaction(SIGCHLD, &act, 0);

    socketpair(AF_LOCAL, SOCK_STREAM, 0, sfd);
}

void init_worker_signal() {
    // init worker signal
}

int get_signal_fd() {
    return sfd[0];
}