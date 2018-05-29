#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#include "php.h"
#include "zend_interfaces.h"

#include "haystack_task_resolver.h"


void resolve_task_annotations(zend_string *task_class_name, zval *annoations) {

    zend_class_entry *ce = zend_lookup_class(task_class_name);
    if(ce == NULL) {
        return ;
    }

    array_init(annoations);
    pcre_cache_entry *name_regex, *queue_regex, *exchange_regex, *rkey_regex;
  
    name_regex = pcre_get_compiled_regex_cache(TASK_ANNOTATION_NAME_REGEX);
    queue_regex = pcre_get_compiled_regex_cache(TASK_ANNOTATION_QUEUE_REGEX);
    exchange_regex = pcre_get_compiled_regex_cache(TASK_ANNOTATION_EXCHANGE_REGEX);
    rkey_regex = pcre_get_compiled_regex_cache(TASK_ANNOTATION_RKEY_REGEX);

    zval exchanges, queues, bind_queues, ht;
    array_init(&exchanges);
	array_init(&queues);
	array_init(&bind_queues);

	if(ce->type == ZEND_USER_CLASS && ce->info.user.doc_comment) {
	    zend_string *doc = ce->info.user.doc_comment;
		char *str = ZSTR_VAL(doc);
		size_t len = ZSTR_LEN(doc);
		zend_string_release(doc);

		zval matched, parts;
		ZVAL_NULL(&parts);
		ZVAL_NULL(&matched);
		php_pcre_match_impl(queue_regex, str, len, &matched, &parts, 0, 0, 0, 0);
		zval *q, *e;
		if(1 == Z_LVAL(matched)) {
			q = zend_hash_find(Z_ARRVAL_P(&parts), zend_string_init(ZEND_STRL("queue"), 0));
			add_next_index_string(&queues, Z_STRVAL_P(q));
			ZVAL_NULL(&parts);
		}
			
		php_pcre_match_impl(exchange_regex, str, len, &matched, &parts, 0, 0, 0, 0);
		if(1 == Z_LVAL(matched)) {
			e = zend_hash_find(Z_ARRVAL_P(&parts), zend_string_init(ZEND_STRL("exchange"), 0));
			add_next_index_string(&exchanges, Z_STRVAL_P(e));
			ZVAL_NULL(&parts);
		}
			
		php_pcre_match_impl(rkey_regex, str, len, &matched, &parts, 0, 0, 0, 0);
   		if(1 == Z_LVAL(matched)) {
			zval *r = zend_hash_find(Z_ARRVAL_P(&parts), zend_string_init(ZEND_STRL("routing_key"), 0));
			zval bind_queue;
			array_init(&bind_queue);
			zend_hash_add(Z_ARRVAL(bind_queue), zend_string_init(ZEND_STRL("exchange"), 0), e);
			zend_hash_add(Z_ARRVAL(bind_queue), zend_string_init(ZEND_STRL("queue"), 0), q);
			zend_hash_add(Z_ARRVAL(bind_queue), zend_string_init(ZEND_STRL("routing_key"), 0), r);
			add_next_index_zval(&bind_queues, &bind_queue);
		}
	}

    zend_hash_add(Z_ARRVAL(annoations), zend_string_init(ZEND_STRL("queues"), 0), &queues);
	zend_hash_add(Z_ARRVAL(annoations), zend_string_init(ZEND_STRL("exchanges"), 0), &exchanges);
	zend_hash_add(Z_ARRVAL(annoations), zend_string_init(ZEND_STRL("bind"), 0), &bind_queues);
}


void execute(zval message) {

    zval *z = zend_hash_find(Z_ARRVAL(message), zend_string_init(TASK_MESSAGE_CLS));
    char *s = Z_STRVAL_P(z);

    zval *zid = zend_hash_find(Z_ARRVAL(message), zend_string_init(TASK_MESSAGE_ID));
    char *id = Z_STRVAL_P(zid);
    
    zval *args = zend_hash_find(Z_ARRVAL(message), zend_string_init(TASK_MESSAGE_ARGS));

    zend_string *pcls_name = zend_string_init("Haystack\\Task\\Parameter", strlen("Haystack\\Task\\Parameter"), 0);
    zend_class_entry *pcls = zend_lookup_class(pcls_name);
    if (pcls == NULL) {
        return;
    }

    zval parameter_obj;
    int k = object_init_ex(&parameter_obj, pcls);

    add_property_zval(&parameter_obj,"values", args);

    zend_class_entry *cls;
    zend_string *cls_name = zend_string_init(s, strlen(s), 0);
    cls = zend_lookup_class(cls_name);
    if (cls == NULL) {
        return;
    }

  zend_string *method = zend_string_init("execute", 7, 0);

  zval o;
  zval ret;
  int r = object_init_ex(&o, cls);

  zend_call_method(&o, cls, NULL, ZSTR_VAL(method), ZSTR_LEN(method), NULL, 1, &parameter_obj, NULL);
}


int jsonify(char *data, zval *result) {

    zval function, json_string, assoc;
    zval args[2];
    
    ZVAL_STRING(&function, "json_decode");

    ZVAL_STRING(&args[0], data);
    ZVAL_TRUE(&args[1]);
    zval ret;
    zend_fcall_info fci;
    fci.size = sizeof(zend_fcall_info);
    fci.function_name = function;
    fci.retval = result;
    fci.params = args;
    fci.object = NULL;
    fci.no_separation = 1;
    fci.param_count = 2;
    if (zend_call_function(&fci, NULL) == FAILURE) {
        return -1;
    }
    return 0;
}

