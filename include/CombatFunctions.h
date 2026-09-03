#ifndef COMBATFUNCTIONS_H_
#define COMBATFUNCTIONS_H_

int ClashingAtk(Characters *Sinner, Characters *Enemy);
void UnopposedAtk(int Coin, int Base, int Pow, double* OpposingHealth);
int ComparePriority(int Pri1,int Pri2);
int CreateSkillStores(int SkillOptions[][2], int EnSkillOrder[][2], int BufferSkill[], int SkillList[], int Turncount);

#define CurrentIndex 1

#endif
