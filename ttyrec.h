#ifndef __TTYREC_H__
#define __TTYREC_H__

#include "sys/time.h"

typedef struct header {
    struct timeval tv;
    int len;
} Header;


#endif
