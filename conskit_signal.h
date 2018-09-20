#ifndef GERALT_SIGNAL_H
#define GERALT_SIGNAL_H

#include <signal.h>

int init_master_signal();
int init_worker_signal();

void handle_master_signal();

int get_signal_fd();

#endif
