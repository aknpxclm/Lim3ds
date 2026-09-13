#include <3ds.h>
#include <stdbool.h>

#ifndef SINNER_ENEMY_DEFIN_H_
#define SINNER_ENEMY_DEFIN_H_

typedef struct {
double Health;
double OldHealth;
int coins;
int Skillbase;
int SkillcoinPow;
int Sanity;
int Char_ID; //id of the identity of sinner or enemy identifier
}Characters;

typedef struct Skill{
int coins;
int Skillbase;
int SkillcoinPow;
}SkillInfo;

typedef struct Clashing_Checks{
u8 SkillClashing;  //Skill slot that is going to be clashed
u8 Priority;       //higher priority means skill will clash over other skills
bool SlotAppeared; //Shows if a skill is already clashing a slot
bool IsClashing;   //sinner is clashing a skill
bool IsUnclashed;  //Based on if the enemy is clashing the current sinner's skill
}ClashParams;

enum SpriteTarget
{
    Ally = 0,
    Opponent
};

#endif