#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <stdbool.h>
#include "Skill.h"
#include "CombatFunctions.h"
#include "SkillUIPositions.h"

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

//Spritesheets
static C2D_SpriteSheet menuSpriteSheet;

//Text and text buffers
C2D_TextBuf staticBuf;
C2D_TextBuf dynamBuf;
C2D_Text staticTex[2];

typedef struct{
double Health;
double OldHealth;
int coins;
int Skillbase;
int SkillcoinPow;
int Sanity;
int Setcoins;
int SetSkillbase;
int SetSkillcoinPow;
}Characters;

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

static Sprite Msprites[MAX_SPRITES];

//sprite animation example from http://www.nyankolab.com/
static const u64 GFXRefresh = 25/*ms*/; //refresh graphics 40 times a second for 40fps

int main(int argc, char **argv){  // initialise variables
gfxInitDefault();
consoleInit(GFX_BOTTOM, NULL);
romfsInit();
C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
C2D_Prepare();
//placeholder stats till i can read files for values in a json or other c file
Characters Sinner[5] = {{195.0f, 0.0, 2, 4, 4, 50, 2, 4, 4}, 
                        {0.0f, 0.0, 0, 0, 0, 50, 0, 0, 0}, 
                        {0.0f, 0.0, 0, 0, 0, 50, 0, 0, 0}, 
                        {0.0f, 0.0, 0, 0, 0, 50, 0, 0, 0}, 
                        {0.0f, 0.0, 0, 0, 0, 50, 0, 0, 0}};  //This looks so awful                            
Characters Enemy[5] = {{1560.0f, 0, 2, 4, 2, 50, 2, 4, 2},  
                       {1560.0f, 0, 2, 4, 2, 50, 2, 4, 2}, 
                       {1560.0f, 0, 2, 4, 2, 50, 2, 4, 2}, 
                       {1560.0f, 0, 2, 4, 2, 50, 2, 4, 2}, 
                       {1560.0f, 0, 2, 4, 2, 50, 2, 4, 2}};

ClashParams SkillPosInfo[5] = {{0, 0, false, false}, {0, 0, false, false}, {0, 0, false, false}, {0, 0, false, false}, {0, 0, false, false}};
SkillTouchPos UIPostion[5] = {FirstSkill, SecondSkill, ThirdSkill, FourthSkill, FifthSkill}; // X & Y areas for touch selecting skills on the bottom screen
SkillTouchPos EnUIPostion[5] = {FirstSkill, SecondSkill, ThirdSkill, FourthSkill, FifthSkill};

u64 InitialTimeMs = 0;
u64 CurrentTimeMs = 0;
u64 ElapsedTimeMs = 0;

int CurrentFrameIndex = 0;
size_t SkillSprites = 0;;

int AttackOrder[5][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};  //Each element is assigned a index based on ther skill selected to attack the Character array above
int EnSkillOrder[5][2] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}}; //skill number/order for main boss, second dimension is used to find the index for AtkOrder
int SkillPriorityLevel[5] = {0};                                   //higher priority means skill will clash over other skills
int SkillOptions[5][2] = {{0, 0},{0, 0},{0, 0},{0, 0},{0, 0}};     //skill numbers for each skill slot for any amount for sinners
int BufferSkill[5] = {0, 0, 0, 0, 0};                              // original order before skills will be randomised and listed / picked from
int SkillList[6] = {1, 1, 1, 2, 2, 3};                             //Sinners can only have three skill 1s, two skill 2s and , one skill 3
int SelectlotNum[5] = {0};
int CurrentSinner = 0;
int Clashes[5] = {0};
int TurnCount = 1;
int TurnStart = 0; //0: Combat menu, 1: Clashes and dmg applied, 1< gfx
int MenuPosition = StartScreen;
bool SelectSlotAppeared[5] = {false, false, false, false, false};
bool CreatedSkillStores = false;

InitialTimeMs = osGetTime();
SeedStart();
Rearrange_SkillPool(SkillList); //Moves the values in SkillList[] (L98) to a random position

//Create 3ds Render targets for the screens
C3D_RenderTarget *top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
C3D_RenderTarget *bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

//Allocate memory for the buffers
staticBuf = C2D_TextBufNew(4096);
dynamBuf = C2D_TextBufNew(4096);

//Parse conditional game text
C2D_TextParse(&staticTex[0], staticBuf, "Victory");
C2D_TextParse(&staticTex[1], staticBuf, "Defeat");
C2D_TextOptimize(&staticTex[0]); //Optimizes the text to be rendered efficiently
C2D_TextOptimize(&staticTex[1]);

menuSpriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/menu.t3x");
if (!menuSpriteSheet) svcBreak(USERBREAK_PANIC);

