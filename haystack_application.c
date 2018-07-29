#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

#include "haystack_application.h"

zend_class_entry *haystack_application_ce;

ZEND_BEGIN_ARG_INFO_EX(haystack_application_register_tasks_directory_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, "directory")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(haystack_application_void_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(haystack_application, __construct) {}
PHP_METHOD(haystack_application, registerTasksDirectory) {}
PHP_METHOD(haystack_application, start) {}

static zend_function_entry haystack_application_methods[] = {
    {NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION(haystack_application) {

    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Haystack\\Application", haystack_application_methods);
    haystack_application_ce = zend_register_internal_class(&ce TSRMLS_CC);
    /*
    zend_declare_property_null(
        haystack_application_ce, 
        ZEND_STRL(HAYSTACK_APPLICATION_TASKS), ZEND_ACC_PROTECTED
    );
    zend_declare_property_null(
        haystack_application_ce,
        ZEND_STRL(HAYSTACK_APPLICATION_DIRECTORIES), ZEND_ACC_PROTECTED)
    );*/
    return SUCCESS;
}