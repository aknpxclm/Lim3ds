#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Skill.h"
#include "CombatFunctions.h"
#include "SkillUIPositions.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240
#define MAX_SPRITES 768

#define StartScreen 0
#define MainMenu 1
#define CombatMenu 2

void ExitApp(){
C2D_Fini();
C3D_Fini();
romfsExit();
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

typedef struct Clashing_Checks{
    int SkillClashing;
    int Priority;
    bool IsClashing;
    bool IsUnclashed; //Based on if the enemy is clashing

}ClashParams;

typedef struct
{
	C2D_Sprite spr;
	float dx, dy; // velocity
} Sprite;

static C2D_SpriteSheet menuspriteSheet;
static Sprite sprites[MAX_SPRITES];

int main(int argc, char **argv){  // initialise variables
gfxInitDefault();
consoleInit(GFX_BOTTOM, NULL);
romfsInit();
C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
C2D_Prepare();

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

SkillTouchPos UIPostion[5] = {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};

//skill number/order for main boss second array is used to find the index for AtkOrder
int EnSkillOrder[5][2] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}};

//Each element is assigned a index based on ther skill selected to attack the array above
int AttackOrder[5][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};

//higher priority means skill will clash over other skills
int SkillPriorityLevel[5] = {0};

//skill numbers for each skill slot for any amount for sinners
int SkillOptions[5][2] = {{0, 0},{0, 0},{0, 0},{0, 0},{0, 0}};

// original order before skills will be randomised and listed / picked from
int SkillList[6] = {1, 1, 1, 2, 2, 3}; //Sinners can only have three skill 1s, two skill 2s and , one skill 3

int SelectlotNum[5] = {0};

int MenuPostion = StartScreen;

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
Rearrange_SkillPool(SkillList);

C3D_RenderTarget *top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	menuspriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/menu.t3x");
	if (!menuspriteSheet) svcBreak(USERBREAK_PANIC);

Sprite *sprite = &sprites[0]; //Setting config for the menu sprite for drawing
	C2D_SpriteFromSheet(&sprite->spr, menuspriteSheet, 0/*sprite index in the sheet*/);
	C2D_SpriteSetCenter(&sprite->spr, 0.1f, 0.1f);
	C2D_SpriteSetPos(&sprite->spr, 30/*X position*/, -40/*Y position*/);
	C2D_SpriteSetRotation(&sprite->spr, 0);
	C2D_SpriteSetScale(&sprite->spr, 1/*X scale*/, 1/*Y scale*/);
	sprite->dx = 0;
	sprite->dy = 0;

while(aptMainLoop()){
    hidScanInput();
    u32 kDown = hidKeysDown();
    if(kDown & KEY_START) break;
    touchPosition touch;
    hidTouchRead(&touch);

switch(MenuPostion){ // Playing menu

    case StartScreen: //Start screen
    if(MenuPostion == StartScreen){ //Prevent redrawing the startmenu sprites when changing menu position
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
	C2D_SceneBegin(top);
	C2D_DrawSprite(&sprites[0].spr);
	C3D_FrameEnd(0);
    if(kDown & KEY_TOUCH) MenuPostion++; //Go to MainMenu
    }
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
                //check if clashing
                if(AttackOrder[Search][CurrentIndex] == EnSkillOrder[Search][CurrentIndex]){
                    SkillPosInfo[Search].IsClashing = true;
                    SkillPosInfo[Search].SkillClashing = Search;
                    SelectSlotAppeared[AttackOrder[Search][CurrentIndex]] = !SelectSlotAppeared[AttackOrder[Search][CurrentIndex]]; // flip to true
                    SkillPriorityLevel[AttackOrder[Search][CurrentIndex]] = AttackOrder[Search][CurrentIndex]; //record what skill slot was targeted
                }
                //check if skill is going unopposed while another skill clashes the same slot
                else if(SelectSlotAppeared[AttackOrder[Search][CurrentIndex]] == true){
                    SkillPosInfo[Search].IsClashing = ComparePriority(SkillPriorityLevel[Search], SkillPriorityLevel[AttackOrder[Search][CurrentIndex]]);
                    if(SkillPosInfo[Search].IsClashing == true){
                        SkillPosInfo[AttackOrder[Search][CurrentIndex]].IsClashing = false;
                    }
                    // reset check bool
                    SelectSlotAppeared[AttackOrder[Search][CurrentIndex]] = !SelectSlotAppeared[AttackOrder[Search][CurrentIndex]];
                }
                //check if enemy attacks wil go unopposed
                else if(EnSkillOrder[Search][CurrentIndex] != AttackOrder[0][CurrentIndex] || 
                    EnSkillOrder[Search][CurrentIndex] != AttackOrder[1][CurrentIndex] || 
                    EnSkillOrder[Search][CurrentIndex] != AttackOrder[2][CurrentIndex] || //Awful counter: 2
                    EnSkillOrder[Search][CurrentIndex] != AttackOrder[3][CurrentIndex] || 
                    EnSkillOrder[Search][CurrentIndex] != AttackOrder[4][CurrentIndex])
                    {SkillPosInfo[Search].IsUnclashed = true;}
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

        //End this turn and start the next one
        TurnStart = !TurnStart;
        SkillsRandomlySet = !SkillsRandomlySet;
        SkillOrderSet = !SkillOrderSet;
        TurnCount += 1;

            if(Enemy[4].Health < 0){
                consoleClear();
                printf("\x1b[16;20HYOU WON");
                TurnStart = !TurnStart;
            }
        } // Turn loop
    } // Turn Running loop
    break; //Leave case 2
} 

    gfxFlushBuffers();
    gfxSwapBuffers();
    gspWaitForVBlank();
}
C2D_SpriteSheetFree(menuspriteSheet);
ExitApp();
return 0;
}
