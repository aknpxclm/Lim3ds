#ifndef SINNER_ENEMY_DEFIN_H_
#define SINNER_ENEMY_DEFIN_H_

typedef struct {
double Health;
double OldHealth;
int coins;
int Skillbase;
int SkillcoinPow;
int Sanity;
}Characters;

typedef struct {
int coins;
int Skillbase;
int SkillcoinPow;
}SkillInfo;

typedef struct Clashing_Checks{
int SkillClashing;
int Priority;
bool IsClashing;
bool IsUnclashed; //Based on if the enemy is clashing
}ClashParams;

#endif