dnl $Id$
dnl config.m4 for extension haystack

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(haystack, for haystack support,
[  --with-haystack             Include haystack support])


dnl PHP_ARG_ENABLE(haystack, whether to enable haystack support,
dnl Make sure that the comment is aligned:
dnl [  --enable-haystack           Enable haystack support])

if test "$PHP_HAYSTACK" != "no"; then

  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR="/include/amqp.h"
  if test -r $PHP_HAYSTACK/$SEARCH_FOR; then 
    AMQP_DIR=$PHP_HAYSTACK
  else 
    AC_MSG_CHECKING([for amqp.h files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        AMQP_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi

  if test -z "$AMQP_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the AMQP distribution])
  fi

  dnl PHP_ADD_INCLUDE($AMQP_DIR/include)

  PHP_ADD_INCLUDE($AMQP_DIR/include)

  old_CFLAGS=$CFLAGS
  CFLAGS="$CFLAGS -I$AMQP_DIR/include"


  dnl # --with-haystack -> check for lib and symbol presence
  dnl LIBNAME=haystack # you may want to change this
  dnl LIBSYMBOL=haystack # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $HAYSTACK_DIR/$PHP_LIBDIR, HAYSTACK_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_HAYSTACKLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong haystack lib version or lib not found])
  dnl ],[
  dnl   -L$HAYSTACK_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(HAYSTACK_SHARED_LIBADD)

  LIBNAME=rabbitmq
  LIBSYMBOL=rabbitmq
  PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $AMQP_DIR/$PHP_LIBDIR, AMQP_SHARED_LIBADD)

  SOURCES="haystack.c haystack_task_parameter.c haystack_task.c"

  dnl PHP_SUBST(AMQP_SHARED_LIBADD)

  PHP_NEW_EXTENSION(haystack, $SOURCES, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
