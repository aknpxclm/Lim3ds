#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <stdbool.h>
#include "Sinner_Enemy_defin.h"
#include "Skill.h"
#include "CombatFunctions.h"

#define MAX_SPRITES 768

#define StartScreen 0
#define MainMenu 1
#define CombatMenu 2

#define NOTSELECTED 9

//Spritesheets
static C2D_SpriteSheet menuSpriteSheet;

C2D_TextBuf dynamBuf;

typedef struct
{
	C2D_Sprite spr;
	float dx, dy; // velocity
} Sprite;

static Sprite Sprites[MAX_SPRITES];

//sprite animation example from http://www.nyankolab.com/
static u64 GFXRefreshMs = 33/*ms*/; //refresh graphics 30 times a second for 30fps

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

int BeginSinSelec(int TOUCHx, int TOUCHy, int CurrSinTOChooseSkill, bool *SkillTargetingLocked, bool *BeganSelec)
{
    if(*SkillTargetingLocked) return CurrSinTOChooseSkill;
        /*return current skill index that the useer is choosing to clash a skill with;
        ignoring touch pos until, touch screen is listed or skills are selected to clash*/
    if(TOUCHx <= 24 && TOUCHx >= 48 && TOUCHy <= 24 && TOUCHy >= 48)
    {
        *SkillTargetingLocked = true;
        *BeganSelec = true;
        return 0; //slot 1
    }
    else if(TOUCHx <= 96 && TOUCHx >= 120 && TOUCHy <= 216 && TOUCHy >= 230)
    {
        *SkillTargetingLocked = true;
        *BeganSelec = true;
        return 1; //slot 2
    }
    else if(TOUCHx <= 168 && TOUCHx >= 192 && TOUCHy <= 216 && TOUCHy >= 230)
    {
        *SkillTargetingLocked = true;
        *BeganSelec = true;
        return 2; //slot 3
    }
    else if(TOUCHx <= 216 && TOUCHx >= 240 && TOUCHy <= 216 && TOUCHy >= 230)
    {
        *SkillTargetingLocked = true;
        *BeganSelec = true;
        return 3; //slot 4
    }
    else if(TOUCHx <= 284 && TOUCHx >= 308 && TOUCHy <= 216 && TOUCHy >= 230)
    {
        *SkillTargetingLocked = true;
        *BeganSelec = true;
        return 4; //slot 5
    }
    else
    {
        *BeganSelec = false;
        return NOTSELECTED;
    }
}

int CursorToEN_Skill(int TOUCHx, int TOUCHy)
{
    // x & y are assuming that the hidtouch function are based on pixel coordinates
    if(TOUCHx <= 24 && TOUCHx >= 48 && TOUCHy <= 24 && TOUCHy >= 48) return 0; //slot 1
    else if(TOUCHx <= 96 && TOUCHx >= 120 && TOUCHy <= 24 && TOUCHy >= 48) return 1; //slot 2
    else if(TOUCHx <= 168 && TOUCHx >= 192 && TOUCHy <= 24 && TOUCHy >= 48) return 2; //slot 3
    else if(TOUCHx <= 216 && TOUCHx >= 240 && TOUCHy <= 24 && TOUCHy >= 48) return 3; //slot 4
    else if(TOUCHx <= 284 && TOUCHx >= 308 && TOUCHy <= 24 && TOUCHy >= 48) return 4; //slot 5
    else return NOTSELECTED; //not selecting
}

void SetUpBoss(SkillInfo Enskill[][3], bool BossOrMultipleEnemy){ //true if there will be a boss
if(BossOrMultipleEnemy){
    int coin = 0;
    int base = 0;
    int coinpow = 0;
    for(int i = 0; i < 3; i++){
        coin = Enskill[0][i].coins;
        base = Enskill[0][i].Skillbase;
        coinpow = Enskill[0][i].SkillcoinPow;
        for(int j = 1; j < 5; j++){
            Enskill[j][i].coins = coin;
            Enskill[j][i].Skillbase = base;
            Enskill[j][i].SkillcoinPow = coinpow;
        }
    }
}
}

