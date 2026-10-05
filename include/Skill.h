#include "Sinner_Enemy_defin.h"

#ifndef SKILL_H_
#define SKILL_H_

int ClashValue(int base, int coins, int coinPow, int sanity);
int Damagedealt(Characters *Charac, int Clashes);
int GuardDmgDealt(Characters *Charac, Characters *AttackingCharac);
u8 EvadeDmg(Characters *EvadeCharac, Characters *AttackingCharac);
u8 LimitSanity(u8* Sanity);
void SeedStart();
int Form_or_Select_Random_Skill();
void Rearrange_SkillPool(int SkillList[]);
void ShiftSkillSelects(int SkillOptions[][2], int BufferSkill[], int SkillList[]);

#define SkillUsedFlag 444

#endif
