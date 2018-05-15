#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

#include "haystack_task_executor.h"


zend_class_entry *haystack_task_executor_ce;

static zend_function_entry haystack_task_executor_methods[] = {
    {NULL, NULL, NULL}
};


PHP_MINIT_FUNCTION(haystack_task) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Haystack\\Task\\Executor", haystack_task_executor_methods); 
    haystack_task_ce = zend_register_internal_class(&ce TSRMLS_CC);
    return SUCCESS;
}