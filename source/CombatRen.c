#include <3ds.h>
#include <citro2d.h>
#include <string.h>
#include "Sinner_Enemy_defin.h"
#include "LoadCharac.h"
#include "CombatRen.h"

//sprite animation example from http://www.nyankolab.com/

typedef struct CharacterSprites
{
    C2D_SpriteSheet CombatSheet;
    C2D_Sprite CombatSpr[70];
    u8 MiscSpritesEnd;
    //[0] : start of animiation, [1] : end of animation, [2] : delay between frames <- unused
    u8 Idle[2]; 
    u8 SkillOne[2];
    u8 SkillTwo[2];
    u8 SkillThree[2];
}CharSprites;

static CharSprites SinnerSpr[5];
static CharSprites EnemySpr[5];

static const u64 GFXRefreshMs = 33/*ms*/; //refresh graphics 30 times a second for 30fps

void LoadSinnerSheet(u8 SinnerToloadOn, u8 SheetId)
{
    char Path[64];

    switch(SheetId)
    {
        case 0:
        return;

        case 1:
        strncpy(Path, "romfs:/gfx/IshCap.t3x", sizeof(Path));
        SinnerSpr[SinnerToloadOn].Idle[0] = 4;
        SinnerSpr[SinnerToloadOn].Idle[1] = 15;
        //SinnerSpr[SinnerToloadOn].Idle[2] = ;
        SinnerSpr[SinnerToloadOn].SkillOne[0] = 16;
        SinnerSpr[SinnerToloadOn].SkillOne[1] = 28;
        //SinnerSpr[SinnerToloadOn].SkillOne[2] = ;
        SinnerSpr[SinnerToloadOn].SkillTwo[0] = 29;
        SinnerSpr[SinnerToloadOn].SkillTwo[1] = 38;
        //SinnerSpr[SinnerToloadOn].SkillTwo[2] = ;
        SinnerSpr[SinnerToloadOn].SkillThree[0] = 39;
        SinnerSpr[SinnerToloadOn].SkillThree[1] = 63;
        //SinnerSpr[SinnerToloadOn].SkillThree[2] = ;
        break;
    }

    SinnerSpr[SinnerToloadOn].CombatSheet = C2D_SpriteSheetLoad(Path);
    if(!SinnerSpr[SinnerToloadOn].CombatSheet) svcBreak(USERBREAK_PANIC);
}

void InitCombat_S()
{
    for(int i = 0; i < 5; i++)
    {
        SinnerSpr[i].MiscSpritesEnd = 3;
    }
}

//This handles the skill animations idle and movement of top screens sprites only
void RenderingCombat_S(C3D_RenderTarget *top, Time *Time_t, size_t *SkillSprites, u16 *CurrentSinner, u8 *InCombatOrGFX)
{
    static u32 FrameIndexSin = 0;
    static u32 FrameIndexEn = 0;

    switch(*InCombatOrGFX)
    {
        case 0:
        Time_t->CurrentTimeMs = osGetTime();
        Time_t->ElapsedTimeMs += (Time_t->CurrentTimeMs - Time_t->InitialTimeMs);
        C2D_SceneBegin(top);
        if(Time_t->ElapsedTimeMs >= GFXRefreshMs)
        {
            Time_t->ElapsedTimeMs -= GFXRefreshMs; //reset elapsed time

                //C2D_DrawSprite(&Sprites[... + IdleIndex[0]].spr);
                //C2D_DrawSprite(&Sprites[... + IdleIndex[1]].spr);
                //IdleIndex[Ally] = (IdleIndex[Ally] + 1) % IdleMax[Ally];
                //IdleIndex[Opponent] = (IdleIndex[Opponent] + 1) % IdleMax[Opponent];
            
            Time_t->InitialTimeMs = osGetTime(); //set new initial time
        }
        else
        {
            //C2D_DrawSprite(&Sprites[... + IdleIndex[0]].spr);
            //C2D_DrawSprite(&Sprites[... + IdleIndex[1]].spr);
        }
        break;

        case 2:
        //SkillSprites = C2D_SpriteSheetCount(/*NO SKILL SHEETS YET*/); load winning character's sprite animation

        Time_t->CurrentTimeMs = osGetTime();
        Time_t->ElapsedTimeMs += (Time_t->CurrentTimeMs - Time_t->InitialTimeMs);
        C2D_SceneBegin(top);
        if(Time_t->ElapsedTimeMs >= GFXRefreshMs)
        {
            Time_t->ElapsedTimeMs -= GFXRefreshMs; //reset elapsed time
            //draw current frame index of the animation
            if(FrameIndexSin != *SkillSprites) FrameIndexSin += 1;
            Time_t->InitialTimeMs = osGetTime(); //set new initial time
        }
        else
        {
            //draw current frame index of the animation
        }
        if(FrameIndexSin == *SkillSprites + 1){
            FrameIndexSin = 0;
            *InCombatOrGFX = 1;
            *CurrentSinner += 1; //cycle through each sinner and clashing or going unopposed then go to the next one. Does this 5 times
        }
        break;

    }
}

void FreeSpriteSheets_Combat()
{
    for(int i = 0; i < 5; i++)
    {
        C2D_SpriteSheetFree(SinnerSpr[i].CombatSheet);
    }
}