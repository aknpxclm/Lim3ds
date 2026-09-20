#include <3ds.h>
#include <citro2d.h>
#include <stdbool.h>
#include "Sinner_Enemy_defin.h"
#include "LoadCharac.h"
#include "LobbyRend.h"

//max sprites is 768
typedef struct
{
	C2D_Sprite spr;
	float dx, dy; // velocity
} Sprite;

static C2D_SpriteSheet menuSpriteSheet;

static Sprite MenuSprites[20]; //only for menu sprites

void InitMain_M()
{
    menuSpriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/menu.t3x");
    if (!menuSpriteSheet) svcBreak(USERBREAK_PANIC);

    for(int x = 0; x < 3; x++){
        Sprite *Menusprite = &MenuSprites[x];
        C2D_SpriteFromSheet(&Menusprite->spr, menuSpriteSheet, x/*sprite index in the sheet*/);
        C2D_SpriteSetCenter(&Menusprite->spr, 0.1f, 0.1f);
        C2D_SpriteSetPos(&Menusprite->spr, 30/*X position*/, 20/*Y position*/);
        C2D_SpriteSetRotation(&Menusprite->spr, 0);
        C2D_SpriteSetScale(&Menusprite->spr, 1/*X scale*/, 1/*Y scale*/);
    }
    C2D_SpriteSetPos(&MenuSprites[2].spr, -10, 20); //set bottom lobby png pos
}

void DrawMain_S(C3D_RenderTarget *top, C3D_RenderTarget *bottom, u8 MenuPosition)
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
        C2D_DrawSprite(&MenuSprites[LobbyTop].spr);
        C2D_SceneBegin(bottom);
        C2D_DrawSprite(&MenuSprites[LobbyBot].spr);
        break;

    }
}

void FreeMain_M()
{
    C2D_SpriteSheetFree(menuSpriteSheet);
}

void SubMain(u8 *MainSubPos, u32 kDown, u32 kUp, char *LoadPath, SkillInfo *SkillBuf, SkillInfo SinSkill[][3])
{
    static int IdToload = 0;
    static int TotalSinIdsInGame = 1; //total unique identities that can be loaded into a sinner slot (5)
    static u8 CursorOn_X_Sinner = 0;
    static bool UserInDeepSelect = false;

    if(!UserInDeepSelect)
    {
        if(kDown & KEY_DRIGHT && *MainSubPos <= 2) *MainSubPos += 1;
        if(kDown & KEY_DLEFT && *MainSubPos > 0) *MainSubPos -= 1;
    }
    switch(*MainSubPos)
    {
        case 0: //lobby with stage select
        break;

        case 1: //Team Select
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
                LoadSinInfo(SkillBuf, LoadPath);
                PassInSkillInfo(SinSkill, SkillBuf, CursorOn_X_Sinner);
                IdToload = 0;
            }
        }
        break;

        case 2: //TBD
        break;
    }
}