#include <stdbool.h>
#include "Sinner_Enemy_defin.h"
#include "CombatFunctions.h"
#include "SlotTarget.h"

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