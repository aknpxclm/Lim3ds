#ifndef SKILL_H_
#define SKILL_H_

int ClashValue(int coins, int Skillbase, int SkillcoinPow, int Sanity);
int Damagedealt(int coins, int Skillbase, int SkillcoinPow, int Clashes);
int LimitSanity(int Sanity);
void SeedStart();
void EqualiseChecks(int Location1, int CurrentLocation1);
int Form_or_Select_Random_Skill();

#endif