dnl $Id$
dnl config.m4 for extension conskit

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(conskit, for conskit support,
Make sure that the comment is aligned:
[  --with-conskit             Include conskit support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(conskit, whether to enable conskit support,
dnl Make sure that the comment is aligned:
dnl [  --enable-conskit           Enable conskit support])

if test "$PHP_CONSKIT" != "no"; then
  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR="/include/amqp.h"
  if test -r $PHP_CONSKIT/$SEARCH_FOR; then 
    AMQP_DIR=$PHP_CONSKIT
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


  dnl # --with-CONSKIT -> check for lib and symbol presence
  dnl LIBNAME=CONSKIT # you may want to change this
  dnl LIBSYMBOL=CONSKIT # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $CONSKIT_DIR/$PHP_LIBDIR, CONSKIT_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_CONSKITLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong CONSKIT lib version or lib not found])
  dnl ],[
  dnl   -L$CONSKIT_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(CONSKIT_SHARED_LIBADD)

  LIBNAME=rabbitmq
  LIBSYMBOL=rabbitmq
  PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $AMQP_DIR/$PHP_LIBDIR, AMQP_SHARED_LIBADD)

  SOURCES="conskit.c conskit_application.c conskit_parameter.c conskit_amqp_broker.c conskit_message.c conskit_signal.c conskit_event.c conskit_worker.c conskit_context.c"

  dnl PHP_SUBST(AMQP_SHARED_LIBADD)

  PHP_NEW_EXTENSION(conskit, $SOURCES, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