//Initialise all sprites in a sheet
size_t numsprites = C2D_SpriteSheetCount(menuSpriteSheet);
for(int x = 0; x < numsprites; x++){
    Sprite *Msprite = &Msprites[x];
    C2D_SpriteFromSheet(&Msprite->spr, menuSpriteSheet, x/*sprite index in the sheet*/);
    C2D_SpriteSetCenter(&Msprite->spr, 0.1f, 0.1f);
    C2D_SpriteSetPos(&Msprite->spr, 0/*X position*/, 0/*Y position*/);
    C2D_SpriteSetRotation(&Msprite->spr, 0);
    C2D_SpriteSetScale(&Msprite->spr, 1/*X scale*/, 1/*Y scale*/);
    Msprite->dx = 0;
    Msprite->dy = 0;
}

while(aptMainLoop()){

    hidScanInput(); //Scans for keys pressed
    u32 kDown = hidKeysDown();
    if(kDown & KEY_START) break;
    touchPosition touch;
    hidTouchRead(&touch);

switch(MenuPosition){ // In game start

    case StartScreen: //Start screen
    if(MenuPosition == StartScreen)
    { //Prevent redrawing the startmenu sprites when changing menu position
    C2D_SpriteMove(&Msprites[0].spr, 30, 20); //Move sprite to center view
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
	C2D_SceneBegin(top);
	C2D_DrawSprite(&Msprites[0].spr); //Draw Start_Screen.png
	C3D_FrameEnd(0);
    if(kDown & KEY_TOUCH) MenuPosition = MainMenu; //Switch to MainMenu
    }
    break;

    case MainMenu: //Main menu
    if(MenuPosition == MainMenu)
    {
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 0.65f)); //Gray background
	C2D_SceneBegin(top);
        if(touch.px/*pixel coordinate of x on the screen?*/ >= 288 && touch.px <= 736/*X area of detection*/ && touch.py >= 168 && touch.py <= 336 /*Y area of detection*/&& kDown & KEY_TOUCH) // if touchpad is pressed in the detection area...
        {
            MenuPosition = CombatMenu;
        }
        //more options in the menu will get their own conditions
    C3D_FrameEnd(0);
    }
    break;
        
    case CombatMenu: //Combat select area
    
    if (TurnStart == 0){
    //(Should Draw / Make menu) - unfinished
        if(CreatedSkillStores == false)
        {
        CreatedSkillStores = CreateSkillStores(SkillOptions, EnSkillOrder, BufferSkill, SkillList, TurnCount); //When completed returns true / 1
        }
        Sinner[0].OldHealth = Sinner[0].Health;
        Enemy[0].OldHealth = Enemy[0].Health;

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(bottom, C2D_Color32(0x82, 0x14, 0x00, 0xFF));
	    C2D_SceneBegin(bottom);
        //uses 3ds/graphics/printing/system-font example
        C2D_TextBufClear(dynamBuf); //clear previous text
        char bufff[256];
        C2D_Text dynamTex;
        snprintf(bufff, sizeof(bufff), "Health: %lf Sanity: %d", Sinner[0].Health, Sinner[0].Sanity); //write to buffer
        C2D_TextParse(&dynamTex, dynamBuf, bufff); //parse the buffer
        C2D_TextOptimize(&dynamTex);
        C2D_DrawText(&dynamTex, 0, 8.0f, 8.0f, 0.5f, 0.0f, 1.0f);
	    C3D_FrameEnd(0);
        
        if(CreatedSkillStores == true && kDown & KEY_L) //Prevent abrupt cancels
        {
            TurnStart = 1;
            for(int Search = 0; Search < 5; Search++){
                //check if clashing
                if(AttackOrder[Search][CurrentIndex] == EnSkillOrder[Search][CurrentIndex]){
                    SkillPosInfo[Search].IsClashing = true;
                    SkillPosInfo[Search].SkillClashing = Search;
                    SelectSlotAppeared[AttackOrder[Search][CurrentIndex]] = true; // skill is targeting a slot
                    SkillPriorityLevel[AttackOrder[Search][CurrentIndex]] = AttackOrder[Search][CurrentIndex]; //record what skill slot was targeted
                }
                //check if skill is going unopposed while another skill clashes the same slot
                if(SelectSlotAppeared[AttackOrder[Search][CurrentIndex]] == true){
                    SkillPosInfo[Search].IsClashing = ComparePriority(SkillPriorityLevel[Search], SkillPriorityLevel[AttackOrder[Search][CurrentIndex]]);
                    if(SkillPosInfo[Search].IsClashing == true){
                        SkillPosInfo[AttackOrder[Search][CurrentIndex]].IsClashing = false;
                    }
                    // reset check bool
                    SelectSlotAppeared[AttackOrder[Search][CurrentIndex]] = false;
                }
                //check if enemy attacks wil go unopposed, no sinner is clashing the slot
                if(EnSkillOrder[Search][CurrentIndex] != AttackOrder[0][CurrentIndex] || 
                    EnSkillOrder[Search][CurrentIndex] != AttackOrder[1][CurrentIndex] || 
                    EnSkillOrder[Search][CurrentIndex] != AttackOrder[2][CurrentIndex] || //Awful counter: 2
                    EnSkillOrder[Search][CurrentIndex] != AttackOrder[3][CurrentIndex] || 
                    EnSkillOrder[Search][CurrentIndex] != AttackOrder[4][CurrentIndex])
                    {
                    SkillPosInfo[Search].IsUnclashed = true;
                    }
            }
        }
    }
    else if(TurnStart == 1)
    { // Turn Running loop

        Sinner[CurrentSinner].coins = Sinner[CurrentSinner].Setcoins;
        Sinner[CurrentSinner].Skillbase = Sinner[CurrentSinner].SetSkillbase;
        Sinner[CurrentSinner].SkillcoinPow = Sinner[CurrentSinner].SetSkillcoinPow;
        
        if(CurrentSinner > 0){
            Enemy[CurrentSinner].Health = Enemy[CurrentSinner - 1].Health;
            Enemy[CurrentSinner].Sanity = Enemy[CurrentSinner - 1].Sanity;
        }

        Enemy[CurrentSinner].coins = Enemy[CurrentSinner].Setcoins;
        Enemy[CurrentSinner].Skillbase = Enemy[CurrentSinner].SetSkillbase;
        Enemy[CurrentSinner].SkillcoinPow = Enemy[CurrentSinner].SetSkillcoinPow;

        //Holy arguements
        if(SkillPosInfo[CurrentSinner].IsClashing == true && SkillPosInfo[CurrentSinner].IsUnclashed == false){ //Enemy and sinner clash skills, returns the amount of clashes between the skills
        Clashes[CurrentSinner] = ClashingAtk(&Sinner[CurrentSinner].Sanity, &Enemy[CurrentSinner].Sanity, 
                                            &Sinner[CurrentSinner].coins, &Enemy[CurrentSinner].coins, 
                                            Sinner[CurrentSinner].Skillbase, Enemy[CurrentSinner].Skillbase, 
                                            Sinner[CurrentSinner].SkillcoinPow, Enemy[CurrentSinner].SkillcoinPow, 
                                            &Sinner[CurrentSinner].Health, &Enemy[CurrentSinner].Health);
        }
        else if(SkillPosInfo[CurrentSinner].IsUnclashed == true && SkillPosInfo[CurrentSinner].IsClashing == false){ //Enemy is going to attack unopposed
            UnopposedAtk(Enemy[CurrentSinner].coins, Enemy[CurrentSinner].Skillbase, Enemy[CurrentSinner].SkillcoinPow, &Sinner[CurrentSinner].Health);
        }
        else{ //Sinner is going to attack unopposed
            UnopposedAtk(Sinner[CurrentSinner].coins, Sinner[CurrentSinner].Skillbase, Sinner[CurrentSinner].SkillcoinPow, &Enemy[CurrentSinner].Health);
        }
        TurnStart++;
    }
    else
    {
        SkillSprites = C2D_SpriteSheetCount(menuSpriteSheet/*PLACEHOLDER*/);

        CurrentTimeMs = osGetTime();
        ElapsedTimeMs = (CurrentTimeMs - InitialTimeMs);
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
	    C2D_SceneBegin(top);

	    C3D_FrameEnd(0);
    }
        CurrentSinner++; //cycle through each sinner and clashing or going unopposed then go to the next one. Does this 5 times
        
    if(Enemy[4].Health < 0){
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(bottom, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
	    C2D_SceneBegin(bottom);
        C2D_DrawText(&staticTex[0], 0, 8.0f, 8.0f, 0.5f, 0.0f, 1.0f);
	    C3D_FrameEnd(0);
        MenuPosition = 1;
    }

    if(CurrentSinner == 5/*All sinners have completed their actions*/){
          //reset to first sinner
        CurrentSinner = 0;
        //End this turn and start the next one
        TurnStart = 0;
        CreatedSkillStores = !CreatedSkillStores;
        TurnCount++; 
    }
    break; //Leave combat code zone

} 
    
    gfxFlushBuffers();
    gfxSwapBuffers();
    gspWaitForVBlank();
}
C2D_SpriteSheetFree(menuSpriteSheet);
C2D_TextBufDelete(staticBuf);
C2D_TextBufDelete(dynamBuf);
ExitApp();
return 0; 
}