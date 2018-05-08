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

// @todo
ZEND_BEGIN_ARG_INFO_EX(haystack_task_parameter_factory_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, args)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(haystack_task_parameter_set_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, name)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

PHP_METHOD(haystack_task_parameter, __construct) {}
PHP_METHOD(haystack_task_parameter, get) {}
PHP_METHOD(haystack_task_parameter, set) {}
PHP_METHOD(haystack_task_parameter, toArray) {}

static zend_function_entry haystack_task_parameter_methods[] = {
    PHP_ME(haystack_task_parameter, toArray, haystack_task_parameter_void_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(haystack_task_parameter, get, haystack_task_parameter_void_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(haystack_task_parameter, set, haystack_task_parameter_set_arginfo, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};


PHP_MINIT_FUNCTION(haystack_task_parameter) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Haystack\\Task\\Parameter", haystack_task_parameter_methods);
    haystack_task_parameter_ce = zend_register_internal_class(&ce TSRMLS_CC);
    return SUCCESS;
}