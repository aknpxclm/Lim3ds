#include <stdlib.h>
#include <time.h>

//Generate a random skill value to compare and clash with
int ClashValue(int coins, int Skillbase, int SkillcoinPow, int Sanity){
 int total = 0;
 total += Skillbase;
for (int i = 0; i < coins; i++)
{
    int coinFlip = rand() % 100;
    if (coinFlip < Sanity){
        total += SkillcoinPow;
    }
}
return total;
}
//Calculate damage of skill
int Damagedealt(int coins, int Skillbase, int SkillcoinPow, int Clashes){
return Skillbase + (coins * SkillcoinPow) * (1 + (3 * (Clashes * 0.01))); 
}
//Generates seed
void SeedStart(){
srand(time(NULL) * (rand() % 100));
}
//Keeps sanity between 5 and 95 (displayed as -45 and 45)
int LimitSanity(int* Sanity){
if(*Sanity > 95){
return 95;
}
else if(*Sanity < 5){
return 5;
}
else{
return *Sanity;
}
}
//Returns a random index from 0 - 5 to swap a skill rank to
int Form_or_Select_Random_Skill(){
    return rand() % 5;
}

//Rearranges the skill rank pool to make getting skills more random
void Rearrange_SkillPool(int *SkillList){
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
