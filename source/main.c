#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <stdbool.h>
#include "Sinner_Enemy_defin.h"
#include "Skill.h"
#include "LoadCharac.h"
#include "CombatFunctions.h"
#include "SlotSelect.h"
#include "LobbyRend.h"
#include "CombatTex.h"
#include "CombSpriteRen.h"

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

#define NOTSELECTED 9

void ExitApp(){
FreeMain_M();
FreeTexBuf();
gfxExit();
romfsExit();
C2D_Fini();
C3D_Fini();
}

int main(int argc, char **argv){  // initialise variables
gfxInitDefault();
romfsInit();
C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
C2D_Prepare();

//placeholder stats till i can read files for values in a json or other c file
Characters Sinner[5] = {{195.0f, 0.0, 0, 0, 0, 50, 1}, \
                        {195.0f, 0.0, 0, 0, 0, 50, 1},   \
                        {195.0f, 0.0, 0, 0, 0, 50, 1},   \
                        {195.0f, 0.0, 0, 0, 0, 50, 1},   \
                        {195.0f, 0.0, 0, 0, 0, 50, 1}};                          
Characters Enemy[5] = {{1560.0f, 0, 0, 0, 0, 50, 1}, \
                       {1560.0f, 0, 0, 0, 0, 50, 1}, \
                       {1560.0f, 0, 0, 0, 0, 50, 1}, \
                       {1560.0f, 0, 0, 0, 0, 50, 1}, \
                       {1560.0f, 0, 0, 0, 0, 50, 1}};
//Skill info for each sinner's skill ranks
SkillInfo SinSkill[5][3] = {{{2, 4, 4}, {3, 4, 4}, {4, 4, 3}}, \
                            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, \
                            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, \
                            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, \
                            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}};
