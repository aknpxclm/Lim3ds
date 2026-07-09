#include <3ds.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Skill.h"
#include "asd.h"

#define StartScreen 0
#define MainMenu 1
#define CombatMenu 2

void ExitApp(){
//C2D_Fini();
//C3D_Fini();
//romfsExit();
gfxExit();
}

struct Characters{
double Health;
double OldHealth;
int coins;
int Skillbase;
int SkillcoinPow;
int Sanity;
int Setcoins;
int SetSkillbase;
int SetSkillcoinPow;
};

typedef struct IsClashing_Check{
    int SkillClashing;
    int Priority;
    bool IsClashing;
    bool IsUnclashed; //Based on if the enemy is clashing

}ClashParams;

int main(int argc, char **argv){  // initialise variables
gfxInitDefault();
consoleInit(GFX_BOTTOM, NULL);
//romfsInit();
//C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
//C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
//C2D_Prepare();
hidInit();

//placeholder stats till i can read files for values in a json or other c file
struct Characters Sinner[5] = {{195.0f, 0.0, 2, 4, 4, 50, 2, 4, 4}, 
                               {0.0f, 0.0, 0, 0, 0, 50, 0, 0, 0}, 
                               {0.0f, 0.0, 0, 0, 0, 50, 0, 0, 0}, 
                               {0.0f, 0.0, 0, 0, 0, 50, 0, 0, 0}, 
                               {0.0f, 0.0, 0, 0, 0, 50, 0, 0, 0}};  //This looks so awful
struct Characters Enemy[5] = {{1560.0f, 0, 2, 4, 2, 50, 2, 4, 2},  
                              {1560.0f, 0, 2, 4, 2, 50, 2, 4, 2}, 
                              {1560.0f, 0, 2, 4, 2, 50, 2, 4, 2}, 
                              {1560.0f, 0, 2, 4, 2, 50, 2, 4, 2}, 
                              {1560.0f, 0, 2, 4, 2, 50, 2, 4, 2}};

ClashParams SkillPosInfo[5] = {{0, 0, false, false}, {0, 0, false, false}, {0, 0, false, false}, {0, 0, false, false}, {0, 0, false, false}};
//skill number/order for main boss second array is used to find the index for AtkOrder
int EnSkillOrder[5][2] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}};

//Each element is assigned a index based on ther skill selected to attack the array above
int AttackOrder[5][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};

//higher priority means skill will clash over other skills
int SkillPriorityLevel[5] = {0};

//skill numbers for each skill slot for any amount for sinners
int SkillOptions[5][2] = {{0, 0},{0, 0},{0, 0},{0, 0},{0, 0}};

// original order before skills will be randomised and listed / picked from
int SkillList[6] = {1, 1, 1, 2, 2, 3}; 

int SelectlotNum[5] = {0};

/*index 0 represents one value in SkillList
index 1 represents another value 
index 2 represents a random index of SkillList to swap to*/
int Swap[3] = {0, 0, 0};

int MenuPostion = 0;

int SinClashNum = 0;
int EnClashNum = 0;
int TurnCount = 1; 
bool SelectSlotAppeared[5] = {false, false, false, false, false};
bool TurnStart = false;
bool StatsPrinted = false;
bool StartMenuVisible = false;
bool SkillsRandomlySet = false;
bool SkillOrderSet = false;
SeedStart();

for(int j = 0; j < 6; j++){
    Swap[0] = SkillList[j];
    Swap[2] = Form_or_Select_Random_Skill();
    Swap[1] = SkillList[Swap[2]];
    SkillList[j] = Swap[1];
    SkillList[Swap[2]] = Swap[0];
}

