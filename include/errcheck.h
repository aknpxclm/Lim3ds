#include <stdint.h>

#ifndef ERRCHECK_H_
#define ERRCHECK_H_

typedef struct 
{
    uint8_t initErr;
    uint8_t logErr;
    char *printErr;
}Logger;

#endif