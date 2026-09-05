#include "Skill.h"
#include "Sinner_Enemy_defin.h"
#include "CombatFunctions.h"

#define ENSANITYLOSS 5
#define SINSANITYLOSS 3


//If a skill is going to clash the enemy's, use this function to clash and deal damage
int ClashingAtk(Characters *Sinner, Characters *Enemy){
    int SinClashNum = 0;
    int EnClashNum = 0;
    int Clashes = 0;
    while(Sinner->coins > 0 && Enemy->coins > 0){ //clash loop
        // get clash values for enemy and sinner
        SinClashNum = ClashValue(Sinner->coins, Sinner->Skillbase, Sinner->SkillcoinPow, Sinner->Sanity);
        EnClashNum = ClashValue(Enemy->coins, Enemy->Skillbase, Enemy->SkillcoinPow, Enemy->Sanity);
        // check who wins
        Enemy->coins -= (EnClashNum < SinClashNum); //if enemy won subtract 0
        Sinner->coins -= (SinClashNum < EnClashNum); //if sinner won subtract 0
        Clashes++;
    }
    //check who "won" in total
    if(Sinner->coins > Enemy->coins){ 
        //sp gain for sinner and loss for enemy
        Sinner->Sanity += (10 + Clashes);
        Sinner->Sanity = LimitSanity(&Sinner->Sanity);
        Enemy->Sanity -= ENSANITYLOSS;
        Enemy->Sanity = LimitSanity(&Enemy->Sanity);
        Enemy->Health -= Damagedealt(Sinner->coins, Sinner->Skillbase, Sinner->SkillcoinPow, Clashes);
    }
    else{
        Enemy->Sanity += (10 + Clashes);
        Enemy->Sanity = LimitSanity(&Enemy->Sanity);
        Sinner->Sanity -= SINSANITYLOSS;
        Sinner->Sanity = LimitSanity(&Sinner->Sanity);
        Sinner->Health -= Damagedealt(Enemy->coins, Enemy->Skillbase, Enemy->SkillcoinPow, Clashes);
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