#include <stdbool.h>
#include "Sinner_Enemy_defin.h"

#ifndef SLOTSELECT_H_
#define SLOTSELECT_H_

void determineClashAtkType(u8 AtkOrder[][2], u8 EnSklOrder[][2], Characters *Sinners);
u8 beginSinSelect(u32 TOUCHx, u32 TOUCHy, u16 CurrSinTOChooseSkill, bool *SkillTargetingLocked, bool *StartSelec);
u8 cursorToEN_Skill(u32 TOUCHx, u32 TOUCHy);
void toggleDefSkill(u32 TOUCHx, u32 TOUCHy, Characters *Sinner);
bool toggleDefenceCheck(u32 TOUCHx, u32 TOUCHy);

#endif