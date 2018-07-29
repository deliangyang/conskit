#ifndef HAYSTACK_TASK_RESOLVER
#define HAYSTACK_TASK_RESOLVER

#include "ext/pcre/php_pcre.h"


#define TASK_MESSAGE_CLS  "cls", 3, 0
#define TASK_MESSAGE_ARGS "args", 4, 0
#define TASK_MESSAGE_ID   "id", 2, 0

#define TASK_ANNOTATION_NAME "name"
#define TASK_ANNOTATION_QUEUE "queue"
#define TASK_ANNOTATION_RKEY "routing_key"
#define TASK_ANNOTATION_EXCHANGE "exchange"

#define TASK_PROPERTY_VALUES "values"
#define TASK_EXECUTE_METHOD "execute"

#define TASK_ANNOTATION_NAME_PATTERN "/@name\\s+(?P<name>[^\\s]+)/"
#define TASK_ANNOTATION_QUEUE_PATTERN "/@queue\\s+(?P<queue>[^\\s]+)/"
#define TASK_ANNOTATION_EXCHANGE_PATTERN "/@exchange\\s+(?P<exchange>[^\\s]+)/"
#define TASK_ANNOTATION_ROUTING_KEY_PATTERN "/@exchange\\s+(?P<exchange>[^\\s]+)/"

#define TASK_ANNOTATION_NAME_REGEX zend_string_init(TASK_ANNOTATION_NAME, strlen(TASK_ANNOTATION_NAME), 0)
#define TASK_ANNOTATION_QUEUE_REGEX zend_string_init(TASK_ANNOTATION_QUEUE_PATTERN, strlen(TASK_ANNOTATION_QUEUE_PATTERN), 0)
#define TASK_ANNOTATION_EXCHANGE_REGEX zend_string_init(TASK_ANNOTATION_EXCHANGE_PATTERN, strlen(TASK_ANNOTATION_EXCHANGE_PATTERN), 0)
#define TASK_ANNOTATION_RKEY_REGEX zend_string_init(TASK_ANNOTATION_ROUTING_KEY_PATTERN, strlen(TASK_ANNOTATION_EXCHANGE_PATTERN), 0)


void execute(zval *data);
int jsonify(char *data, zval *result);
void resolve_task_annotations(zend_string *, zval *annoations);


#endif
