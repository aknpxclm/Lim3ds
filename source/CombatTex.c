#include <3ds.h>
#include <citro2d.h>
#include "Sinner_Enemy_defin.h"
#include "CombatTex.h"

static C2D_TextBuf dynamBuf;

void CreateTexBuf()
{
    //Allocate memory for the buffer
    dynamBuf = C2D_TextBufNew(4096);
}

void SinnerTex(Characters Sinner[], float xPosHP, float yPosHP, float xPosSP, float yPosSP)
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

void FreeTexBuf()
{
    C2D_TextBufDelete(dynamBuf);
}