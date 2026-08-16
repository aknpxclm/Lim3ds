#include <stdio.h>
#include <stdbool.h>
#include "Skill.h"

#define ENSANITYLOSS 5
#define SINSANITYLOSS 3


//If a skill is going to clash the enemy's, use this function to clash and deal damage
int ClashingAtk(int* SinSanity, int* EnSanity, int* SinCoin, int* EnCoin, int SinBase, int EnBase, int SinPow, int EnPow, double* SinHealth, double* EnHealth){
    int SinClashNum = 0;
    int EnClashNum = 0;
    int Clashes = 0;
    while(*SinCoin > 0 && *EnCoin > 0){ //clash loop
        // get clash values for enemy and sinner
        SinClashNum = ClashValue(*SinCoin, SinBase, SinPow, *SinSanity);
        EnClashNum = ClashValue(*EnCoin, EnBase, EnPow, *EnSanity);
        // check who wins
        *EnCoin -= (EnClashNum < SinClashNum); //if enemy won subtract 0
        *SinCoin -= (SinClashNum < EnClashNum); //if sinner won subtract 0
        Clashes++;
    }
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
    return Clashes;
}
//Damage where a character doesnt clash
void UnopposedAtk(int Coin, int Base, int Pow, double* OpposingHealth){
    *OpposingHealth -= Damagedealt(Coin, Base, Pow, 0); //No clashes so pass in no clashing conditionals (Sanity, opposing stats)
}
//compares the priority of two skills 
int ComparePriority(int Pri1,int Pri2){
return (Pri1 > Pri2); //Branchless???
}

//Adds skill ranks to the enemy, sinners and buffer skill arrays
int CreateSkillStores(int SkillOptions[][2], int EnSkillOrder[][2], int BufferSkill[], int SkillList[], int TurnCount){
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
    return 1; //Completed sucessfully
}