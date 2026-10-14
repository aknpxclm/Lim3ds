#include <stdbool.h>
#include "Skill.h"
#include "Sinner_Enemy_defin.h"

#ifndef COMBATFUNCTIONS_H_
#define COMBATFUNCTIONS_H_

#define CurrentIndex 1

int ClashingAtk(Characters *Sinner, Characters *Enemy, float *ENhealth, u8 *ENsanity);
void UnopposedAtk(Characters *Attack, Characters *Oppo);
bool CreateSkillStores(u8 SkillOptions[][2], u8 EnSkillOrder[][2], u8 BufferSkill[], u8 SkillList[], u32 TurnCount);
void SetUpBoss(SkillInfo Enskill[][4], u8 IsBoss);
void DefenceAgainstAtk(Characters *Sinner, SkillInfo *SinSkill, Characters *Enemy, u8 *EvadeResult);

#endif
