#include <3ds.h>
#include <citro2d.h>
#include <string.h>
#include "Sinner_Enemy_defin.h"
#include "LoadCharac.h"
#include "CombatRen.h"

//sprite animation example from http://www.nyankolab.com/

static CharSprites SinnerSpr[5];
static CharSprites EnemySpr[5];

static const u64 GFXRefreshMs = 33/*ms*/; //refresh graphics 30 times a second for 30fps

void LoadSinnerSheet(u8 SinnerToloadOn, u8 SheetId)
{
    char PathMisc[64];
    char PathS1[64];
    char PathS2[64];
    char PathS3[64];

    switch(SheetId)
    {
        case 0:
        return;

        case 1:
        strncpy(PathMisc, "romfs:/gfx/IshCapMisc.t3x", sizeof(PathMisc));
        SinnerSpr[SinnerToloadOn].Idle[0] = 4;
        SinnerSpr[SinnerToloadOn].Idle[1] = 15;
        //SinnerSpr[SinnerToloadOn].Idle[2] = ;
        strncpy(PathS1, "romfs:/gfx/IshCapS1.t3x", sizeof(PathS1));
        SinnerSpr[SinnerToloadOn].SkillOne[0] = 16;
        SinnerSpr[SinnerToloadOn].SkillOne[1] = 28;
        //SinnerSpr[SinnerToloadOn].SkillOne[2] = ;
        strncpy(PathS2, "romfs:/gfx/IshCapS2.t3x", sizeof(PathS2));
        SinnerSpr[SinnerToloadOn].SkillTwo[0] = 29;
        SinnerSpr[SinnerToloadOn].SkillTwo[1] = 38;
        //SinnerSpr[SinnerToloadOn].SkillTwo[2] = ;
        strncpy(PathS3, "romfs:/gfx/IshCapS3.t3x", sizeof(PathS3));
        SinnerSpr[SinnerToloadOn].SkillThree[0] = 39;
        SinnerSpr[SinnerToloadOn].SkillThree[1] = 63;
        //SinnerSpr[SinnerToloadOn].SkillThree[2] = ;
        break;
    }
    SinnerSpr[SinnerToloadOn].CombatSheetMisc = C2D_SpriteSheetLoad(PathMisc); 
        if(!SinnerSpr[SinnerToloadOn].CombatSheetMisc) svcBreak(USERBREAK_PANIC);
    SinnerSpr[SinnerToloadOn].CombatSheetS1 = C2D_SpriteSheetLoad(PathS1);
        if(!SinnerSpr[SinnerToloadOn].CombatSheetS1) svcBreak(USERBREAK_PANIC);
    SinnerSpr[SinnerToloadOn].CombatSheetS2 = C2D_SpriteSheetLoad(PathS2);
        if(!SinnerSpr[SinnerToloadOn].CombatSheetS2) svcBreak(USERBREAK_PANIC);
    SinnerSpr[SinnerToloadOn].CombatSheetS3 = C2D_SpriteSheetLoad(PathS3);
        if(!SinnerSpr[SinnerToloadOn].CombatSheetS3) svcBreak(USERBREAK_PANIC);
}

void InitCombat_S()
{
    size_t NumSprites = 0;
    for(int i = 0; i < 5; i++)
    {
        SinnerSpr[i].MiscSpritesEnd = 3;
        NumSprites += C2D_SpriteSheetCount(SinnerSpr[i].CombatSheetMisc);
        NumSprites += C2D_SpriteSheetCount(SinnerSpr[i].CombatSheetS1);
        NumSprites += C2D_SpriteSheetCount(SinnerSpr[i].CombatSheetS2);
        NumSprites += C2D_SpriteSheetCount(SinnerSpr[i].CombatSheetS3);

        for(int j = 0; j < NumSprites; j++)
        {
            Sprite *CharSpr = &SinnerSpr[i].CombatSpr[j];
            if(j < SinnerSpr[i].MiscSpritesEnd) C2D_SpriteFromSheet(&CharSpr->spr, SinnerSpr[i].CombatSheetMisc, j);
            else if(j >= SinnerSpr[i].MiscSpritesEnd && j < SinnerSpr[i].SkillOne[1]) C2D_SpriteFromSheet(&CharSpr->spr, SinnerSpr[i].CombatSheetS1, j - SinnerSpr[i].MiscSpritesEnd);
            else if(j >= SinnerSpr[i].SkillOne && j < SinnerSpr[i].SkillTwo[1]) C2D_SpriteFromSheet(&CharSpr->spr, SinnerSpr[i].CombatSheetS2, j - SinnerSpr[i].SkillOne[1]);
            else if(j >= SinnerSpr[i].SkillTwo && j < SinnerSpr[i].SkillThree[1]) C2D_SpriteFromSheet(&CharSpr->spr, SinnerSpr[i].CombatSheetS3, j - SinnerSpr[i].SkillTwo[1]);
            C2D_SpriteSetCenter(&CharSpr->spr, 0.1f, 0.1f);
            C2D_SpriteSetPos(&CharSpr->spr, 0, 0);
            C2D_SpriteSetRotation(&CharSpr->spr, 0);
            C2D_SpriteSetScale(&CharSpr->spr, 1, 1);
        }
        NumSprites = 0;
    }
}

