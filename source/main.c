#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <stdbool.h>
#include "Skill.h"
#include "CombatFunctions.h"
#include "MenuSelec.h"
#include "Sinner_Enemy_defin.h"

#define MAX_SPRITES 768

#define StartScreen 0
#define MainMenu 1
#define CombatMenu 2

//Spritesheets
static C2D_SpriteSheet menuSpriteSheet;

//Text and text buffers
C2D_TextBuf staticBuf;
C2D_TextBuf dynamBuf;
C2D_Text staticTex[2];

typedef struct
{
	C2D_Sprite spr;
	float dx, dy; // velocity
} Sprite;

static Sprite Msprites[MAX_SPRITES];

//sprite animation example from http://www.nyankolab.com/
static const u64 GFXRefreshMs = 33/*ms*/; //refresh graphics 30 times a second for 30fps

void ExitApp(){
C2D_Fini();
C3D_Fini();
romfsExit();
gfxExit();
}

void SinnerTex(Characters Sinner[5], C2D_TextBuf dynamBuf, float xPosHP, float yPosHP, float xPosSP, float yPosSP)
{
    //uses 3ds/graphics/printing/system-font example
    C2D_TextBufClear(dynamBuf); //clear previous text
    char HpBuf[256];
    char Sanbuf[256];
    C2D_Text dynamTex[2];

    snprintf(HpBuf, sizeof(HpBuf), "Health: %lf  %lf  %lf  %lf  %lf", \
     Sinner[0].Health, Sinner[1].Health, Sinner[2].Health, Sinner[3].Health, Sinner[4].Health);
    
    snprintf(Sanbuf, sizeof(Sanbuf), "Sanity: %d  %d  %d  %d  %d", \
     Sinner[0].Sanity, Sinner[1].Sanity, Sinner[2].Sanity, Sinner[3].Sanity, Sinner[4].Sanity); //write to buffer

    C2D_TextParse(&dynamTex[0], dynamBuf, HpBuf); //parse the formatted strings
    C2D_TextParse(&dynamTex[1], dynamBuf, Sanbuf);
    C2D_TextOptimize(&dynamTex[0]);
    C2D_TextOptimize(&dynamTex[1]);
    C2D_DrawText(&dynamTex[0], 0, xPosHP, yPosHP, 0.0f, 1.0f, 1.0f);
    C2D_DrawText(&dynamTex[1], 0, xPosSP, yPosSP, 0.0f, 1.0f, 1.0f);
}

