#include "Sinner_Enemy_defin.h"

#ifndef SKILL_H_
#define SKILL_H_

u32 clashValue(u8 base, u8 coins, u8 coinPow, u8 sanity);
u32 damagedealt(Characters *Charac, u16 Clashes);
u32 guardDmgDealt(Characters *Charac, Characters *AttackingCharac);
u8 evadeDmg(Characters *EvadeCharac, Characters *AttackingCharac);
u8 limitSanity(u8* Sanity);
void seedStart();
int form_or_Select_Random_Skill();
void rearrange_SkillPool(u8 SkillList[]);
void shiftSkillSelects(u8 SkillOptions[][2], u8 BufferSkill[], u8 SkillList[]);
u8 checkAllSlotsAreUsed(u8 AttackOrder[][2]);
void loadSkill(Characters *charac, u8 skillRank);
void loadDefenceSkill(Characters *charac);

#endif
