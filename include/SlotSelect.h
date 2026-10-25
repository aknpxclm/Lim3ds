#include <stdbool.h>
#include "Sinner_Enemy_defin.h"

#ifndef SLOTSELECT_H_
#define SLOTSELECT_H_

void DetermineClashAtkType(u8 AtkOrder[][2], u8 EnSklOrder[][2], ClashParams SkillPosInfo[]);
u8 BeginSinSelect(u32 TOUCHx, u32 TOUCHy, u16 CurrSinTOChooseSkill, bool *SkillTargetingLocked, bool *StartSelec);
u8 CursorToEN_Skill(u32 TOUCHx, u32 TOUCHy);
void ToggleDefSkill(u32 TOUCHx, u32 TOUCHy, ClashParams Sinner[]);
bool ToggleDefenceCheck(u32 TOUCHx, u32 TOUCHy);

#endif