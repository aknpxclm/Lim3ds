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
    static u16 CharsPrinted = 0;
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

int TextWrapper(char *Dialogue)
{
    u32 currentPos = 0;
    u32 Track = 0;
    u16 word = 0;

    for(u32 i = 0; Dialogue[i] != '\0'; i++)
    {
        if(Dialogue[i] == ' ')
        {
            currentPos = i;
            while(Dialogue[currentPos + 1] != ' ')
            {
                word++;
                currentPos++;
                if(Dialogue[currentPos + 1] == '\0') break;
            }
            if(Track + word > 48) //to be hardcoded or variable by other functs
            {
                Dialogue[i] = '\n';
                Track = 0;
            }
            word = 0;
        }
        Track++;
    }
    return 0;
}

void DialogueManager(u16 *Line, char *Dialogue)
{
    FILE *TextLine;
    char BufText[4096];
    u16 currentLine = 1;
    u32 charpos = 0;
    u32 writepos = 0;

    TextLine = fopen("romfs:/Dialogue.txt", "r");
    fgets(BufText, sizeof(BufText), TextLine);
    fclose(TextLine);

    for(u32 i = 0; BufText[i] != '\0'; i++)
    {
        charpos = i;
        if(BufText[i] = '\n') currentLine += 1;
        if(currentLine == *Line)break;
    }
    for(u32 j = charpos; BufText[j] != '\n'; j++)
    {
        Dialogue[writepos] = BufText[j];
        writepos++;
    }
    Dialogue[writepos + 1] = '\0';
    TextWrapper(Dialogue);
}

void TextSceneBegin(C3D_RenderTarget *top, C3D_RenderTarget *bottom, u32 kDown, u16 *Line, Time *ScrollTime, char *Dialogue)
{
    static u16 charsToprint = 0;
    static u16 String = 0;
    String = strlen(Dialogue);
    C2D_Text DiaTex;
    char DialogueBuff[512];

    C2D_SceneBegin(top);

    snprintf(DialogueBuff, charsToprint, Dialogue);
    C2D_TextParse(&DiaTex, dynamBuf, DialogueBuff);
    C2D_TextOptimize(&DiaTex);

    if(ScrollTime->ElapsedTimeMs >= 27) //check if  x ms has passed
    {
        C2D_DrawText(&DiaTex, C2D_AlignCenter | C2D_WithColor, 5.0f, 120.0f, 0.5f, 1.0f, 1.0f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f));
        ScrollTime->InitialTimeMs = osGetTime();
    }
    else C2D_DrawText(&DiaTex, C2D_AlignCenter | C2D_WithColor, 5.0f, 120.0f, 0.5f, 1.0f, 1.0f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f));

    if(kDown & KEY_B)
    {
        if(charsToprint != String)
        {
            charsToprint = String;
        }
        else Line += 1;
    }
}

void FreeTexBuf()
{
    C2D_TextBufDelete(dynamBuf);
}