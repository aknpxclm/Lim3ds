#include <3ds.h>
#include <citro2d.h>
#include "Sinner_Enemy_defin.h"
#include "CombatTex.h"

static C2D_TextBuf dynamBuf;

void CreateTexBuf()
{
    //Allocate memory for the buffer
    dynamBuf = C2D_TextBufNew(2048);
}

void SinnerTex(Characters Sinner[])
{
    //uses 3ds/graphics/printing/system-font example
    C2D_TextBufClear(dynamBuf); //clear text for next text change
    char SinnerBuf[256];
    C2D_Text SinDynamTex;
    
    snprintf(SinnerBuf, sizeof(SinnerBuf), "Health: %.1f  %.1f  %.1f  %.1f  %.1f\nSanity: %d  %d  %d  %d  %d", \
    Sinner[0].Health, Sinner[1].Health, Sinner[2].Health, Sinner[3].Health, Sinner[4].Health, \
    Sinner[0].Sanity - 50, Sinner[1].Sanity - 50, Sinner[2].Sanity - 50, Sinner[3].Sanity - 50, Sinner[4].Sanity - 50);

    C2D_TextParse(&SinDynamTex, dynamBuf, SinnerBuf); //parse the formatted strings
    C2D_TextOptimize(&SinDynamTex);
    C2D_DrawText(&SinDynamTex, C2D_AlignLeft, 2.0f, 206.0f, 0.5f, 0.525f, 0.525f);
}

void EnemyTex(u8 *BossOrRegular, Characters Enemy[])
{
    C2D_TextBufClear(dynamBuf);
    char EnemyBuf[256];
    C2D_Text EnDynamTex;

    switch(*BossOrRegular)
    {
        case 0: //reg
        snprintf(EnemyBuf, sizeof(EnemyBuf), "Health: %.1f %.1f %.1f %.1f %.1f\nSanity: %d  %d  %d  %d  %d", \
        Enemy[0].Health, Enemy[1].Health, Enemy[2].Health, Enemy[3].Health, Enemy[4].Health, \
        Enemy[0].Sanity -50, Enemy[1].Sanity - 50, Enemy[2].Sanity - 50, Enemy[3].Sanity - 50, Enemy[4].Sanity - 50);
        break;

        case 1: //boss
        snprintf(EnemyBuf, sizeof(EnemyBuf), "Health: %.1f Sanity: %d", Enemy[0].Health, Enemy[0].Sanity - 50);
        break;
    }

    C2D_TextParse(&EnDynamTex, dynamBuf, EnemyBuf);
    C2D_TextOptimize(&EnDynamTex);
    C2D_DrawText(&EnDynamTex, C2D_AlignLeft, 2.0f, 3.0f, 0.5f, 0.525f, 0.525f);
}

void FreeTexBuf()
{
    C2D_TextBufDelete(dynamBuf);
}