int main(int argc, char **argv){  // initialise variables
gfxInitDefault();
consoleInit(GFX_BOTTOM, NULL);
romfsInit();
C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
C2D_Prepare();
//placeholder stats till i can read files for values in a json or other c file
Characters Sinner[5] = {{195.0f, 0.0, 2, 4, 4, 50, 2, 4, 4}, \
                        {0.0f, 0.0, 0, 0, 0, 50, 0, 0, 0}, \
                        {0.0f, 0.0, 0, 0, 0, 50, 0, 0, 0}, \
                        {0.0f, 0.0, 0, 0, 0, 50, 0, 0, 0}, \
                        {0.0f, 0.0, 0, 0, 0, 50, 0, 0, 0}};                            
Characters Enemy[5] = {{1560.0f, 0, 2, 4, 2, 50, 2, 4, 2}, \
                       {1560.0f, 0, 2, 4, 2, 50, 2, 4, 2}, \
                       {1560.0f, 0, 2, 4, 2, 50, 2, 4, 2}, \
                       {1560.0f, 0, 2, 4, 2, 50, 2, 4, 2}, \
                       {1560.0f, 0, 2, 4, 2, 50, 2, 4, 2}};

ClashParams SkillPosInfo[5] = {{0, 0, false, false}, {0, 0, false, false}, {0, 0, false, false}, {0, 0, false, false}, {0, 0, false, false}};

u64 InitialTimeMs = 0;
u64 CurrentTimeMs = 0;
u64 ElapsedTimeMs = 0;

int CurrentFrameIndex = 0;
size_t SkillSprites = 0;

int AttackOrder[5][2] = {{0, NOTSELECTED/* = 6*/}, {0, 6}, {0, 6}, {0, 6}, {0, }};  //Each element is assigned a index based on ther skill selected to attack the Character array above
int EnSkillOrder[5][2] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}}; //skill number/order for main boss, second dimension is used to find the index for AtkOrder
int SkillPriorityLevel[5] = {0};                                   //higher priority means skill will clash over other skills
int SkillOptions[5][2] = {{0, 0},{0, 0},{0, 0},{0, 0},{0, 0}};     //skill numbers for each skill slot for any amount for sinners
int BufferSkill[5] = {0, 0, 0, 0, 0};                              // original order before skills will be randomised and listed / picked from
int SkillList[6] = {1, 1, 1, 2, 2, 3};                             //Sinners can only have three skill 1s, two skill 2s and , one skill 3
int SelectlotNum[5] = {0};
int CurrentSinner = 0;
int CurrSinTOChooseSkill = NOTSELECTED;
int Clashes[5] = {0};
int TurnCount = 1;
int MenuPosition = StartScreen;
int TurnStart = 0;
int InCombatOrGFX = 0; //1: combat clashing logic, 2: GFX of clashes
bool SelectSlotAppeared[5] = {false, false, false, false, false};
bool CreatedSkillStores = false;
bool BeganSelec = false;
bool SkillTargetingLocked = false;

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
    u32 kHeld = hidKeysHeld();
    if(kDown & KEY_START) break;
    touchPosition touch;
    hidTouchRead(&touch);

