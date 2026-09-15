#include <stdbool.h>
#include "Sinner_Enemy_defin.h"
#include "CombatFunctions.h"
#include "SlotSelect.h"

//Sets variables in a struct depending on what type of attack will happen
void DetermineClashAtkType(int AtkOrder[][2], int EnSklOrder[][2], ClashParams SkillPosInfo[])
{
    for(int Slot = 0; Slot < 5; Slot++)
    {
        if(AtkOrder[Slot][1] == EnSklOrder[Slot][1]) //if the slots are targeting each other they clash
        {
            SkillPosInfo[Slot].SlotAppeared = true; // skill is targeting a slot
            SkillPosInfo[Slot].IsClashing = true;
            SkillPosInfo[Slot].SkillClashing = AtkOrder[Slot][1]; //record what skill slot was targeted
        }
        //check if skill is going unopposed while another skill clashes the same slot
        if(SkillPosInfo[AtkOrder[Slot][1]].SlotAppeared == true && Slot != 0)
        {
            SkillPosInfo[Slot].IsClashing = ComparePriority(SkillPosInfo[Slot].Priority, SkillPosInfo[AtkOrder[Slot][1]].Priority);
            //Check if other skill has the higher pirority and remove them from clashing if it is lower
            if(SkillPosInfo[Slot].IsClashing)
            {
                SkillPosInfo[AtkOrder[Slot][1]].IsClashing = false;
            }
            // reset check bool
            SkillPosInfo[AtkOrder[Slot][1]].SlotAppeared = false;
        }
        //check if enemy attacks wil go unopposed, no sinner is clashing the slot
        if(EnSklOrder[Slot][1] != AtkOrder[0][1] || EnSklOrder[Slot][1] != AtkOrder[1][1] || \
            EnSklOrder[Slot][1] != AtkOrder[2][1] || EnSklOrder[Slot][1] != AtkOrder[3][1] || \
            EnSklOrder[Slot][1] != AtkOrder[4][1])
            
            SkillPosInfo[Slot].IsUnclashed = true;
    }
}
//Creates sinner character anchor to select the slot it targets
int BeginSinSelect(int TOUCHx, int TOUCHy, int CurrSinTOChooseSkill, bool *SkillTargetingLocked, bool *StartSelec)
{
    if(*SkillTargetingLocked) return CurrSinTOChooseSkill;
        /*return current skill index that the useer is choosing to clash a skill with;
        ignoring touch pos until, touch screen is listed or skills are selected to clash*/
    if(TOUCHx <= 24 && TOUCHx >= 48 && TOUCHy <= 24 && TOUCHy >= 48)
    {
        *SkillTargetingLocked = true;
        *StartSelec = true;
        return 0; //slot 1
    }
    else if(TOUCHx <= 96 && TOUCHx >= 120 && TOUCHy <= 216 && TOUCHy >= 230)
    {
        *SkillTargetingLocked = true;
        *StartSelec = true;
        return 1; //slot 2
    }
    else if(TOUCHx <= 168 && TOUCHx >= 192 && TOUCHy <= 216 && TOUCHy >= 230)
    {
        *SkillTargetingLocked = true;
        *StartSelec = true;
        return 2; //slot 3
    }
    else if(TOUCHx <= 216 && TOUCHx >= 240 && TOUCHy <= 216 && TOUCHy >= 230)
    {
        *SkillTargetingLocked = true;
        *StartSelec = true;
        return 3; //slot 4
    }
    else if(TOUCHx <= 284 && TOUCHx >= 308 && TOUCHy <= 216 && TOUCHy >= 230)
    {
        *SkillTargetingLocked = true;
        *StartSelec = true;
        return 4; //slot 5
    }
    *StartSelec = false;
    return 9; // "NOTSELECTED"
}

int CursorToEN_Skill(int TOUCHx, int TOUCHy)
{
    // x & y are assuming that the hidtouch function are based on pixel coordinates
    if(TOUCHx <= 24 && TOUCHx >= 48 && TOUCHy <= 24 && TOUCHy >= 48) return 0; //slot 1
    else if(TOUCHx <= 96 && TOUCHx >= 120 && TOUCHy <= 24 && TOUCHy >= 48) return 1; //slot 2
    else if(TOUCHx <= 168 && TOUCHx >= 192 && TOUCHy <= 24 && TOUCHy >= 48) return 2; //slot 3
    else if(TOUCHx <= 216 && TOUCHx >= 240 && TOUCHy <= 24 && TOUCHy >= 48) return 3; //slot 4
    else if(TOUCHx <= 284 && TOUCHx >= 308 && TOUCHy <= 24 && TOUCHy >= 48) return 4; //slot 5
    else return 9; // "NOTSELECTED"
}