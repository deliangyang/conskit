#ifndef HAYSTACK_TASK_RESOLVER
#define HAYSTACK_TASK_RESOLVER


#define TASK_MESSAGE_CLS  "cls", 3, 0
#define TASK_MESSAGE_ARGS "args", 4, 0
#define TASK_MESSAGE_ID   "id", 2, 0

#define TASK_ANNOTATION_NAME "name"
#define TASK_ANNOTATION_QUEUE "queue"
#define TASK_ANNOTATION_RKEY "routing_key"
#define TASK_ANNOTATION_EXCHANGE "exchange"

#define TASK_ANNOTATION_NAME_REGEX zend_string_init(ZEND_STRL("/@name\\s+(?P<name>[^\\s]+)/"), 0)
#define TASK_ANNOTATION_QUEUE_REGEX zend_string_init(ZEND_STRL("/@queue\\s+(?P<queue>[^\\s]+)/"), 0)
#define TASK_ANNOTATION_EXCHANGE_REGEX zend_string_init(ZEND_STRL("/@exchange\\s+(?P<exchange>[^\\s]+)/"), 0)
#define TASK_ANNOTATION_RKEY_REGEX zend_string_init(ZEND_STRL("/@routing_key\\s+(?P<routing_key>[^\\s]+)/"), 0)


void execute(zval *data);
int jsonify(char *data, zval *result);
void resolve_task_annotations(zend_string *, zval *annoations);


#endif
