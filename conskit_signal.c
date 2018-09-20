#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


static int sfd[2];

int set_unblock(int fd) {
    int flag;
    flag = fcntl(fd, F_GETFL);
    if (flag < 0) {
        return -1;
    }
    flag |= O_NONBLOCK;
    return fcntl(fd, F_SETFL, flag);
}

void handle_master_signal(int signal) {
    char c;
    switch(signal) {
        case SIGCHLD:
            c = 'c';
            break;

        default:
            c = '1';
    }
    write(sfd[1], &c, 1);
}

int init_worker_signal() {
    close(sfd[0]);
    close(sfd[1]);
    return 0;
}

int init_master_signal() {
    struct sigaction act;
    int ret;
    ret = socketpair(AF_UNIX, SOCK_STREAM, 0, sfd);
    if (ret < 0) {
        return -1;
    }

    if (set_unblock(sfd[0]) != 0 || set_unblock(sfd[1]) != 0) {
        return -1;
    }

    fcntl(sfd[0], F_SETFD, FD_CLOEXEC);
    fcntl(sfd[1], F_SETFD, FD_CLOEXEC);

    memset(&act, 0, sizeof(act));
    act.sa_handler = handle_master_signal;
    sigfillset(&act.sa_mask);

    if (0 > sigaction(SIGINT,   &act, 0) ||
        0 > sigaction(SIGUSR1,  &act, 0) ||
        0 > sigaction(SIGUSR2,  &act, 0) ||
        0 > sigaction(SIGCHLD,  &act, 0) ||
        0 > sigaction(SIGQUIT,  &act, 0)) {
        return -1;
    }
    return 0;
}

int get_signal_fd() {
    return sfd[0];
}