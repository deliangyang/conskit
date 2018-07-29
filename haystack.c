
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_haystack.h"


#include "haystack_task_parameter.h"
#include "haystack_task.h"
#include "haystack_application.h"
#include "haystack_task_resolver.h"

#include "amqp_rb.h"


/* If you declare any globals in php_haystack.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(haystack)
*/

/* True global resources - no need for thread safety here */
static int le_haystack;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("haystack.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_haystack_globals, haystack_globals)
    STD_PHP_INI_ENTRY("haystack.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_haystack_globals, haystack_globals)
PHP_INI_END()
*/
/* }}} */



/* }}} */

/* {{{ php_haystack_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_haystack_init_globals(zend_haystack_globals *haystack_globals)
{
	haystack_globals->global_value = 0;
	haystack_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(haystack)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	ZEND_MODULE_STARTUP_N(haystack_task_parameter)(INIT_FUNC_ARGS_PASSTHRU);
	ZEND_MODULE_STARTUP_N(haystack_task)(INIT_FUNC_ARGS_PASSTHRU);
	ZEND_MODULE_STARTUP_N(haystack_application)(INIT_FUNC_ARGS_PASSTHRU);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(haystack)
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
PHP_RINIT_FUNCTION(haystack)
{
#if defined(COMPILE_DL_HAYSTACK) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(haystack)
{
	return SUCCESS;
}

int handle(amqp_envelope_t *envelop) {
	int ret;
	zval val;
	char *s = bytes_as_string(envelop->message.body);
	printf("message: %s\n", s);
	ret = jsonify(s, &val);
	execute(&val);
}


PHP_FUNCTION(inspect_arg) {

	zval *props;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(props)
	ZEND_PARSE_PARAMETERS_END();

	zval *host;
	zval *port;
	zval *username;
	zval *password;
	
	host = zend_hash_find(
		Z_ARRVAL_P(props),
		zend_string_init(ZEND_STRL("host"), 0)
	);

	port = zend_hash_find(
		Z_ARRVAL_P(props),
		zend_string_init(ZEND_STRL("port"), 0)
	);

	username = zend_hash_find(
		Z_ARRVAL_P(props),
		zend_string_init(ZEND_STRL("username"), 0)
	);

	password = zend_hash_find(
		Z_ARRVAL_P(props),
		zend_string_init(ZEND_STRL("password"), 0)
	);
	
	char *s_host = DEFAULT_AMQP_HOST,
	     *s_username = DEFAULT_AMQP_USERNAME, 
		 *s_password = DEFAULT_AMQP_PASSWORD;

	zend_long i_port = DEFAULT_AMQP_PORT;

	if (port != NULL) {
		i_port = Z_LVAL_P(port);
	}

	if (host != NULL) {
		s_host = Z_STRVAL_P(host);
	}

	if (username != NULL) {
		s_username = Z_STRVAL_P(username);
	}

	if (password != NULL) {
		s_password = Z_STRVAL_P(password);
	}

	amqp_connection_state_t c = make_rb_connection(s_host, i_port);	
	rb_auth(c, s_username, s_password, "/");
	amqp_channel_open_ok_t *ok_channel = open_rb_channel(c, 1);
	zval *queues;
	queues = zend_hash_find(Z_ARRVAL_P(props), zend_string_init(ZEND_STRL("queues"), 0));

	zval *queue;
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(queues), queue) {
		rb_basic_consume(c, 1, Z_STRVAL_P(queue), Z_STRVAL_P(queue));
	}
	ZEND_HASH_FOREACH_END();
	rb_consume(c, handle, 1000000);
}

/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(haystack)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "haystack support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ haystack_functions[]
 *
 * Every user visible function must have an entry in haystack_functions[].
 */
const zend_function_entry haystack_functions[] = {
	PHP_FE(inspect_arg, NULL)
	PHP_FE_END	/* Must be the last line in haystack_functions[] */
};
/* }}} */

/* {{{ haystack_module_entry
 */
zend_module_entry haystack_module_entry = {
	STANDARD_MODULE_HEADER,
	"haystack",
	haystack_functions,
	PHP_MINIT(haystack),
	PHP_MSHUTDOWN(haystack),
	PHP_RINIT(haystack),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(haystack),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(haystack),
	PHP_HAYSTACK_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_HAYSTACK
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(haystack)
#endif