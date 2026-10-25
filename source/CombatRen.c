#include <3ds.h>
#include <citro2d.h>
#include <stdlib.h>
#include <string.h>
#include "Sinner_Enemy_defin.h"
#include "LoadCharac.h"
#include "CombatRen.h"

//sprite animation example from http://www.nyankolab.com/

static CharSprites SinnerSpr[5];
static CharSprites *EnemySpr;

static const u64 GFXRefreshMs = 33/*ms*/; //refresh graphics 30 times a second for 30fps

/// @brief Loads SpriteSheets of different Ids onto a Sinner Sprite Object
/// @param SinnerToloadOn Which Sinner struct to load a sheet onto
/// @param SinnerSheetId Id of the Sheet to load (Look at each case to see which Id is which)
void LoadSinnerSheet(u8 SinnerToloadOn, u8 SinnerSheetId)
{
    char PathMisc[64];
    char PathS1[64];
    char PathS2[64];
    char PathS3[64];

    switch(SinnerSheetId)
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

/// @brief Loads the boss and enemy sheets into the Enemy Sprite object
/// @param IsABoss
void LoadEnemySheets(u8 *IsABoss)
{
    const char EnemySheets[4][4][64] =  
    {{"romfs:/gfx/BossMisc", "romfs:/gfx/BossS1", "romfs:/gfx/BossS2", "romfs:/gfx/BossS3"}, \
     {"romfs:/gfx/EnType1Misc", "romfs:/gfx/EnType1S1", "romfs:/gfx/EnType1S2", "romfs:/gfx/EnType1S3"}, \
     {"romfs:/gfx/EnType2Misc", "romfs:/gfx/EnType2S1", "romfs:/gfx/EnType2S2", "romfs:/gfx/EnType2S3"}, \
     {"romfs:/gfx/EnType3Misc", "romfs:/gfx/EnType3S1", "romfs:/gfx/EnType3S2", "romfs:/gfx/EnType3S3"}};
    
    if(*IsABoss == 1)
    {
        EnemySpr[0].CombatSheetMisc = C2D_SpriteSheetLoad(EnemySheets[0][0]);
            if(!EnemySpr[0].CombatSheetMisc) svcBreak(USERBREAK_PANIC);
        EnemySpr[0].CombatSheetS1 = C2D_SpriteSheetLoad(EnemySheets[0][1]);
            if(!EnemySpr[0].CombatSheetS1) svcBreak(USERBREAK_PANIC);
        EnemySpr[0].CombatSheetS2 = C2D_SpriteSheetLoad(EnemySheets[0][2]);
            if(!EnemySpr[0].CombatSheetS2) svcBreak(USERBREAK_PANIC);
        EnemySpr[0].CombatSheetS3 = C2D_SpriteSheetLoad(EnemySheets[0][3]);
            if(!EnemySpr[0].CombatSheetS3) svcBreak(USERBREAK_PANIC);
    }
    else
    {
        for(int i = 0; i < 3; i++)
        {
            EnemySpr[i].CombatSheetMisc = C2D_SpriteSheetLoad(EnemySheets[i + 1][0]);
                if(!EnemySpr[i].CombatSheetMisc) svcBreak(USERBREAK_PANIC);
            EnemySpr[i].CombatSheetS1 = C2D_SpriteSheetLoad(EnemySheets[i + 1][1]);
                if(!EnemySpr[i].CombatSheetS1) svcBreak(USERBREAK_PANIC);
            EnemySpr[i].CombatSheetS2 = C2D_SpriteSheetLoad(EnemySheets[i + 1][2]);
                if(!EnemySpr[i].CombatSheetS2) svcBreak(USERBREAK_PANIC);
            EnemySpr[i].CombatSheetS3 = C2D_SpriteSheetLoad(EnemySheets[i + 1][3]);
                if(!EnemySpr[i].CombatSheetS3) svcBreak(USERBREAK_PANIC);
        }
    }
}

/// @brief Allocate memory for the Enemy Sprite Object
void InitEnemySheets()
{
    EnemySpr = (CharSprites*)malloc(sizeof(CharSprites) * 3); //allocate enough for unique 3 types of enemies
}

/// @brief Load all the Sprites in the spritesheets into the individual sprite handles
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
            else if(j >= SinnerSpr[i].SkillOne[1] && j < SinnerSpr[i].SkillTwo[1]) C2D_SpriteFromSheet(&CharSpr->spr, SinnerSpr[i].CombatSheetS2, j - SinnerSpr[i].SkillOne[1]);
            else if(j >= SinnerSpr[i].SkillTwo[1] && j < SinnerSpr[i].SkillThree[1]) C2D_SpriteFromSheet(&CharSpr->spr, SinnerSpr[i].CombatSheetS3, j - SinnerSpr[i].SkillTwo[1]);
            C2D_SpriteSetCenter(&CharSpr->spr, 0.1f, 0.1f);
            C2D_SpriteSetPos(&CharSpr->spr, 0, 0);
            C2D_SpriteSetRotation(&CharSpr->spr, 0);
            C2D_SpriteSetScale(&CharSpr->spr, 1, 1);
        }
        NumSprites = 0;
    }
}

