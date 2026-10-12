#include <3ds.h>
#include <stdlib.h>
#include <time.h>
#include "Skill.h"
#include "Sinner_Enemy_defin.h"

//Generate a random skill value to compare and clash with
u32 ClashValue(u8 base, u8 coins, u8 coinPow, u8 sanity)
{
int coinFlip = 0;
int total = 0;
total += base;
for (int i = 0; i < coins; i++)
{
    coinFlip = rand() % 100;
    total += (coinFlip < sanity) * coinPow;
}
if(total <= 0) return 0; //minus coin check
return total;
}

u32 Damagedealt(Characters *Charac, int Clashes)
{
return Charac->Skillbase + (Charac->coins * Charac->SkillcoinPow) * (1 + (3 * ((float)Clashes * 0.01))); 
}

u32 GuardDmgDealt(Characters *Charac, Characters *AttackingCharac)
{
    int coinFlip = 0;
    int total = 0;
    total += Charac->Skillbase;
    coinFlip = rand() % 100;
    total += (coinFlip < Charac->Sanity) * Charac->SkillcoinPow;
    return Damagedealt(AttackingCharac, 0) - total;
}

u8 EvadeDmg(Characters *EvadeCharac, Characters *AttackingCharac)
{
    u8 i = 0;
    int coinFlip = 0;
    int totalAtk = 0;
    int totalEvade = 0;
    totalAtk += AttackingCharac->Skillbase;
    totalEvade += EvadeCharac->Skillbase;
    for(i = 0; i < AttackingCharac->coins; i++)
    {
        coinFlip = rand() % 100;
        totalAtk += (coinFlip < AttackingCharac->Sanity) * AttackingCharac->SkillcoinPow;
        totalEvade += (coinFlip < EvadeCharac->Sanity) * EvadeCharac->SkillcoinPow;
        if(totalAtk > totalEvade)
        {
            goto FailEvade;
        }
        totalEvade -= EvadeCharac->SkillcoinPow; //reset to default
    }
    return 0;

    FailEvade:
    EvadeCharac->Health -= Damagedealt(AttackingCharac, 0);
    return i;
}

void SeedStart()
{
srand(time(NULL));
}

u8 LimitSanity(u8 *Sanity)
{
//If Sanity is with 5 - 95 return the orig val, if san < 5 return 5, if san > 95 return 95
if(*Sanity > 95){
    return 95;
}
else if(*Sanity < 5){
    return 5;
}
else return *Sanity;
//return *Sanity * ((*Sanity < 95) & (*Sanity > 5)) + 95 * (*Sanity > 95) + 5 * (*Sanity < 5);
}
//Returns a random index from 0 - 5 to swap a skill rank to
int Form_or_Select_Random_Skill(){
    return rand() % 5;
}

//Rearranges the skill rank pool to make getting skills more random
void Rearrange_SkillPool(u8 SkillList[])
{
/*index 0 represents one value in SkillList
index 1 represents another value 
index 2 represents a random index of SkillList to swap to*/
int Swap[3] = {0, 0, 0};

for(int j = 0; j < 6; j++){
    Swap[0] = SkillList[j];
    Swap[2] = Form_or_Select_Random_Skill();
    Swap[1] = SkillList[Swap[2]];
    SkillList[j] = Swap[1];
    SkillList[Swap[2]] = Swap[0];
}
}

//Move skills down the selction area when a skill is used
void ShiftSkillSelects(u8 SkillOptions[][2], u8 BufferSkill[], u8 SkillList[])
{
int TopRowSkill = 0;
int BufferSkillnum = 0;

//9 == NOTSELECTED macro
for(int i = 0; i < 5; i++){
    TopRowSkill = SkillOptions[i][1];
    BufferSkillnum = BufferSkill[i];

    if(SkillOptions[i][0] == 9){ //Bottom row skill used
        SkillOptions[i][0] = TopRowSkill;
        SkillOptions[i][1] = BufferSkillnum;
    }
    else if(SkillOptions[i][1] == 9){ //Top row skill used
        SkillOptions[i][1] = BufferSkillnum;
    }
    BufferSkill[i] = SkillList[Form_or_Select_Random_Skill()]; //generate a new skill rank in the buffer
}
}