while(aptMainLoop()){
    hidScanInput();
    u32 kDown = hidKeysDown();
    if(kDown & KEY_START) break;
    touchPosition touch;
    hidTouchRead(&touch);

switch(MenuPostion){ // Playing menu
        case StartScreen: //Start screen
        if(kDown & KEY_TOUCH) MenuPostion++;
        break;

        case MainMenu: //Main menu
        break;
        
        case CombatMenu: //Combat select area
        
        if(!SkillsRandomlySet){
            for(int i = 0; i < 5; i++){
                for(int j = 0; j < 2; j++){
                    SkillOptions[i][j] = SkillList[Form_or_Select_Random_Skill()];
                }
            }
            SkillsRandomlySet = !SkillsRandomlySet;
        }
        if(!SkillOrderSet){
            for(int k = 0; k < 5; k++){
                EnSkillOrder[k][0] = SkillList[Form_or_Select_Random_Skill()];
            }
            SkillOrderSet = !SkillOrderSet;
        }
        if (!TurnStart){
        //(Should Draw / Make menu)
        //This is currently a band-aid solution
        if( Sinner[0].OldHealth != Sinner[0].Health || Enemy[0].OldHealth != Enemy[0].Health){
            StatsPrinted = false;
        }
        if(!StatsPrinted){
        Sinner[0].OldHealth = Sinner[0].Health;
        Enemy[0].OldHealth = Enemy[0].Health;
        printf("\x1b[16;20HPress L to start combat"); 
        printf("\x1b[17;20HTurn %d", TurnCount);
        printf("\x1b[18;20HHealth: %f", Sinner[0].Health);
        printf("\x1b[19;20HHealth: %f", Enemy[0].Health);
        printf("\x1b[20;14HSinner Sanity: %d    Enemy Sanity: %d", Sinner[0].Sanity - 50, Enemy[0].Sanity - 50);
        StatsPrinted = true;
        }
        if(SkillsRandomlySet == true && SkillOrderSet == true){
             if (kDown & KEY_L) TurnStart = !TurnStart;
        }

        }
        else{
        consoleClear();
        while(TurnStart){ //combat turn loop

            for(int Search = 0; Search < 5; Search++){
                if(AttackOrder[Search][1] == EnSkillOrder[Search][1]){ //check if clashing
                    SkillPosInfo[Search].IsClashing = true;
                    SkillPosInfo[Search].SkillClashing = Search;
                    SelectSlotAppeared[AttackOrder[Search][1]] = !SelectSlotAppeared[AttackOrder[Search][1]]; // flip to true
                    SkillPriorityLevel[AttackOrder[Search][1]] = AttackOrder[Search][1]; //record what skill slot was targeted
                }
               else if(SelectSlotAppeared[AttackOrder[Search][1]] == true){ //check if skill is going unopposed while another skill clashes the same slot
                   SkillPosInfo[Search].IsClashing = ComparePriority(SkillPriorityLevel[Search], SkillPriorityLevel[AttackOrder[Search][1]]);
                   if(SkillPosInfo[Search].IsClashing == true){
                        SkillPosInfo[AttackOrder[Search][1]].IsClashing = false;
                    }
                  SelectSlotAppeared[AttackOrder[Search][1]] = !SelectSlotAppeared[AttackOrder[Search][1]]; // reset check bool
                }
             else if(EnSkillOrder[Search][1] != AttackOrder[0][1] || 
                    EnSkillOrder[Search][1] != AttackOrder[1][1] || 
                    EnSkillOrder[Search][1] != AttackOrder[2][1] || //Awful counter: 2
                    EnSkillOrder[Search][1] != AttackOrder[3][1] || 
                    EnSkillOrder[Search][1] != AttackOrder[4][1]){
                        SkillPosInfo[Search].IsUnclashed = true;} //check if enemy attacks wil go unopposed

        }
        for(int SinCompleted = 0; SinCompleted < 5; SinCompleted++){
          Sinner[SinCompleted].coins = Sinner[SinCompleted].Setcoins;
          Sinner[SinCompleted].Skillbase = Sinner[SinCompleted].SetSkillbase;
          Sinner[SinCompleted].SkillcoinPow = Sinner[SinCompleted].SetSkillcoinPow;
        
          if(SinCompleted > 0){
            Enemy[SinCompleted].Health = Enemy[SinCompleted - 1].Health;
            Enemy[SinCompleted].Sanity = Enemy[SinCompleted - 1].Sanity;
          }

          Enemy[SinCompleted].coins = Enemy[SinCompleted].Setcoins;
          Enemy[SinCompleted].Skillbase = Enemy[SinCompleted].SetSkillbase;
          Enemy[SinCompleted].SkillcoinPow = Enemy[SinCompleted].SetSkillcoinPow;

            //Holy arguements
            if(SkillPosInfo[SinCompleted].IsClashing == true && SkillPosInfo[SinCompleted].IsUnclashed == false){ //Enemy and sinner clash skills
            ClashingAtk(&Sinner[SinCompleted].Sanity, &Enemy[SinCompleted].Sanity, 
                        &Sinner[SinCompleted].coins, &Enemy[SinCompleted].coins, 
                        Sinner[SinCompleted].Skillbase, Enemy[SinCompleted].Skillbase, 
                        Sinner[SinCompleted].SkillcoinPow, Enemy[SinCompleted].SkillcoinPow, 
                        &Sinner[SinCompleted].Health, &Enemy[SinCompleted].Health);
            }

            else if(SkillPosInfo[SinCompleted].IsUnclashed == true && SkillPosInfo[SinCompleted].IsClashing == false){ //Enemy is going to attack unopposed
                UnopposedAtk(Enemy[SinCompleted].coins, Enemy[SinCompleted].Skillbase, Enemy[SinCompleted].SkillcoinPow, &Sinner[SinCompleted].Health);
            }

            else{ //Sinner is going to attack unopposed
                UnopposedAtk(Sinner[SinCompleted].coins, Sinner[SinCompleted].Skillbase, Sinner[SinCompleted].SkillcoinPow, &Enemy[SinCompleted].Health);
            }
        
        } //cycle through each sinner and clashing or going unopposed then go to the next one. Does this 5 times
        
        if(Enemy[4].Health < 0){
            consoleClear();
           printf("\x1b[16;20HYOU WON");
            TurnStart = !TurnStart;
        }
        else{
        //End this turn and start the next one
        TurnStart = !TurnStart;
        SkillsRandomlySet = !SkillsRandomlySet;
        SkillOrderSet = !SkillOrderSet;
        TurnCount++;
        } // Win condition check
        
                } // Turn loop
            } // Turn Running loop
        break; //Leave case 2
} 

    gfxFlushBuffers();
    gfxSwapBuffers();
    gspWaitForVBlank();
}
ExitApp();
return 0;
}
