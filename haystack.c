
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_haystack.h"

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


PHP_FUNCTION(confirm_haystack_compiled)
{
    php_printf("default size: %d\n", AMQP_DEFAULT_FRAME_SIZE);
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "haystack", arg);

	RETURN_STR(strg);
}
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
	PHP_FE(confirm_haystack_compiled,	NULL)		/* For testing, remove later. */
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