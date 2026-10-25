#include <3ds.h>
#include <stdlib.h>
#include <time.h>
#include "Skill.h"
#include "Sinner_Enemy_defin.h"

/// @brief Generate a random skill value to compare and clash with
/// @param base Skill base number
/// @param coins Total coins a skill has
/// @param coinPow The total amount a coin can add to the total
/// @param sanity How likely a skill will add the coin power from 5% to 95%
/// @return 
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

/// @brief Calculates damage a skill will do to an opponent
/// @param Charac Character to deal dameage to
/// @param Clashes Total Clashes completed
/// @return Damage that will be dealt
u32 Damagedealt(Characters *Charac, u16 Clashes)
{
return Charac->Skillbase + (Charac->coins * Charac->SkillcoinPow) * (1 + (3 * ((float)Clashes * 0.01))); 
}

/// @brief Amount of damage to deal under the clashable guards damage reduction
/// @param Charac Defending character
/// @param AttackingCharac Attacking character
/// @return 
u32 GuardDmgDealt(Characters *Charac, Characters *AttackingCharac)
{
    int coinFlip = 0;
    int total = 0;
    total += Charac->Skillbase;
    coinFlip = rand() % 100;
    total += (coinFlip < Charac->Sanity) * Charac->SkillcoinPow;
    return Damagedealt(AttackingCharac, 0) - total;
}

/// @brief Allows an evading skill to negate incoming damage if the evade's total is greater than or equal to the opposing skill total
/// @param EvadeCharac Evading character
/// @param AttackingCharac Attacking character
/// @return Coin evade failed at, 0 if never failed
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

/// @brief Generates a seed
void SeedStart()
{
srand(time(NULL));
}

/// @brief Clamps sanity between 5 and 95, displayed as -45 to 45
/// @param Sanity Character sanity
/// @return Clamped sanity value
u8 LimitSanity(u8 *Sanity)
{
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

/// @brief Rearranges the skill rank pool to make getting skills more random
/// @param SkillList Pool of skill ranks that can be collected from
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

/// @brief Move skills down the selction area when a skill is used
/// @param SkillOptions Skill rank options a player chooses from
/// @param BufferSkill Next skill rank to be placed as a skill option
/// @param SkillList Pool to take a skill rank from
void ShiftSkillSelects(u8 SkillOptions[][2], u8 BufferSkill[], u8 SkillList[])
{
u8 TopRowSkill = 0;
u8 BufferSkillnum = 0;

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