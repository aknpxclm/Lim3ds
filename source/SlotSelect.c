#include <stdbool.h>
#include "Sinner_Enemy_defin.h"
#include "CombatFunctions.h"
#include "SlotSelect.h"

//allows the change of the size, spacing and position of the skill gui touch points simply
//top left of touchscreen is (0, 0)
#define LOWERxSCREEN 320
#define LOWERySCREEN 240
#define SLOTTOUCHBOXAREA 32
#define padding 10

#define LOWERxSLOTPOS(num) ( LOWERxSCREEN - ( ( SLOTTOUCHBOXAREA *  (10 - num) ) + padding ) )

enum slot
{
    SlotOne = 4,
    SlotTwo = 3,
    SlotTree = 2,
    SlotFour = 1,
    SlotFive = 0
};

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
            //Check if other skill has the higher pirority and remove them from clashing if it is lower
            if(SkillPosInfo[Slot].Priority > SkillPosInfo[AtkOrder[Slot][1]].Priority)
            {
                SkillPosInfo[AtkOrder[Slot][1]].IsClashing = false;
            }
            if(SkillPosInfo[Slot].IsClashing)
            {
                SkillPosInfo[AtkOrder[Slot][1]].IsClashing = false;
            }
            // reset check bool
            SkillPosInfo[AtkOrder[Slot][1]].SlotAppeared = false;
        }
        //check if enemy attacks wil go unopposed, no sinner is clashing the slot
        for(int i = 0; i < 5; i++)
        {
            if(EnSklOrder[Slot][1] != AtkOrder[i][1]) SkillPosInfo[Slot].IsUnclashed = true;
            else SkillPosInfo[Slot].IsUnclashed = false; //safeguard
        }
    }
}
//Creates sinner character anchor to select the slot it targets
int BeginSinSelect(int TOUCHx, int TOUCHy, int CurrSinTOChooseSkill, bool *SkillTargetingLocked, bool *StartSelec)
{
    if(*SkillTargetingLocked) return CurrSinTOChooseSkill;
        /*return current skill index that the useer is choosing to clash a skill with;
        ignoring touch pos until, touch screen is listed or skills are selected to clash*/
    if(TOUCHx <= LOWERxSLOTPOS(SlotOne) && TOUCHx >= LOWERxSLOTPOS(SlotOne) + SLOTTOUCHBOXAREA && TOUCHy <= LOWERySCREEN - (SLOTTOUCHBOXAREA + padding) && TOUCHy >= LOWERySCREEN - padding)
    {
        *SkillTargetingLocked = true;
        *StartSelec = true;
        return 0; //slot 1
    }
    else if(TOUCHx <= LOWERxSLOTPOS(SlotTwo) && TOUCHx >= LOWERxSLOTPOS(SlotTwo) + SLOTTOUCHBOXAREA && TOUCHy <= LOWERySCREEN - (SLOTTOUCHBOXAREA + padding) && TOUCHy >= LOWERySCREEN - padding)
    {
        *SkillTargetingLocked = true;
        *StartSelec = true;
        return 1; //slot 2
    }
    else if(TOUCHx <= LOWERxSLOTPOS(SlotTree) && TOUCHx >= LOWERxSLOTPOS(SlotTree) + SLOTTOUCHBOXAREA && TOUCHy <= LOWERySCREEN - (SLOTTOUCHBOXAREA + padding) && TOUCHy >= LOWERySCREEN - padding)
    {
        *SkillTargetingLocked = true;
        *StartSelec = true;
        return 2; //slot 3
    }
    else if(TOUCHx <= LOWERxSLOTPOS(SlotFour) && TOUCHx >= LOWERxSLOTPOS(SlotFour) + SLOTTOUCHBOXAREA && TOUCHy <= LOWERySCREEN - (SLOTTOUCHBOXAREA + padding) && TOUCHy >= LOWERySCREEN - padding)
    {
        *SkillTargetingLocked = true;
        *StartSelec = true;
        return 3; //slot 4
    }
    else if(TOUCHx <= LOWERxSLOTPOS(SlotFive) && TOUCHx >= LOWERxSLOTPOS(SlotFive) + SLOTTOUCHBOXAREA && TOUCHy <= LOWERySCREEN - (SLOTTOUCHBOXAREA + padding) && TOUCHy >= LOWERySCREEN - padding)
    {
        *SkillTargetingLocked = true;
        *StartSelec = true;
        return 4; //slot 5
    }
    return 9; // "NOTSELECTED"
}

