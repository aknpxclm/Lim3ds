#include "errcheck.h"
#include <stdint.h>

int checkinit(Logger *mainlog, int n)
{
    uint8_t result = (mainlog->initErr >> n) & 1;
    if(!result)
    {
        switch(n)
        {
            case 1:
            return 1;
            break;
        }
    }
    return 0;
}