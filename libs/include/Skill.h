#ifndef SKILL_H_
#define SKILL_H_

int ClashValue(int coins, int Skillbase, int SkillcoinPow, int Sanity);
int Damagedealt(int coins, int Skillbase, int SkillcoinPow, int Clashes);
int LimitSanity(int* Sanity);
void SeedStart();
int Form_or_Select_Random_Skill();
void Rearrange_SkillPool(int SkillList[6]);

#endif