int CursorToEN_Skill(int TOUCHx, int TOUCHy)
{
    // x & y are assuming that the hidtouch function are based on pixel coordinates
    if(TOUCHx <= LOWERxSLOTPOS(SlotOne) && TOUCHx >= LOWERxSLOTPOS(SlotOne) + SLOTTOUCHBOXAREA && TOUCHy <= padding && TOUCHy >= SLOTTOUCHBOXAREA + padding) return 0; //slot 1
    else if(TOUCHx <= LOWERxSLOTPOS(SlotTwo) && TOUCHx >= LOWERxSLOTPOS(SlotTwo) + SLOTTOUCHBOXAREA && TOUCHy <= padding && TOUCHy >= SLOTTOUCHBOXAREA + padding) return 1; //slot 2
    else if(TOUCHx <= LOWERxSLOTPOS(SlotTree) && TOUCHx >= LOWERxSLOTPOS(SlotTree) + SLOTTOUCHBOXAREA && TOUCHy <= padding && TOUCHy >= SLOTTOUCHBOXAREA + padding) return 2; //slot 3
    else if(TOUCHx <= LOWERxSLOTPOS(SlotFour) && TOUCHx >= LOWERxSLOTPOS(SlotFour) + SLOTTOUCHBOXAREA && TOUCHy <= padding && TOUCHy >= SLOTTOUCHBOXAREA + padding) return 3; //slot 4
    else if(TOUCHx <= LOWERxSLOTPOS(SlotFive) && TOUCHx >= LOWERxSLOTPOS(SlotFive) + SLOTTOUCHBOXAREA && TOUCHy <= padding && TOUCHy >= SLOTTOUCHBOXAREA + padding) return 4; //slot 5
    else return 9; // "NOTSELECTED"
}

void ToggleDefSkill(int TOUCHx, int TOUCHy, ClashParams Sinner[]) //toggles the bool value that checks if a sinner is using a defence skill
{
    if(TOUCHx <= LOWERxSLOTPOS(SlotOne) && TOUCHx >= LOWERxSLOTPOS(SlotOne) + SLOTTOUCHBOXAREA && TOUCHy <= LOWERySCREEN - (SLOTTOUCHBOXAREA + padding) && TOUCHy >= LOWERySCREEN - padding) Sinner[0].UseDefence = !Sinner[0].UseDefence;
    else if(TOUCHx <= LOWERxSLOTPOS(SlotTwo) && TOUCHx >= LOWERxSLOTPOS(SlotTwo) + SLOTTOUCHBOXAREA  && TOUCHy <= LOWERySCREEN - (SLOTTOUCHBOXAREA + padding) && TOUCHy >= LOWERySCREEN - padding) Sinner[1].UseDefence = !Sinner[1].UseDefence;
    else if(TOUCHx <= LOWERxSLOTPOS(SlotTree) && TOUCHx >= LOWERxSLOTPOS(SlotTree) + SLOTTOUCHBOXAREA  && TOUCHy <= LOWERySCREEN - (SLOTTOUCHBOXAREA + padding) && TOUCHy >= LOWERySCREEN - padding) Sinner[2].UseDefence = !Sinner[2].UseDefence;
    else if(TOUCHx <= LOWERxSLOTPOS(SlotFour) && TOUCHx >= LOWERxSLOTPOS(SlotFour) + SLOTTOUCHBOXAREA  && TOUCHy <= LOWERySCREEN - (SLOTTOUCHBOXAREA + padding) && TOUCHy >= LOWERySCREEN - padding) Sinner[3].UseDefence = !Sinner[3].UseDefence;
    else if(TOUCHx <= LOWERxSLOTPOS(SlotFive) && TOUCHx >= LOWERxSLOTPOS(SlotFive) + SLOTTOUCHBOXAREA  && TOUCHy <= LOWERySCREEN - (SLOTTOUCHBOXAREA + padding) && TOUCHy >= LOWERySCREEN - padding) Sinner[4].UseDefence = !Sinner[4].UseDefence;
}

bool ToggleDefenceCheck(int TOUCHx, int TOUCHy)
{
    // x & y are assuming that the hidtouch function are based on pixel coordinates
    if(TOUCHx <= LOWERxSLOTPOS(SlotOne) && TOUCHx >= LOWERxSLOTPOS(SlotOne) + SLOTTOUCHBOXAREA  && TOUCHy <= LOWERySCREEN - (SLOTTOUCHBOXAREA + padding) && TOUCHy >= LOWERySCREEN - padding) return true; //slot 1
    else if(TOUCHx <= LOWERxSLOTPOS(SlotTwo) && TOUCHx >= LOWERxSLOTPOS(SlotTwo) + SLOTTOUCHBOXAREA  && TOUCHy <= LOWERySCREEN - (SLOTTOUCHBOXAREA + padding) && TOUCHy >= LOWERySCREEN - padding) return true; //slot 2
    else if(TOUCHx <= LOWERxSLOTPOS(SlotTree) && TOUCHx >= LOWERxSLOTPOS(SlotTree) + SLOTTOUCHBOXAREA  && TOUCHy <= LOWERySCREEN - (SLOTTOUCHBOXAREA + padding) && TOUCHy >= LOWERySCREEN - padding) return true; //slot 3
    else if(TOUCHx <= LOWERxSLOTPOS(SlotFour) && TOUCHx >= LOWERxSLOTPOS(SlotFour) + SLOTTOUCHBOXAREA  && TOUCHy <= LOWERySCREEN - (SLOTTOUCHBOXAREA + padding) && TOUCHy >= LOWERySCREEN - padding) return true; //slot 4
    else if(TOUCHx <= LOWERxSLOTPOS(SlotFive) && TOUCHx >= LOWERxSLOTPOS(SlotFive) + SLOTTOUCHBOXAREA  && TOUCHy <= LOWERySCREEN - (SLOTTOUCHBOXAREA + padding) && TOUCHy >= LOWERySCREEN - padding) return true; //slot 5
    else return false;
}