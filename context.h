#include <unistd.h>

struct global_context {
    pid_t parent_pid;
    pid_t pid;
    int is_worker;
    int max_times;
};

