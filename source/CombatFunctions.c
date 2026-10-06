#include <stdbool.h>
#include "Skill.h"
#include "Sinner_Enemy_defin.h"
#include "CombatFunctions.h"

#define ENSANITYLOSS 5
#define SINSANITYLOSS 3
#define MAX_CLASH 50

//If a skill is going to clash the enemy's, use this function to clash and deal damage
int ClashingAtk(Characters *Sinner, Characters *Enemy)
{
    int SinClashNum = 0;
    int EnClashNum = 0;
    int Clashes = 0;
    while(Sinner->coins > 0 && Enemy->coins > 0){
        // get clash values for enemy and sinner
        SinClashNum = ClashValue(Sinner->Skillbase, Sinner->coins, Sinner->SkillcoinPow, Sinner->Sanity);
        EnClashNum = ClashValue(Enemy->Skillbase, Enemy->coins, Enemy->SkillcoinPow, Enemy->Sanity);
        // check who wins
        Enemy->coins -= (EnClashNum < SinClashNum);
        Sinner->coins -= (SinClashNum < EnClashNum);
        Clashes++;
        if(Clashes == MAX_CLASH) return Clashes;
    }
    if(Sinner->coins > Enemy->coins){ 
        //sp gain for sinner and loss for enemy
        Sinner->Sanity += (10 + Clashes);
        Sinner->Sanity = LimitSanity(&Sinner->Sanity);
        Enemy->Sanity -= ENSANITYLOSS;
        Enemy->Sanity = LimitSanity(&Enemy->Sanity);
        Enemy->Health -= Damagedealt(Sinner, Clashes);
    }
    else{
        Enemy->Sanity += (10 + Clashes);
        Enemy->Sanity = LimitSanity(&Enemy->Sanity);
        Sinner->Sanity -= SINSANITYLOSS;
        Sinner->Sanity = LimitSanity(&Sinner->Sanity);
        Sinner->Health -= Damagedealt(Enemy, Clashes);
    }
    return Clashes;
}
//Damage where a character doesnt clash
void UnopposedAtk(Characters *Attack, Characters *Oppo)
{
    Oppo->Health -= Damagedealt(Attack, 0); //No clashes so pass in clashing conditionals (Sanity, opposing stats)
}

//Adds skill ranks to the enemy, sinners and buffer skill arrays
bool CreateSkillStores(int SkillOptions[][2], int EnSkillOrder[][2], int BufferSkill[], int SkillList[], int TurnCount)
{
    if(TurnCount == 1){
        for(int i = 0; i < 5/*Amount of sinners*/; i++){
            for(int j = 0; j < 2/*skill choices*/; j++){
                SkillOptions[i][j] = SkillList[Form_or_Select_Random_Skill()];
            }
        }
    }
    for(int k = 0; k < 5; k++){
        BufferSkill[k] = SkillList[Form_or_Select_Random_Skill()];
    }
    for(int l = 0; l < 5; l++){
        EnSkillOrder[l][0] = SkillList[Form_or_Select_Random_Skill()];
    }
    return true; //Completed sucessfully
}

void SetUpBoss(SkillInfo Enskill[][4], u8 IsBoss)
{
    if(IsBoss == 1){
        int coin = 0;
        int base = 0;
        int coinpow = 0;
        for(int i = 0; i < 3; i++){
            coin = Enskill[0][i].coins;
            base = Enskill[0][i].Skillbase;
            coinpow = Enskill[0][i].SkillcoinPow;
            for(int j = 1; j < 5; j++){
                Enskill[j][i].coins = coin;
                Enskill[j][i].Skillbase = base;
                Enskill[j][i].SkillcoinPow = coinpow;
            }
        }
    }
}

void DefenceAgainstAtk(Characters *Sinner, SkillInfo *SinSkill, Characters *Enemy, u8 *EvadeResult)
{
    switch(SinSkill->coins) //defence type of fourth column fir each sinner
    {
        case Guard:
        Sinner->Health -= GuardDmgDealt(Sinner, Enemy);
        break;

        case ClashGuard:
        Sinner->Health -= (float)( (1.0f - (float)(ClashValue(SinSkill->Skillbase, 1, SinSkill->SkillcoinPow, Sinner->Sanity) * 0.01)) * Damagedealt(Enemy, 0) ); //reduce dmg by a percentage
        break;

        case Evade:
        *EvadeResult = EvadeDmg(Sinner, Enemy);
        break;

        default: //defence skill type higher than 2, must be a reuglar counter

        break;
    }
}