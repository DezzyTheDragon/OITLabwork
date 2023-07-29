#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

//*************************************************
void Fatal_Error(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);

    vfprintf(stderr, msg, args);

    va_end(args);

    exit(-1);
}

#ifdef __cplusplus
}
#endif