int main(int argc, char **argv){  // initialise variables
gfxInitDefault();
romfsInit();
C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
C2D_Prepare();

//placeholder stats till i can read files for values in a json or other c file
Characters Sinner[5] = {{195.0f, 0.0, 2, 4, 4, 50}, {0.0f, 0.0, 0, 0, 0, 50}, {0.0f, 0.0, 0, 0, 0, 50}, {0.0f, 0.0, 0, 0, 0, 50}, {0.0f, 0.0, 0, 0, 0, 50}};                            
Characters Enemy[5] = {{1560.0f, 0, 2, 4, 2, 50}, {1560.0f, 0, 2, 4, 2, 50}, {1560.0f, 0, 2, 4, 2, 50}, {1560.0f, 0, 2, 4, 2, 50}, {1560.0f, 0, 2, 4, 2, 50}};
//Skill info for each sinner's skill ranks
SkillInfo SinSkill[5][3] = {{{2, 4, 4}, {3, 4, 4}, {4, 4, 3}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}};
SkillInfo EnSkill[5][3] = {{{2, 4, 2}, {3, 3, 3}, {1, 8, 12}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}};

int EnSkillPattern[5] = {2, 2, 1, 1, 1};

ClashParams SkillPosInfo[5] = {{0, 0, false, false}, {0, 0, false, false}, {0, 0, false, false}, {0, 0, false, false}, {0, 0, false, false}};

u64 InitialTimeMs = 0;
u64 CurrentTimeMs = 0;
u64 ElapsedTimeMs = 0;

int CurrentFrameIndex = 0;
size_t SkillSprites = 0;

int AttackOrder[5][2] = {{0/*Skill rank to load and clash*/, NOTSELECTED/* = 6*/}, {0, 6}, {0, 6}, {0, 6}, {0, 6}};  //Each element is assigned a index based on ther skill selected to attack the Character array above
int EnSkillOrder[5][2] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}}; //skill number/order for main boss, second dimension is used to find the index for AtkOrder
int SkillPriorityLevel[5] = {0};                                   //higher priority means skill will clash over other skills
int SkillOptions[5][2] = {{0, 0},{0, 0},{0, 0},{0, 0},{0, 0}};     //skill numbers for each skill slot for any amount for sinners
int BufferSkill[5] = {0, 0, 0, 0, 0};                              // original order before skills will be randomised and listed / picked from
int SkillList[6] = {1, 1, 1, 2, 2, 3};                             //Sinners can only have three skill 1s, two skill 2s and , one skill 3
uint16_t CurrentSinner = 0;
uint16_t CurrSinTOChooseSkill = NOTSELECTED;
uint16_t Clashes = 0; //max 255 which should be enough for these variables
uint16_t TurnCount = 1;
int MenuPosition = StartScreen;
uint16_t InCombatOrGFX = 0; //1: combat clashing logic, 2: GFX of clashes
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
dynamBuf = C2D_TextBufNew(4096);

menuSpriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/menu.t3x");
if (!menuSpriteSheet) svcBreak(USERBREAK_PANIC);

//Initialise all sprites in a sheet
//size_t numsprites = C2D_SpriteSheetCount(menuSpriteSheet);
//for(int x = 0; x < numsprites; x++){
    Sprite *Menusprite = &Sprites[0];
    C2D_SpriteFromSheet(&Menusprite->spr, menuSpriteSheet, 0/*sprite index in the sheet*/);
    C2D_SpriteSetCenter(&Menusprite->spr, 0.1f, 0.1f);
    C2D_SpriteSetPos(&Menusprite->spr, 0/*X position*/, 0/*Y position*/);
    C2D_SpriteSetRotation(&Menusprite->spr, 0);
    C2D_SpriteSetScale(&Menusprite->spr, 1/*X scale*/, 1/*Y scale*/);
//}

