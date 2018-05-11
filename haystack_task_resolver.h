#ifndef HAYSTACK_TASK_RESOLVER
#define HAYSTACK_TASK_RESOLVER


#define TASK_MESSAGE_CLS  "cls", 3, 0
#define TASK_MESSAGE_ARGS "args", 4, 0
#define TASK_MESSAGE_ID   "id", 2, 0

void execute(zval *data);
int jsonify(char *data, zval *result);

#endif