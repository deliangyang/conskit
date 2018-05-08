#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

#include "haystack_task.h"

zend_class_entry *haystack_task_ce;

ZEND_BEGIN_ARG_INFO_EX(haystack_task_void_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(haystack_task_construct_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, parameter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(haystack_task_execute_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, parameter)
ZEND_END_ARG_INFO()



PHP_METHOD(haystack_task, __clone) {}
PHP_METHOD(haystack_task, __construct) {}
PHP_METHOD(haystack_task, execute) {}
PHP_METHOD(haystack_task, getName) {}

static zend_function_entry haystack_task_methods[] = {
    PHP_ABSTRACT_ME(haystack_task, execute, haystack_task_execute_arginfo)
    PHP_ME(haystack_task, getName, haystack_task_void_arginfo, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};


PHP_MINIT_FUNCTION(haystack_task) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Haystack\\Task", haystack_task_methods); 
    haystack_task_ce = zend_register_internal_class(&ce TSRMLS_CC);
    haystack_task_ce->ce_flags |= ZEND_ACC_IMPLICIT_ABSTRACT_CLASS;   
    return SUCCESS;
}