SkillInfo EnSkill[5][3] = {{{2, 4, 2}, {3, 3, 3}, {1, 8, 12}}, \
                           {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, \
                           {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, \
                           {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, \
                           {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}};

ClashParams SkillPosInfo[5] = {{0, 0, false, false, false}, {0, 0, false, false, false}, {0, 0, false, false, false}, {0, 0, false, false, false}, {0, 0, false, false, false}};

char *LoadPath;

int AttackOrder[5][2] = {{0/*Skill rank to load and clash*/, NOTSELECTED/* = 9*/}, {0, 9}, {0, 9}, {0, 9}, {0, 9}}; 
int EnSkillOrder[5][2] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}}; //skill number/order for main boss, second dimension is used to find the index for AtkOrder
int SkillOptions[5][2] = {{0, 0},{0, 0},{0, 0},{0, 0},{0, 0}};     //skill numbers for each skill slot for any amount for sinners
int BufferSkill[5] = {0, 0, 0, 0, 0};                              // original order before skills will be randomised and listed / picked from
int SkillList[6] = {1, 1, 1, 2, 2, 3};                             //Sinners can only have three skill 1s, two skill 2s and , one skill 3

int EnSkillPattern[5] = {2, 2, 1, 1, 1};

int IdToload = 0;
int TotalSinIdsInGame = 1; //total unique identities that can be loaded into a sinner slot (5)

size_t SkillSprites = 0;

Time Time_T = {0, 0, 0};

u32 CurrentFrameIndex = 0;
u32 TurnCount = 1;

u16 CurrentSinner = 0;
u16 CurrSinTOChooseSkill = NOTSELECTED;
u16 Clashes = 0; //max 255 which should be enough for these variables

u8 CursorOn_X_Sinner = 0;
u8 MenuPosition = 0;
u8 MainSubPos = 0;
u8 InCombatOrGFX = 0; //0: idle animation 1: combat clashing logic, 2: GFX of clashes
u8 IdleIndex[2] = {0, 0};
u8 IdleMax[2] = {0, 0};

bool PriGivenAlredy[5] = {false, false, false, false, false};
bool CreatedSkillStores = false;
bool StartSelec = false;
bool SkillTargetingLocked = false;

bool UserInDeepSelect = false;

//Create 3ds Render targets for the screens
C3D_RenderTarget *top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
C3D_RenderTarget *bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

Time_T.InitialTimeMs = osGetTime();

SeedStart();
Rearrange_SkillPool(SkillList); //Moves the values in SkillList[] (L98) to a random position

InitMain_M();
LoadPath = AllocPathBuf();

while(aptMainLoop()){

    hidScanInput(); //Scans for keys pressed
    u32 kDown = hidKeysDown();
    u32 kHeld = hidKeysHeld();
    u32 kUp = hidKeysUp();
    if(kDown & KEY_START) break;
    touchPosition touch;
    hidTouchRead(&touch);

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

switch(MenuPosition){ // In game start

    case StartMen: //Start screen
        DrawMain_S(top, bottom, MenuPosition);
        if(kDown) MenuPosition = MainMen;
    break;


    case MainMen: //Main menu
	DrawMain_S(top, bottom, MenuPosition);
    if(!UserInDeepSelect)
    {
        if(kDown & KEY_DRIGHT && MainSubPos <= 2) MainSubPos += 1;
        if(kDown & KEY_DLEFT && MainSubPos > 0) MainSubPos -= 1;
    }

    switch(MainSubPos)
    {
        case 0:
        break;

        case 1: //team select
        if(kDown & KEY_A) UserInDeepSelect = true; //enter id select
        if(kDown & KEY_B) UserInDeepSelect = false;
        if(UserInDeepSelect == true)
        {
            if(kDown & KEY_DRIGHT && CursorOn_X_Sinner < 4) CursorOn_X_Sinner += 1;
            if(kDown & KEY_DLEFT && CursorOn_X_Sinner > 0) CursorOn_X_Sinner -= 1;
            if(kDown & KEY_DUP && IdToload < TotalSinIdsInGame) IdToload += 1;
            if(kDown & KEY_DDOWN && IdToload > 0) IdToload -= 1;

            if(kUp & KEY_X)
            {
                CharIdPath(IdToload, LoadPath);
                LoadSinInfo(&SinSkill[CursorOn_X_Sinner], LoadPath);
                IdToload = 0;
            }
        }
        break;

        case 2:
        break;
    }

    if(kDown & KEY_TOUCH){
        if(touch.px/*pixel coordinate of x on the screen?*/ >= 288 && touch.px <= 736/*X area of detection*/ && touch.py >= 168 && touch.py <= 336 /*Y area of detection*/)
        {
        // if touchpad is pressed in the detection area...
        SetUpBoss(EnSkill, true);
        MenuPosition = CombatMen;
        FreeMain_M();
        }
    }
    break;
       
    
    case CombatMen: //Combat select area
        C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
        C2D_TargetClear(bottom, C2D_Color32(0x82, 0x14, 0x00, 0xFF));
    //(Should Draw / Make menu) - unfinished
    if(InCombatOrGFX == 0)
    {
        if(CreatedSkillStores == false)
        {
            CreatedSkillStores = CreateSkillStores(SkillOptions, EnSkillOrder, BufferSkill, SkillList, TurnCount); //When completed returns true / 1
        }
        Sinner[0].OldHealth = Sinner[0].Health;
        Enemy[0].OldHealth = Enemy[0].Health;

        if(kHeld & KEY_TOUCH){
            CurrSinTOChooseSkill = BeginSinSelect(touch.px, touch.py, CurrSinTOChooseSkill, &SkillTargetingLocked, &StartSelec);
        }
        else{ SkillTargetingLocked = false; StartSelec = false; }
        if(kHeld & KEY_TOUCH && StartSelec)
        {
            AttackOrder[CurrSinTOChooseSkill][0] = CursorToEN_Skill(touch.px, touch.py);
        }
        if(kUp & KEY_TOUCH) //if the user released from the touch pad recently
        {
            if(CurrSinTOChooseSkill != 9)
            {
                if(PriGivenAlredy[CurrSinTOChooseSkill] == false)
                {
                    PriGivenAlredy[CurrSinTOChooseSkill] = true;
                    SkillPosInfo[CurrSinTOChooseSkill].Priority += 1; //if first time slot selected, increase by one
                }
                else SkillPosInfo[CurrSinTOChooseSkill].Priority += 2;
                CurrSinTOChooseSkill = NOTSELECTED;
            }
            
        }

        if(CreatedSkillStores == true && kDown & KEY_L && InCombatOrGFX == 0) //Prevent abrupt cancels
        {
            InCombatOrGFX = Combat;
            DetermineClashAtkType(AttackOrder, EnSkillOrder, SkillPosInfo);
        }
    }

    C2D_SceneBegin(bottom);
    SinnerTex(Sinner, 8.0f, 8.0f, 8.0f, 12.0f);

    switch(InCombatOrGFX){

        case GFX: //idle animations
        RenderingCombat_S(top, &Time_T, &SkillSprites, &CurrentFrameIndex, &CurrentSinner, IdleIndex, IdleMax, &InCombatOrGFX);
        break;

        case Combat: // Turn Running loop -> Clashing
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

        if(SkillPosInfo[CurrentSinner].IsClashing == true && SkillPosInfo[CurrentSinner].IsUnclashed == false){ //Enemy and sinner clash skills, returns the amount of clashes between the skills
            Clashes = ClashingAtk(&Sinner[CurrentSinner], &Enemy[SkillPosInfo[CurrentSinner].SkillClashing]);
        }
        else if(SkillPosInfo[CurrentSinner].IsUnclashed == true && SkillPosInfo[CurrentSinner].IsClashing == false){ //Enemy is going to attack unopposed
           UnopposedAtk(&Enemy[SkillPosInfo[CurrentSinner].SkillClashing], &Sinner[CurrentSinner]);
        }
        else{ //Sinner is going to attack unopposed
            UnopposedAtk(&Sinner[CurrentSinner], &Enemy[SkillPosInfo[CurrentSinner].SkillClashing]);
        }
        InCombatOrGFX = CombatGFX;
        break;

        case CombatGFX: //GFX of the clash and combat
        RenderingCombat_S(top, &Time_T, &SkillSprites, &CurrentFrameIndex, &CurrentSinner, IdleIndex, IdleMax, &InCombatOrGFX);
        break;

    }
    
    if(Enemy[4].Health < 0){
        MenuPosition = MainMen;
        InitMain_M(); //reload menu sprites when returning to main menu
        break;
    }

    if(CurrentSinner == 5/*All sinners have completed their actions*/){
        InCombatOrGFX = GFX; //exit clash and GFX
        CurrentSinner = 0; //reset to first sinner
        //End this turn and start the next one
        CreatedSkillStores = false;
        TurnCount++;
    }
    break; //Leave combat code zone

}
    C3D_FrameEnd(0);
}
FreePath(LoadPath);
ExitApp();
return 0; 
}