void GetTotalFrames(CharSprites TotalFrames[], u8 AtkOrder[][5])
{
    for(int i = 0; i < 5; i++)
    {
        TotalFrames[i].Frame.Idle = SinnerSpr[i].Idle[1] - SinnerSpr[i].Idle[0];
        switch(AtkOrder[0][i])
        {
            case 1:
            TotalFrames[i].Frame.SkillUsed = SinnerSpr[i].SkillOne[1] - SinnerSpr[i].SkillOne[0];
            break;
            
            case 2:
            TotalFrames[i].Frame.SkillUsed = SinnerSpr[i].SkillTwo[1] - SinnerSpr[i].SkillTwo[0];
            break;

            case 3:
            TotalFrames[i].Frame.SkillUsed = SinnerSpr[i].SkillThree[1] - SinnerSpr[i].SkillThree[0];
            break;
        }   
    }
}

//This handles the skill animations idle and movement of top screens sprites only
void RenderingCombat_S(C3D_RenderTarget *top, Time *Time_t, u16 *CurrentSinner, u8 *InCombatOrGFX, u8 AtkOrder[][5])
{
    static u8 FrameIndexSin = 0;
    static u8 FrameIndexEn = 0;

    if(SinnerSpr[0].Frame.Idle == 0) GetTotalFrames(SinnerSpr, AtkOrder);

    switch(*InCombatOrGFX)
    {
        case 0: //idle
        Time_t->CurrentTimeMs = osGetTime();
        Time_t->ElapsedTimeMs += (Time_t->CurrentTimeMs - Time_t->InitialTimeMs);
        C2D_SceneBegin(top);
        if(Time_t->ElapsedTimeMs >= GFXRefreshMs)
        {
            Time_t->ElapsedTimeMs -= GFXRefreshMs; //reset elapsed time

            C2D_DrawSprite(&SinnerSpr[0].CombatSpr[SinnerSpr[0].Idle[0] + FrameIndexSin].spr);
            //C2D_DrawSprite(&Sprites[... + IdleIndex[1]].spr);
            //IdleIndex[Ally] = (IdleIndex[Ally] + 1) % IdleMax[Ally];
            FrameIndexSin = (FrameIndexSin + 1) % SinnerSpr[*CurrentSinner].Frame.Idle;
            //IdleIndex[Opponent] = (IdleIndex[Opponent] + 1) % IdleMax[Opponent];                
            
            Time_t->InitialTimeMs = osGetTime(); //set new initial time
        }
        else
        {
            C2D_DrawSprite(&SinnerSpr[0].CombatSpr[SinnerSpr[0].Idle[0] + FrameIndexSin].spr);
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
            if(FrameIndexSin != SinnerSpr[*CurrentSinner].Frame.SkillUsed) FrameIndexSin += 1;
            Time_t->InitialTimeMs = osGetTime(); //set new initial time
        }
        else
        {
            //draw current frame index of the animation
        }
        if(FrameIndexSin == SinnerSpr[*CurrentSinner].Frame.SkillUsed){
            FrameIndexSin = 0;
            *InCombatOrGFX = 1; //do next clash
            *CurrentSinner += 1; //cycle through each sinner and clashing or going unopposed then go to the next one. Does this 5 times
        }
        break;

    }
}

void DeInitSpriteInfo()
{
    for(int i = 0; i < 5; i++)
    {
        SinnerSpr[i].Frame.Idle = 0;
        SinnerSpr[i].Frame.SkillUsed = 0;
    } 
}

void FreeSpriteSheets_Combat()
{
    for(int i = 0; i < 5; i++)
    {
        C2D_SpriteSheetFree(SinnerSpr[i].CombatSheetMisc);
        C2D_SpriteSheetFree(SinnerSpr[i].CombatSheetS1);
        C2D_SpriteSheetFree(SinnerSpr[i].CombatSheetS2);
        C2D_SpriteSheetFree(SinnerSpr[i].CombatSheetS3);
    }
}