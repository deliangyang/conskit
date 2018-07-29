#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

#include "haystack_task_parameter.h"


zend_class_entry *haystack_task_parameter_ce;


ZEND_BEGIN_ARG_INFO_EX(haystack_task_parameter_void_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(haystack_task_parameter_get_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(haystack_task_parameter_construct_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, array)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(haystack_task_parameter_set_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, name)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()


PHP_METHOD(haystack_task_parameter, __construct) {
    zval *hash;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &hash) == FAILURE) {
        RETURN_FALSE;
    }
    zend_update_property(
        haystack_task_parameter_ce, 
        getThis(),
        ZEND_STRL(HAYSTACK_TASK_PARAMETER_PROPERTY_VALUES), 
        hash
    );
}


PHP_METHOD(haystack_task_parameter, get) {
    zend_string *name;
    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &name) == FAILURE) {
        RETURN_NULL();
    }
    zval *vals;
    zval *ret;
    vals = zend_read_property(haystack_task_parameter_ce, getThis(), ZEND_STRL(HAYSTACK_TASK_PARAMETER_PROPERTY_VALUES), 1, NULL);
    if(EXPECTED(vals && Z_TYPE_P(vals) == IS_ARRAY)) {
        if ((ret = zend_hash_find(Z_ARRVAL_P(vals), name)) != NULL) {
                RETURN_ZVAL(ret, 1, 0);
        }
    }
    RETURN_NULL();
}


PHP_METHOD(haystack_task_parameter, set) {
    zend_string *name;
    zval *value;

    if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Sz", &name, &value) == FAILURE) {
        RETURN_NULL();
    }
    zval *vals;
    vals = zend_read_property(haystack_task_parameter_ce, getThis(), ZEND_STRL(
        HAYSTACK_TASK_PARAMETER_PROPERTY_VALUES
    ), 1, NULL);
    
    if(EXPECTED(vals && Z_TYPE_P(vals) == IS_ARRAY)) {
        if(NULL != zend_hash_update(Z_ARRVAL_P(vals), name, value)) {
            Z_TRY_ADDREF_P(value);
            RETURN_TRUE;
        }
    }
    RETURN_FALSE;
}


PHP_METHOD(haystack_task_parameter, toArray) {
    zval *values = zend_read_property(haystack_task_parameter_ce, getThis(),
        ZEND_STRL(HAYSTACK_TASK_PARAMETER_PROPERTY_VALUES), 1, NULL
        );
    RETURN_ZVAL(values, 1, 0);
}


static zend_function_entry haystack_task_parameter_methods[] = {
    PHP_ME(haystack_task_parameter, __construct, haystack_task_parameter_construct_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(haystack_task_parameter, toArray, haystack_task_parameter_void_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(haystack_task_parameter, get, haystack_task_parameter_void_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(haystack_task_parameter, set, haystack_task_parameter_set_arginfo, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};


PHP_MINIT_FUNCTION(haystack_task_parameter) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Haystack\\Task\\Parameter", haystack_task_parameter_methods);
    haystack_task_parameter_ce = zend_register_internal_class(&ce TSRMLS_CC);

    zend_declare_property_null(
        haystack_task_parameter_ce, 
        ZEND_STRL(HAYSTACK_TASK_PARAMETER_PROPERTY_VALUES), ZEND_ACC_PUBLIC
    );
    return SUCCESS;
}