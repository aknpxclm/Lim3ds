#include <3ds.h>
#include <stdbool.h>

#ifndef SINNER_ENEMY_DEFIN_H_
#define SINNER_ENEMY_DEFIN_H_

enum Menu{ StartMen = 0, MainMen, CombatMen };

enum Rendering{ GFX = 0, Combat, CombatGFX };

enum Msprites{ Loading = 0, LobbyTop, LobbyBot, BattleStageBack};

typedef struct {
float Health;
u8 coins;
u8 Skillbase;
u8 SkillcoinPow;
u8 Sanity;
u32 Char_ID; //id of the identity of sinner or enemy identifier
}Characters;

typedef struct Skill{
u8 coins;
u8 Skillbase;
u8 SkillcoinPow;
}SkillInfo;

typedef struct Clashing_Checks{
u8 SkillClashing;  //Skill slot that is going to be clashed
u8 Priority;       //higher priority means skill will clash over other skills
bool SlotAppeared; //Shows if a skill is already clashing a slot
bool IsClashing;   //sinner is clashing a skill
bool IsUnclashed;  //Based on if the enemy is clashing the current sinner's skill
bool UseDefence;
}ClashParams;

enum SpriteTarget
{
    Ally = 0,
    Opponent
};

enum DefenceSkillType
{
    Guard = 0,
    ClashGuard,
    Evade,
    Counter
    //ClashCounter
};

#endif