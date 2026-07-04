#include <stdlib.h>
#include <time.h>
//generate a random skill value to compare and clash with
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
//generate seed
void SeedStart(){
srand(time(NULL) * (rand() % 100));
}
//keep sanity between 5 and 95 (displayed as -45 and 45)
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
//Make old location the new location
void EqualiseChecks(int Location1, int CurrentLocation1){
    Location1 = CurrentLocation1;
}

int Form_or_Select_Random_Skill(){
    return rand() % (5 + 1 - 1) + 1;
}