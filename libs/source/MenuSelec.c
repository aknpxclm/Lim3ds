#include <stdbool.h>
#include "MenuSelec.h"

int BeginSinSelec(int TOUCHx, int TOUCHy, int CurrSinTOChooseSkill, bool *SkillTargetingLocked, bool *BeganSelec)
{
    if(*SkillTargetingLocked)
    {
        return CurrSinTOChooseSkill; 
        /*return current skill index that the useer is choosing to clash a skill with;
        ignoring touch pos until, touch screen is listed or skills are selected to clash*/
    }

    if(TOUCHx <= 24 && TOUCHx >= 48 && TOUCHy <= 24 && TOUCHy >= 48)
    {
        *SkillTargetingLocked = true;
        *BeganSelec = true;
        return 0; //slot 1
    }
    else if(TOUCHx <= 96 && TOUCHx >= 120 && TOUCHy <= 216 && TOUCHy >= 230)
    {
        *SkillTargetingLocked = true;
        return 1; //slot 2
    }
    else if(TOUCHx <= 168 && TOUCHx >= 192 && TOUCHy <= 216 && TOUCHy >= 230)
    {
        *SkillTargetingLocked = true;
        *BeganSelec = true;
        return 2; //slot 3
    }
    else if(TOUCHx <= 216 && TOUCHx >= 240 && TOUCHy <= 216 && TOUCHy >= 230)
    {
        *SkillTargetingLocked = true;
        *BeganSelec = true;
        return 3; //slot 4
    }
    else if(TOUCHx <= 284 && TOUCHx >= 308 && TOUCHy <= 216 && TOUCHy >= 230)
    {
        *SkillTargetingLocked = true;
        *BeganSelec = true;
        return 4; //slot 5
    }
    else
    {
        *BeganSelec = false;
        return NOTSELECTED;
    }
}

int CursorToEN_Skill(int TOUCHx, int TOUCHy)
{
    // x & y are assuming that the hidtouch function are based on pixel coordinates
    if(TOUCHx <= 24 && TOUCHx >= 48 && TOUCHy <= 24 && TOUCHy >= 48)
    {
        return 0; //slot 1
    }
    else if(TOUCHx <= 96 && TOUCHx >= 120 && TOUCHy <= 24 && TOUCHy >= 48)
    {
        return 1; //slot 2
    }
    else if(TOUCHx <= 168 && TOUCHx >= 192 && TOUCHy <= 24 && TOUCHy >= 48)
    {
        return 2; //slot 3
    }
    else if(TOUCHx <= 216 && TOUCHx >= 240 && TOUCHy <= 24 && TOUCHy >= 48)
    {
        return 3; //slot 4
    }
    else if(TOUCHx <= 284 && TOUCHx >= 308 && TOUCHy <= 24 && TOUCHy >= 48)
    {
        return 4; //slot 5
    }
    else{
        return NOTSELECTED; //not selecting
    }
}
    

/* SkillTouchPos UIPostion[5] = {FirstSkill, SecondSkill, ThirdSkill, FourthSkill, FifthSkill};  X & Y areas for touch selecting skills on the bottom screen
SkillTouchPos EnUIPostion[5] = {FirstSkill, SecondSkill, ThirdSkill, FourthSkill, FifthSkill}; */