#include "Sinner_Enemy_defin.h"

#ifndef SLOTSELECT_H_
#define SLOTSELECT_H_

void DetermineClashAtkType(int AtkOrder[][2], int EnSklOrder[][2], ClashParams SkillPosInfo[]);
int BeginSinSelect(int TOUCHx, int TOUCHy, int CurrSinTOChooseSkill, bool *SkillTargetingLocked, bool *StartSelec);
int CursorToEN_Skill(int TOUCHx, int TOUCHy);

#endif