#include <signal.h>

#ifndef TASK_WORKER_SIGNALS_H
#define TASK_WORKER_SIGNALS_H

#endif //TASK_WORKER_SIGNALS_H


void handle_master_signal(int n);
void handle_worker_signal(int n);

void init_master_signal();
void init_worker_signal();
int get_signal_fd();