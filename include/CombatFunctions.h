#include "Skill.h"
#include "Sinner_Enemy_defin.h"

#ifndef COMBATFUNCTIONS_H_
#define COMBATFUNCTIONS_H_

#define CurrentIndex 1

int ClashingAtk(Characters *Sinner, Characters *Enemy);
void UnopposedAtk(Characters *Attack, Characters *Oppo);
int ComparePriority(int Pri1,int Pri2);
int CreateSkillStores(int SkillOptions[][2], int EnSkillOrder[][2], int BufferSkill[], int SkillList[], int Turncount);
void SetUpBoss(SkillInfo Enskill[][4], bool BossOrMultipleEnemy);
void DefenceAgainstAtk(Characters *Sinner, SkillInfo *SinSkill, Characters *Enemy, bool clashable);

#endif
