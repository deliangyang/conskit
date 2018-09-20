#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "php.h"

#include "conskit_amqp.h"

int handle_message(amqp_envelope_t *envelope);
void execute(zval *message);