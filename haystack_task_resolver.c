#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#include "php.h"
#include "zend_interfaces.h"

#include "haystack_task_resolver.h"


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
