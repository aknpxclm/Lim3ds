#ifndef USER_DENY__H_
#define USER_DENY__H_

#define DENYREASONS(num) 1U << num

enum TurnDenyReasons
{
    UnInited,
    SlotUnused
};

#endif