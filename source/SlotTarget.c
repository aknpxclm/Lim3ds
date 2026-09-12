#include <stdbool.h>
#include "Sinner_Enemy_defin.h"
#include "CombatFunctions.h"
#include "SlotTarget.h"

void DetermineClashAtkType(int AtkOrder[][2], int EnSklOrder[][2], int SkillPriLvl[], bool SelectSlotAppeared[], ClashParams SkillPosInfo[])
{
    for(int Search = 0; Search < 5; Search++){
        //check if clashing
        if(AtkOrder[Search][1] == EnSklOrder[Search][1])
        {
            SkillPosInfo[Search].IsClashing = true;
            SkillPosInfo[Search].SkillClashing = Search;
            SelectSlotAppeared[AtkOrder[Search][1]] = true; // skill is targeting a slot
            SkillPriLvl[AtkOrder[Search][1]] = AtkOrder[Search][1]; //record what skill slot was targeted
        }
        //check if skill is going unopposed while another skill clashes the same slot
        if(SelectSlotAppeared[AtkOrder[Search][1]] == true)
        {
            SkillPosInfo[Search].IsClashing = ComparePriority(SkillPriLvl[Search], SkillPriLvl[AtkOrder[Search][1]]);
            //Check if other skill has the higher pirority and remove them from clashing if it is lower
            if(SkillPosInfo[Search].IsClashing)
            {
                SkillPosInfo[AtkOrder[Search][1]].IsClashing = false;
            }
            // reset check bool
            SelectSlotAppeared[AtkOrder[Search][1]] = false;
        }
        //check if enemy attacks wil go unopposed, no sinner is clashing the slot
        if(EnSklOrder[Search][1] != AtkOrder[0][1] || \
            EnSklOrder[Search][1] != AtkOrder[1][1] || \
            EnSklOrder[Search][1] != AtkOrder[2][1] || \
            EnSklOrder[Search][1] != AtkOrder[3][1] || \
            EnSklOrder[Search][1] != AtkOrder[4][1])
            {
                SkillPosInfo[Search].IsUnclashed = true;
            }
    }
}