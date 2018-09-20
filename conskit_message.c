#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/json/php_json.h"

#include "conskit_message.h"
#include "amqp_broker.h"

int handle_message(amqp_envelope_t *envelope) {
    zval z;
    int r;
    char *s;
    s = bytes_as_string(envelope->message.body);
    r = php_json_decode(&z, s, strlen(s), 1, 128);
    if (r == 0) {
        printf("Worker %d ", getpid());
        execute(&z);
        printf("\n");
    } else {
        php_printf("Invalid json '%s'\n", s);
    }
    free(s);
}

void execute(zval *message) {

    zval *z = zend_hash_find(Z_ARRVAL_P(message), zend_string_init("cls", 3, 0));
    char *s = Z_STRVAL_P(z);

    zval *zid = zend_hash_find(Z_ARRVAL_P(message), zend_string_init("id", 2, 0));
    char *id = Z_STRVAL_P(zid);
    
    zval *args = zend_hash_find(Z_ARRVAL_P(message), zend_string_init("args", 4, 0));
       
    zend_string *pcls_name = zend_string_init("Conskit\\Parameter", strlen("Conskit\\Parameter"), 0);
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
    
    zend_string *method = zend_string_init("execute", strlen("execute"), 0);

    zval o;
    zval ret;
    
    int r = object_init_ex(&o, cls);
    zend_call_method(&o, cls, NULL, ZSTR_VAL(method), ZSTR_LEN(method), &ret, 1, &parameter_obj, NULL);
}