/// @brief Calculate the total frames of skills / idle animations excluding other combat sprites
/// @param TotalFrames Stores the frame data for rendering
/// @param AtkOrder Skill rank being used in the turn for each sinner
void GetTotalFrames(CharSprites TotalFrames[], u8 AtkOrder[][2])
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

/// @brief This handles the skill animations idle and movement of top screens sprites only
/// @param top 
/// @param Time_t Records the time for when sprite need to be changed for animations
/// @param CurrentSinner Current sinner to render
/// @param InCombatOrGFX Checks if the idle or skill animations if needed to render
/// @param AtkOrder Skill rank to be used this turn
/// @param SinnerPositions Contains positions to render idle sinners at
void RenderingCombat_S(C3D_RenderTarget *top, Time *Time_t, u16 *CurrentSinner, u8 *InCombatOrGFX, u8 AtkOrder[][2], float SinnerPositions[][2])
{
    static u8 FrameIndexSin[5] = {0};
    static u8 FrameIndexEn[5] = {0};

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
            //for(int i = 0; i < 5; i++)
            //{
                C2D_DrawSprite(&SinnerSpr[0].CombatSpr[SinnerSpr[0].Idle[0] + FrameIndexSin[0]].spr);
                FrameIndexSin[0] = (FrameIndexSin[0] + 1) % SinnerSpr[*CurrentSinner].Frame.Idle;
            //}                
            
            Time_t->InitialTimeMs = osGetTime(); //set new initial time
        }
        else
        {
            //for(int j = 0; j < 5; j++)
            //{
            C2D_DrawSprite(&SinnerSpr[0].CombatSpr[SinnerSpr[0].Idle[0] + FrameIndexSin[0]].spr);
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
            if(FrameIndexSin[0] != SinnerSpr[*CurrentSinner].Frame.SkillUsed) FrameIndexSin[0] += 1;
            Time_t->InitialTimeMs = osGetTime(); //set new initial time
        }
        else
        {
            //draw current frame index of the animation
        }
        if(FrameIndexSin[0] == SinnerSpr[*CurrentSinner].Frame.SkillUsed){
            FrameIndexSin[*CurrentSinner] = 0;
            *InCombatOrGFX = 1; //do next clash
            *CurrentSinner += 1; //cycle through each sinner and clashing or going unopposed then go to the next one. Does this 5 times
        }
        break;

    }
}

/// @brief Clears info of a previous animation
void DeInitSpriteInfo()
{
    for(int i = 0; i < 5; i++)
    {
        SinnerSpr[i].Frame.Idle = 0;
        SinnerSpr[i].Frame.SkillUsed = 0;
    } 
}

/// @brief Frees all spritesheets used in the combat
/// @param BossOrRegular Checks if it's a boss or a group
void FreeSpriteSheets_Combat(u8 *BossOrRegular)
{
    for(int i = 0; i < 5; i++)
    {
        C2D_SpriteSheetFree(SinnerSpr[i].CombatSheetMisc);
        C2D_SpriteSheetFree(SinnerSpr[i].CombatSheetS1);
        C2D_SpriteSheetFree(SinnerSpr[i].CombatSheetS2);
        C2D_SpriteSheetFree(SinnerSpr[i].CombatSheetS3);
    }
    switch(*BossOrRegular)
    {
        case 0:
        for(int j = 0; j < 5; j++)
        {
            C2D_SpriteSheetFree(EnemySpr[j].CombatSheetMisc);
            C2D_SpriteSheetFree(EnemySpr[j].CombatSheetS1);
            C2D_SpriteSheetFree(EnemySpr[j].CombatSheetS2);
            C2D_SpriteSheetFree(EnemySpr[j].CombatSheetS3);
        }
        break;

        case 1:
        C2D_SpriteSheetFree(EnemySpr->CombatSheetMisc);
        C2D_SpriteSheetFree(EnemySpr->CombatSheetS1);
        C2D_SpriteSheetFree(EnemySpr->CombatSheetS2);
        C2D_SpriteSheetFree(EnemySpr->CombatSheetS3);
        break;

    }
    free(EnemySpr);
}