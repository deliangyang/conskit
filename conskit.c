/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_conskit.h"

#include "conskit_application.h"
#include "conskit_parameter.h"


/* If you declare any globals in php_conskit.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(conskit)
*/

/* True global resources - no need for thread safety here */
static int le_conskit;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("conskit.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_conskit_globals, conskit_globals)
    STD_PHP_INI_ENTRY("conskit.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_conskit_globals, conskit_globals)
PHP_INI_END()
*/
/* }}} */

/*

int handle(amqp_envelope_t *t) {
	char *s = bytes_as_string(t->message.body);
	zval z;
	int i = php_json_decode(&z, s, strlen(s), 1, 128);
	if (i != 0) {
		php_printf("Invalid message: '%s'\n", s);
		return -1;
	}

	php_printf("type: %d\n", Z_TYPE(z));
	
	zval *id = zend_hash_find(Z_ARRVAL(z), zend_string_init("id", 2, 0));
	if(id == NULL) {
		php_printf("%s\n", "error");
		return -1;
	}
	php_printf("id: %s\n", Z_STRVAL_P(id));
}

PHP_FUNCTION(test_conskit) {
	amqp_connection_state_t connection = make_rb_connection("localhost", 5672);
    rb_auth(connection, "guest", "guest", "/");
    open_rb_channel(connection, 1);
    
    rb_basic_consume(connection, 1, "demo", "demo");
    rb_consume(connection, handle, 100000000);
}
*/
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_conskit_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_conskit_init_globals(zend_conskit_globals *conskit_globals)
{
	conskit_globals->global_value = 0;
	conskit_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(conskit)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
    ZEND_MODULE_STARTUP_N(conskit_application)(INIT_FUNC_ARGS_PASSTHRU);
	ZEND_MODULE_STARTUP_N(conskit_parameter)(INIT_FUNC_ARGS_PASSTHRU);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(conskit)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(conskit)
{
#if defined(COMPILE_DL_CONSKIT) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(conskit)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(conskit)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "conskit support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ conskit_functions[]
 *
 * Every user visible function must have an entry in conskit_functions[].
 */
const zend_function_entry conskit_functions[] = {
	//PHP_FE(confirm_conskit_compiled,	NULL)		/* For testing, remove later. */
	//PHP_FE(test_conskit, NULL)
	PHP_FE_END	/* Must be the last line in conskit_functions[] */
};
/* }}} */

/* {{{ conskit_module_entry
 */
zend_module_entry conskit_module_entry = {
	STANDARD_MODULE_HEADER,
	"conskit",
	conskit_functions,
	PHP_MINIT(conskit),
	PHP_MSHUTDOWN(conskit),
	PHP_RINIT(conskit),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(conskit),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(conskit),
	PHP_CONSKIT_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_CONSKIT
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(conskit)
#endif