switch(MenuPosition){ // In game start

    case StartScreen: //Start screen
    if(MenuPosition == StartScreen)
    { //Prevent redrawing the startmenu sprites when changing menu position
    C2D_SpriteSetPos(&Msprites[0].spr, 30, 20); //Move sprite to center view
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
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(bottom, C2D_Color32(0x82, 0x14, 0x00, 0xFF));
        C2D_SceneBegin(bottom);
    //(Should Draw / Make menu) - unfinished
    if(CreatedSkillStores == false)
    {
    CreatedSkillStores = CreateSkillStores(SkillOptions, EnSkillOrder, BufferSkill, SkillList, TurnCount); //When completed returns true / 1
    }
    Sinner[0].OldHealth = Sinner[0].Health;
    Enemy[0].OldHealth = Enemy[0].Health;

    if(kHeld & KEY_TOUCH){
        CurrSinTOChooseSkill = BeginSinSelec(touch.px, touch.py, CurrSinTOChooseSkill, &SkillTargetingLocked, &BeganSelec);
    }
    else
    {
        SkillTargetingLocked = false;
        BeganSelec = false;
    }
    if(kHeld & KEY_TOUCH && BeganSelec)
    {
        AttackOrder[CurrSinTOChooseSkill][1] = CursorToEN_Skill(touch.px, touch.py);
    }

    SinnerTex(Sinner, dynamBuf, 8.0f, 8.0f, 8.0f, 12.0f);
    
    if(CreatedSkillStores == true && kDown & KEY_L && InCombatOrGFX == 0) //Prevent abrupt cancels
    {
        InCombatOrGFX = 1; //combat
        for(int Search = 0; Search < 5; Search++){
            //check if clashing
            if(AttackOrder[Search][CurrentIndex] == EnSkillOrder[Search][CurrentIndex])
            {
                SkillPosInfo[Search].IsClashing = true;
                SkillPosInfo[Search].SkillClashing = Search;
                SelectSlotAppeared[AttackOrder[Search][CurrentIndex]] = true; // skill is targeting a slot
                SkillPriorityLevel[AttackOrder[Search][CurrentIndex]] = AttackOrder[Search][CurrentIndex]; //record what skill slot was targeted
            }
            //check if skill is going unopposed while another skill clashes the same slot
            if(SelectSlotAppeared[AttackOrder[Search][CurrentIndex]] == true)
            {
                SkillPosInfo[Search].IsClashing = ComparePriority(SkillPriorityLevel[Search], SkillPriorityLevel[AttackOrder[Search][CurrentIndex]]);
                if(SkillPosInfo[Search].IsClashing == true)
                {
                    SkillPosInfo[AttackOrder[Search][CurrentIndex]].IsClashing = false;
                }
                // reset check bool
                SelectSlotAppeared[AttackOrder[Search][CurrentIndex]] = false;
            }
            //check if enemy attacks wil go unopposed, no sinner is clashing the slot
            if(EnSkillOrder[Search][CurrentIndex] != AttackOrder[0][CurrentIndex] || \
                EnSkillOrder[Search][CurrentIndex] != AttackOrder[1][CurrentIndex] || \
                EnSkillOrder[Search][CurrentIndex] != AttackOrder[2][CurrentIndex] || \
                EnSkillOrder[Search][CurrentIndex] != AttackOrder[3][CurrentIndex] || \
                EnSkillOrder[Search][CurrentIndex] != AttackOrder[4][CurrentIndex])
                {
                    SkillPosInfo[Search].IsUnclashed = true;
                }
        }
    }

    switch(InCombatOrGFX){

        case 1: // Turn Running loop -> Clashing
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
        Clashes[CurrentSinner] = ClashingAtk(&Sinner[CurrentSinner].Sanity, &Enemy[CurrentSinner].Sanity, \
                                             &Sinner[CurrentSinner].coins, &Enemy[CurrentSinner].coins, \
                                             Sinner[CurrentSinner].Skillbase, Enemy[CurrentSinner].Skillbase, \
                                             Sinner[CurrentSinner].SkillcoinPow, Enemy[CurrentSinner].SkillcoinPow, \
                                             &Sinner[CurrentSinner].Health, &Enemy[CurrentSinner].Health);
        }
        else if(SkillPosInfo[CurrentSinner].IsUnclashed == true && SkillPosInfo[CurrentSinner].IsClashing == false){ //Enemy is going to attack unopposed
            UnopposedAtk(Enemy[CurrentSinner].coins, Enemy[CurrentSinner].Skillbase, Enemy[CurrentSinner].SkillcoinPow, &Sinner[CurrentSinner].Health);
        }
        else{ //Sinner is going to attack unopposed
            UnopposedAtk(Sinner[CurrentSinner].coins, Sinner[CurrentSinner].Skillbase, Sinner[CurrentSinner].SkillcoinPow, &Enemy[CurrentSinner].Health);
        }
        InCombatOrGFX = 2; //GFX
        break;

        case 2: //GFX of the clash and combat
        SkillSprites = C2D_SpriteSheetCount(menuSpriteSheet/*PLACEHOLDER*/);

        CurrentTimeMs = osGetTime();
        ElapsedTimeMs += (CurrentTimeMs - InitialTimeMs);
        C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
        C2D_SceneBegin(top);
        if(ElapsedTimeMs >= GFXRefreshMs)
        {
            ElapsedTimeMs -= GFXRefreshMs; //reset elapsed time
            //draw current frame index of the animation
            //increase to the next index
            InitialTimeMs = osGetTime(); //set new initial time
        }
        else
        {
            //draw current frame index of the animation
        }
        CurrentSinner++; //cycle through each sinner and clashing or going unopposed then go to the next one. Does this 5 times}
        InCombatOrGFX = 0;
        break;
    }
    
    
    if(Enemy[4].Health < 0){
        C2D_TargetClear(bottom, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
	    C2D_SceneBegin(bottom);
        C2D_DrawText(&staticTex[0], 0, 8.0f, 8.0f, 0.5f, 0.0f, 1.0f);
        MenuPosition = MainMenu;
        C3D_FrameEnd(0);
        break;
    }

    if(CurrentSinner == 5/*All sinners have completed their actions*/){
        //reset to first sinner
        CurrentSinner = 0;
        //End this turn and start the next one
        
        CreatedSkillStores = false;
        TurnCount++;
    }
        C3D_FrameEnd(0);
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