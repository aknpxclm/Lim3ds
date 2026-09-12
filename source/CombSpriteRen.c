#include <3ds.h>
#include <citro2d.h>
#include "Sinner_Enemy_defin.h"
#include "CombSpriteRen.h"

//sprite animation example from http://www.nyankolab.com/

static u64 GFXRefreshMs = 33/*ms*/; //refresh graphics 30 times a second for 30fps

//void InitCombat_S(){}

//This handles the skill animations idle and movement of top screens sprites only
void RenderingCombat_S(C3D_RenderTarget *top, Time *Time_t, size_t *SkillSprites, u32 *CurrentFrameIndex,  u16 *CurrentSinner, u8 IdleIndex[], u8 IdleMax[], u8 *InCombatOrGFX)
{
    switch(*InCombatOrGFX)
    {
        case 0:
        Time_t->CurrentTimeMs = osGetTime();
        Time_t->ElapsedTimeMs += (*Time_t->CurrentTimeMs - *Time_t->InitialTimeMs);
        C2D_SceneBegin(top);
        if(Time_t->ElapsedTimeMs >= GFXRefreshMs)
        {
            Time_t->ElapsedTimeMs -= GFXRefreshMs; //reset elapsed time

                //C2D_DrawSprite(&Sprites[... + IdleIndex[0]].spr);
                //C2D_DrawSprite(&Sprites[... + IdleIndex[1]].spr);
                IdleIndex[Ally] = (IdleIndex[Ally] + 1) % IdleMax[Ally];
                IdleIndex[Opponent] = (IdleIndex[Opponent] + 1) % IdleMax[Opponent];
            
            Time_t->InitialTimeMs = osGetTime(); //set new initial time
        }
        else
        {
            //C2D_DrawSprite(&Sprites[... + IdleIndex[0]].spr);
            //C2D_DrawSprite(&Sprites[... + IdleIndex[1]].spr);
        }
        break;

        case 1:
        //SkillSprites = C2D_SpriteSheetCount(/*NO SKILL SHEETS YET*/); load winning character's sprite animation

        Time_t->CurrentTimeMs = osGetTime();
        Time_t->ElapsedTimeMs += (Time_t->CurrentTimeMs - Time_t->InitialTimeMs);
        C2D_SceneBegin(top);
        if(Time_t->ElapsedTimeMs >= GFXRefreshMs)
        {
            Time_t->ElapsedTimeMs -= GFXRefreshMs; //reset elapsed time
            //draw current frame index of the animation
            if(*CurrentFrameIndex != *SkillSprites) CurrentFrameIndex += 1;
            Time_t->InitialTimeMs = osGetTime(); //set new initial time
        }
        else
        {
            //draw current frame index of the animation
        }
        if(*CurrentFrameIndex == *SkillSprites){
        *CurrentFrameIndex = 0;
        *InCombatOrGFX = 1;
        *CurrentSinner += 1; //cycle through each sinner and clashing or going unopposed then go to the next one. Does this 5 times}
        }
        break;

    }
}

//void FreeSpriteSheets_Combat(){}