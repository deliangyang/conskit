#ifndef PHP_HAYSTACK_H
#define PHP_HAYSTACK_H

extern zend_module_entry haystack_module_entry;
#define phpext_haystack_ptr &haystack_module_entry

#define PHP_HAYSTACK_VERSION "0.1.0"

#ifdef PHP_WIN32
#	define PHP_HAYSTACK_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_HAYSTACK_API __attribute__ ((visibility("default")))
#else
#	define PHP_HAYSTACK_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include "amqp.h"

/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:

ZEND_BEGIN_MODULE_GLOBALS(haystack)
	zend_long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(haystack)
*/

/* Always refer to the globals in your function as HAYSTACK_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define HAYSTACK_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(haystack, v)

#if defined(ZTS) && defined(COMPILE_DL_HAYSTACK)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* PHP_HAYSTACK_H */