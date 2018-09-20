#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/json/php_json.h"


#include "conskit_application.h"
#include "conskit_message.h"
#include "conskit_amqp_broker.h"
#include "conskit_context.h"


zend_class_entry *conskit_application_ce;


ZEND_BEGIN_ARG_INFO_EX(conskit_application_register_tasks_directory_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, "directory")
    ZEND_ARG_INFO(0, "prefix")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(conskit_application_void_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(conskit_application, __construct) {
    zval dirs;
    array_init(&dirs);
    zend_update_property(
        conskit_application_ce,
        getThis(),
        ZEND_STRL("dirs"),
        &dirs
    );
}


PHP_METHOD(conskit_application, getRegisteredDirs) {
    zval *dirs = zend_read_property(
        conskit_application_ce, getThis(), 
        ZEND_STRL("dirs"), 1, NULL);
    RETURN_ZVAL(dirs, 1, 0);
}


PHP_METHOD(conskit_application, registerTasksDirectory) {
    zend_string *path;
    zval *prefix;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(path)
        Z_PARAM_ZVAL(prefix)
    ZEND_PARSE_PARAMETERS_END();

    zval *dirs = zend_read_property(
        conskit_application_ce, getThis(), ZEND_STRL("dirs"), 1, NULL
    );
    zend_hash_add(
        Z_ARRVAL_P(dirs), path, prefix
    );

    RETURN_ZVAL(getThis(), 0, 0);
}

/*
void consume_message() {
    init_child_signals();
    amqp_connection_state_t connection = make_rb_connection("localhost", 5672);
    rb_auth(connection, "guest", "guest", "/");
    open_rb_channel(connection, 1);
    rb_basic_consume(connection, 1, "demo", "demo");
    rb_consume(connection, handle_message, 5);
}*/

void worker() {
    php_printf("Worker %d started\n", getpid());
    amqp_connection_state_t connection = make_rb_connection("localhost", 5672);
    rb_auth(connection, "guest", "guest", "/");
    open_rb_channel(connection, 1);
    rb_basic_consume(connection, 1, "demo", "demo");
    rb_consume(connection, handle_message, 5000);
}

PHP_METHOD(conskit_application, start) {

    init_context();
    int n=0;
    pid_t pid;
    for(; n<8; n++) {
        pid = fork();
        if(pid == 0) {
            goto run_worker;
        }
    }

    main_loop(geralt_context.event_loop, NULL);

run_worker:
    worker();
};


static zend_function_entry conskit_application_methods[] = {
    PHP_ME(conskit_application, __construct, conskit_application_void_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(conskit_application, registerTasksDirectory, conskit_application_register_tasks_directory_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(conskit_application, getRegisteredDirs, conskit_application_void_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(conskit_application, start, conskit_application_void_arginfo, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};


PHP_MINIT_FUNCTION(conskit_application) {

    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Conskit\\Application", conskit_application_methods);
    conskit_application_ce = zend_register_internal_class(&ce TSRMLS_CC);
       
    return SUCCESS;
}