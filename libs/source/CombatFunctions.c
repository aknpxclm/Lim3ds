#include <stdio.h>
#include <stdbool.h>
#include "Skill.h"

#define ENSANITYLOSS 5
#define SINSANITYLOSS 3


//If a skill is going to clash the enemy's, use this function to clash and deal damage
void ClashingAtk(int* SinSanity, int* EnSanity, int* SinCoin, int* EnCoin, int SinBase, int EnBase, int SinPow, int EnPow, double* SinHealth, double* EnHealth){
    int SinClashNum = 0;
    int EnClashNum = 0;
    int Clashes = 0;
    while(*SinCoin > 0 && *EnCoin > 0){ //clash loop
        // get clash values for enemy and sinner
        SinClashNum = ClashValue(*SinCoin, SinBase, SinPow, *SinSanity);
        EnClashNum = ClashValue(*EnCoin, EnBase, EnPow, *EnSanity);
        // check who wins
        if(SinClashNum > EnClashNum){ //Sinner win
            *EnCoin -= 1;
        }
        else if(SinClashNum < EnClashNum){ // enemy win
            *SinCoin -= 1;
        }
        Clashes++;
    }
    printf("\x1b[21;20H%d Clashes!", Clashes);
    Clashes = 0;
    //check who "won" in total
    if(*SinCoin > *EnCoin){ 
        //sp gain for sinner and loss for enemy
        *SinSanity = LimitSanity((SinSanity + (10 + Clashes)));
        *EnSanity -= ENSANITYLOSS;
        *EnSanity = LimitSanity(EnSanity);
        *EnHealth -= Damagedealt(*SinCoin, SinBase, SinPow, Clashes);
    }
    else{
        *EnSanity = LimitSanity((EnSanity + (10 + Clashes)));
        *SinSanity -= SINSANITYLOSS;
        *SinSanity = LimitSanity(SinSanity);
        *SinHealth -= Damagedealt(*EnCoin, EnBase, EnPow, Clashes);
    }
}
//Damage where a character doesnt clash
void UnopposedAtk(int Coin, int Base, int Pow, double* OpposingHealth){
    *OpposingHealth -= Damagedealt(Coin, Base, Pow, 0); //No clashes so pass in no clashing conditionals (Sanity, opposing stats)
}
//compares the priority of two skills 
int ComparePriority(int Pri1,int Pri2){
if(Pri1 > Pri2){
    return 1;
}
else{
    return 0;
}
}

void FinishTurn(bool TurnStart, bool SkillsRandomlySet, bool SkillOrderSet, int* TurnCount){
    
    //End this turn and start the next one
    TurnStart = !TurnStart;
    SkillsRandomlySet = !SkillsRandomlySet;
    SkillOrderSet = !SkillOrderSet;
    TurnCount += 1;
    
}
