#include <3ds.h>
#include <citro2d.h>
#include <stdbool.h>
#include "Sinner_Enemy_defin.h"
#include "CombatFunctions.h"
#include "LoadCharac.h"
#include "LobbyRend.h"

//max sprites is 768
typedef struct
{
	C2D_Sprite spr;
	float dx, dy; // velocity
} Sprite;

static C2D_SpriteSheet menuSpriteSheet;
static Sprite MenuSprites[20];

static C2D_ImageTint DarkenBGtint;

void InitMain_M()
{
    C2D_PlainImageTint(&DarkenBGtint, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f), 0.425f);

    size_t NumMenuSpr = 0;
    menuSpriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/menu.t3x");
    if (!menuSpriteSheet) svcBreak(USERBREAK_PANIC);

    NumMenuSpr = C2D_SpriteSheetCount(menuSpriteSheet);

    for(int x = 0; x < NumMenuSpr; x++){
        Sprite *Menusprite = &MenuSprites[x];
        C2D_SpriteFromSheet(&Menusprite->spr, menuSpriteSheet, x/*sprite index in the sheet*/);
        C2D_SpriteSetCenter(&Menusprite->spr, 0.1f, 0.1f);
        C2D_SpriteSetPos(&Menusprite->spr, 30/*X position*/, 20/*Y position*/);
        C2D_SpriteSetRotation(&Menusprite->spr, 0);
        C2D_SpriteSetScale(&Menusprite->spr, 1/*X scale*/, 1/*Y scale*/);
    }
    C2D_SpriteSetPos(&MenuSprites[LobbyBot].spr, -10, 20); //set bottom lobby png pos
}

void DrawMain_S(C3D_RenderTarget *top, C3D_RenderTarget *bottom, u8 MenuPosition, u8 *TintBG)
{
    C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
    C2D_TargetClear(bottom, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f)); //Looked at NateXS' pong repo for proper usage of the function
    switch(MenuPosition)
    {
        case StartMen:
        C2D_SceneBegin(top);
        C2D_DrawSprite(&MenuSprites[Loading].spr); //"Loading screen"
        break;

        case MainMen:
        C2D_SceneBegin(top);
        if(*TintBG) C2D_DrawSpriteTinted(&MenuSprites[LobbyTop].spr, &DarkenBGtint);
        else C2D_DrawSprite(&MenuSprites[LobbyTop].spr);
        C2D_SceneBegin(bottom);
        if(*TintBG) C2D_DrawSpriteTinted(&MenuSprites[LobbyBot].spr, &DarkenBGtint);
        else C2D_DrawSprite(&MenuSprites[LobbyBot].spr);
        break;

    }
}

void FreeMain_M()
{
    C2D_SpriteSheetFree(menuSpriteSheet);
}

void SubMain(C3D_RenderTarget *top, C3D_RenderTarget *bottom, u32 kDown, u32 kUp, char *LoadPath, SkillInfo *SkillBuf, SkillInfo SinSkill[][4], SkillInfo Enskill[][4], u8 *MenuPos, u8 *TintBG)
{
    static bool UserInDeepSelect = false;
    static u8 CursorPos = 0;
    static u8 MainSubPos = 0;
    static u16 IdToload = 0;
    static u16 TotalSinIdsInGame = 1; //total unique identities that can be loaded into a sinner slot (5)

    static u64 Init_A_PressMs = 0;
    static u64 NextaPressMs = 0;
    const u64 PressDelayMs = 20;

    if(kDown & KEY_A && !UserInDeepSelect)
    {
        UserInDeepSelect = true;
        CursorPos = 0;
        if(MainSubPos == 2) *TintBG = 1;
        Init_A_PressMs = osGetTime();
    }
    if(kDown & KEY_B)
    {
        UserInDeepSelect = false;
        CursorPos = 0;
        if(MainSubPos == 2) *TintBG = 0;
    }
    if(!UserInDeepSelect)
    {
        if(kDown & KEY_R && MainSubPos < 2) MainSubPos += 1;
        if(kDown & KEY_L && MainSubPos > 0) MainSubPos -= 1;
    }
    switch(MainSubPos)
    {
        case 0: //"Tutorial" / Dialogue showcase
        break;

        case 1: //Team Select
        if(UserInDeepSelect == true)
        { //CursorPos represents which sinner its pointing to load
            if(kDown & KEY_DRIGHT && CursorPos < 4) CursorPos += 1;
            if(kDown & KEY_DLEFT && CursorPos > 0) CursorPos -= 1;
            if(kDown & KEY_DUP && IdToload < TotalSinIdsInGame) IdToload += 1;
            if(kDown & KEY_DDOWN && IdToload > 0) IdToload -= 1;

            if(kUp & KEY_X)
            {
                CharIdPath(IdToload, LoadPath);
                LoadSinInfo(SkillBuf, LoadPath);
                PassInSkillInfo(SinSkill, SkillBuf, CursorPos);
                IdToload = 0;
            }
        }
        break;

        case 2: //Stage select
        if(UserInDeepSelect)
        {
            if(kDown & KEY_DRIGHT && CursorPos < 1) CursorPos += 1;
            if(kDown & KEY_DLEFT && CursorPos > 0) CursorPos -= 1;
            if(kDown & KEY_A && CursorPos == 0)
            {
                NextaPressMs = osGetTime();
                if(NextaPressMs > Init_A_PressMs + PressDelayMs)
                {
                SetUpBoss(Enskill, true);
                *MenuPos = CombatMen;
                FreeMain_M();
                }
            }
            //else if(kDown & KEY_A && CursorPos == 1)
        }
            C2D_SceneBegin(top);
            if(CursorPos == 0) C2D_SpriteSetScale(&MenuSprites[BattleStageBack].spr, 0.423, 0.423);
            else C2D_SpriteSetScale(&MenuSprites[BattleStageBack].spr, 0.41, 0.41);
            C2D_SpriteSetPos(&MenuSprites[BattleStageBack].spr, 40, 27);
            C2D_DrawSprite(&MenuSprites[BattleStageBack].spr);

            if(CursorPos == 1) C2D_SpriteSetScale(&MenuSprites[BattleStageBack].spr, 0.423, 0.423);
            else C2D_SpriteSetScale(&MenuSprites[BattleStageBack].spr, 0.41, 0.41);
            C2D_SpriteSetPos(&MenuSprites[BattleStageBack].spr, 230, 27);
            C2D_DrawSprite(&MenuSprites[BattleStageBack].spr);
        break;
    }
}