#include "Sinner_Enemy_defin.h"

#ifndef SKILL_H_
#define SKILL_H_

u32 ClashValue(u8 base, u8 coins, u8 coinPow, u8 sanity);
u32 Damagedealt(Characters *Charac, u16 Clashes);
u32 GuardDmgDealt(Characters *Charac, Characters *AttackingCharac);
u8 EvadeDmg(Characters *EvadeCharac, Characters *AttackingCharac);
u8 LimitSanity(u8* Sanity);
void SeedStart();
int Form_or_Select_Random_Skill();
void Rearrange_SkillPool(u8 SkillList[]);
void ShiftSkillSelects(u8 SkillOptions[][2], u8 BufferSkill[], u8 SkillList[]);

#endif