while(aptMainLoop()){

    hidScanInput(); //Scans for keys pressed
    uint32_t kDown = hidKeysDown();
    uint32_t kHeld = hidKeysHeld();
    if(kDown & KEY_START) break;
    touchPosition touch;
    hidTouchRead(&touch);

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

switch(MenuPosition){ // In game start

    case StartScreen: //Start screen
    C2D_SpriteSetPos(&Sprites[0].spr, 30, 20); //Move sprite to center view
	C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
	C2D_SceneBegin(top);
	C2D_DrawSprite(&Sprites[0].spr); //Draw Start_Screen.png
	C3D_FrameEnd(0);
    if(kDown & KEY_TOUCH) {
        C2D_SpriteSheetFree(menuSpriteSheet);
        MenuPosition = MainMenu;
    }
        //Switch to MainMenu
    break;


    case MainMenu: //Main menu
	C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 0.65f)); //Gray background
	C2D_SceneBegin(top);
    SetUpBoss(EnSkill, true);
    if(touch.px/*pixel coordinate of x on the screen?*/ >= 288 && touch.px <= 736/*X area of detection*/ && touch.py >= 168 && touch.py <= 336 /*Y area of detection*/&& kDown & KEY_TOUCH)
    {
        // if touchpad is pressed in the detection area...
        MenuPosition = CombatMenu;
    }
    //more options in the menu will get their own condition
    break;
       
    
    case CombatMenu: //Combat select area
        C2D_TargetClear(bottom, C2D_Color32(0x82, 0x14, 0x00, 0xFF));
        C2D_SceneBegin(bottom);
    //(Should Draw / Make menu) - unfinished
    if(InCombatOrGFX > 0)
    {
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
        AttackOrder[CurrSinTOChooseSkill][0] = CursorToEN_Skill(touch.px, touch.py);
    }

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
                //Check if other skill has the higher pirority and remove them from clashing if it is lower
                if(SkillPosInfo[Search].IsClashing)
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
    }
    
    SinnerTex(Sinner, dynamBuf, 8.0f, 8.0f, 8.0f, 12.0f);
    
    switch(InCombatOrGFX){

        case 1: // Turn Running loop -> Clashing
        if(CurrentSinner > 0){ //solo sinner for now
            Enemy[CurrentSinner].Health = Enemy[CurrentSinner - 1].Health;
            Enemy[CurrentSinner].Sanity = Enemy[CurrentSinner - 1].Sanity;
        }

        Sinner[CurrentSinner].coins = SinSkill[CurrentSinner][AttackOrder[CurrentSinner][1]].coins;
        Sinner[CurrentSinner].Skillbase = SinSkill[CurrentSinner][AttackOrder[CurrentSinner][1]].Skillbase;
        Sinner[CurrentSinner].SkillcoinPow = SinSkill[CurrentSinner][AttackOrder[CurrentSinner][1]].SkillcoinPow;

        Enemy[CurrentSinner].coins = EnSkill[CurrentSinner][EnSkillPattern[CurrentSinner]].coins;
        Enemy[CurrentSinner].Skillbase = EnSkill[CurrentSinner][EnSkillPattern[CurrentSinner]].Skillbase;
        Enemy[CurrentSinner].SkillcoinPow = EnSkill[CurrentSinner][EnSkillPattern[CurrentSinner]].SkillcoinPow;

        //Holy arguements
        if(SkillPosInfo[CurrentSinner].IsClashing == true && SkillPosInfo[CurrentSinner].IsUnclashed == false){ //Enemy and sinner clash skills, returns the amount of clashes between the skills
        Clashes = ClashingAtk(&Sinner[CurrentSinner].Sanity, &Enemy[CurrentSinner].Sanity, &Sinner[CurrentSinner].coins, &Enemy[CurrentSinner].coins, \
                              Sinner[CurrentSinner].Skillbase, Enemy[CurrentSinner].Skillbase, Sinner[CurrentSinner].SkillcoinPow, Enemy[CurrentSinner].SkillcoinPow, \
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
        SkillSprites = C2D_SpriteSheetCount(menuSpriteSheet/*PLACEHOLDER*/); //load winning character's sprite animation

        CurrentTimeMs = osGetTime();
        ElapsedTimeMs += (CurrentTimeMs - InitialTimeMs);
        C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
        C2D_SceneBegin(top);
        if(ElapsedTimeMs >= GFXRefreshMs)
        {
            ElapsedTimeMs -= GFXRefreshMs; //reset elapsed time
            //draw current frame index of the animation
            if(CurrentFrameIndex != SkillSprites) CurrentFrameIndex++;
            InitialTimeMs = osGetTime(); //set new initial time
        }
        else
        {
            //draw current frame index of the animation
        }
        if(CurrentFrameIndex == SkillSprites){
        CurrentFrameIndex = 0;
        InCombatOrGFX = 1;
        CurrentSinner++; //cycle through each sinner and clashing or going unopposed then go to the next one. Does this 5 times}
        }
        break;
    }
    
    if(Enemy[4].Health < 0){
        MenuPosition = MainMenu;
        goto exit;
        break;
    }

    if(CurrentSinner == 5/*All sinners have completed their actions*/){
        InCombatOrGFX = 0; //exit clash and GFX
        CurrentSinner = 0; //reset to first sinner
        //End this turn and start the next one
        CreatedSkillStores = false;
        TurnCount++;
    }
    break; //Leave combat code zone

}
    C3D_FrameEnd(0);
    gfxFlushBuffers();
    gfxSwapBuffers();
    gspWaitForVBlank();
}
exit:
C2D_TextBufDelete(dynamBuf);
ExitApp();
return 0; 
}