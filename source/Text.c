#include <3ds.h>
#include <citro2d.h>
#include "Sinner_Enemy_defin.h"
#include "Text.h"

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
        Enemy[0].Sanity - 50, Enemy[1].Sanity - 50, Enemy[2].Sanity - 50, Enemy[3].Sanity - 50, Enemy[4].Sanity - 50);
        break;

        case 1: //boss
        snprintf(EnemyBuf, sizeof(EnemyBuf), "Health: %.1f Sanity: %d", Enemy[0].Health, Enemy[0].Sanity - 50);
        break;
    }

    C2D_TextParse(&EnDynamTex, dynamBuf, EnemyBuf);
    C2D_TextOptimize(&EnDynamTex);
    C2D_DrawText(&EnDynamTex, C2D_AlignLeft, 2.0f, 3.0f, 0.5f, 0.525f, 0.525f);
}

void LobbyScrollingText(C3D_RenderTarget *top, C3D_RenderTarget *bottom, u8 *MenuPos, u8 *MainSubPos, Time *timeT)
{
    C2D_TextBufClear(dynamBuf);
    static u8 CharsPrinted = 0;
    static u8 currentmenu = 0;
    char LobbyBuf[128];
    const char MessageStart[] = "Press any botton to start";
    const char MessageLobbyInfo[3][64] = {"Tutorial and Dialogue Showcase", "Team select: Pick and order a team of sinners here", "Select to fight a Boss or Multiple enemies"};
    C2D_Text Scrollingtext;

    timeT->CurrentTimeMs = osGetTime();
    timeT->ElapsedTimeMs = (timeT->CurrentTimeMs - timeT->InitialTimeMs);

    if(currentmenu != *MainSubPos)
    {
        currentmenu = *MainSubPos;
        CharsPrinted = 0;
    }

    switch(*MenuPos)
    {
        case StartMen:
        if(CharsPrinted <= strlen(MessageStart)) CharsPrinted += 1;
        snprintf(LobbyBuf, CharsPrinted, MessageStart);
        C2D_TextParse(&Scrollingtext, dynamBuf, LobbyBuf);
        C2D_TextOptimize(&Scrollingtext);

        if(timeT->ElapsedTimeMs >= 33) //scroll speed, new char every 33ms
        {
            C2D_DrawText(&Scrollingtext, C2D_AlignLeft, 2.0f, 120.0f, 0.5f, 1.0f, 1.0f);
            timeT->InitialTimeMs = osGetTime();
        }
        else C2D_DrawText(&Scrollingtext, C2D_AlignLeft, 2.0f, 120.0f, 0.5f, 1.0f, 1.0f);
        break;

        case MainMen:
        C2D_SceneBegin(top);

        if(CharsPrinted <= strlen(MessageLobbyInfo[*MainSubPos])) CharsPrinted += 1;
        snprintf(LobbyBuf, CharsPrinted, MessageLobbyInfo[*MainSubPos]);
        C2D_TextParse(&Scrollingtext, dynamBuf, LobbyBuf);
        C2D_TextOptimize(&Scrollingtext);

        if(timeT->ElapsedTimeMs >= 27) //check if 25ms has passed
        {
            C2D_DrawText(&Scrollingtext, C2D_AlignLeft | C2D_WithColor, 5.0f, 220.0f, 0.5f, 0.5f, 0.5f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f));
            timeT->InitialTimeMs = osGetTime();
        }
        else C2D_DrawText(&Scrollingtext, C2D_AlignLeft | C2D_WithColor, 5.0f, 220.0f, 0.5f, 0.5f, 0.5f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f));
        break;
    }
}

void FreeTexBuf()
{
    C2D_TextBufDelete(dynamBuf);
}