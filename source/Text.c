#include <3ds.h>
#include <citro2d.h>
#include "Sinner_Enemy_defin.h"
#include "Text.h"

#define WordWrapLen 500.0f

static C2D_TextBuf dynamBuf;

/// @brief Creates a text buffer
void CreateTexBuf()
{
    //Allocate memory for the buffer
    dynamBuf = C2D_TextBufNew(4096);
}

/// @brief Renders the sinner health and sanity to the bottom screen
/// @param Sinner Sinner info
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

/// @brief Renders the enemy health and sanity to the bottom screen
/// @param BossOrRegular Check to render a just a boss or a group
/// @param Enemy Enemy info
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

/// @brief Draws text to help understand each submenu and also scrolls showing more characters over time
/// @param MenuPos 
/// @param MainSubPos 
/// @param timeT Records time for when more charcters should be shown
void LobbyScrollingText(u8 MenuPos, u8 *MainSubPos, Time *timeT)
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

    switch(MenuPos)
    {
        case StartMen:
        if(CharsPrinted <= strlen(MessageStart)) CharsPrinted += 1;
        snprintf(LobbyBuf, CharsPrinted, MessageStart);
        C2D_TextParse(&Scrollingtext, dynamBuf, LobbyBuf);
        C2D_TextOptimize(&Scrollingtext);

        if(timeT->ElapsedTimeMs >= 33) //scroll speed, new char every 33ms
        {
            timeT->InitialTimeMs = osGetTime();
        }
        C2D_DrawText(&Scrollingtext, C2D_AlignLeft, 2.0f, 120.0f, 0.5f, 1.0f, 1.0f);
        break;

        case MainMen:

        if(CharsPrinted <= strlen(MessageLobbyInfo[*MainSubPos])) CharsPrinted += 1;
        snprintf(LobbyBuf, CharsPrinted, MessageLobbyInfo[*MainSubPos]);
        C2D_TextParse(&Scrollingtext, dynamBuf, LobbyBuf);
        C2D_TextOptimize(&Scrollingtext);

        if(timeT->ElapsedTimeMs >= 27) //check if 25ms has passed
        {
            timeT->InitialTimeMs = osGetTime();
        }
        C2D_DrawText(&Scrollingtext, C2D_AlignLeft | C2D_WithColor, 5.0f, 220.0f, 0.5f, 0.5f, 0.5f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f));
        break;
    }
}

/// @brief Manages which line of dialogue to load from the file and format for rendering
/// @param Line Line of dialogue to load
/// @param Dialogue Buffer for the dialogue text
size_t DialogueManager(u16 *Line, char Dialogue[])
{
    FILE *TextLine;

    TextLine = fopen("romfs:/Dialogue/Tutorial.txt", "r");
    for(u16 i = 0; i < *Line + 1; i++) fgets(Dialogue, 256, TextLine);
    fclose(TextLine);

    return strlen(Dialogue);
    //TextWrapper(Dialogue);
}

/// @brief Draws dialogue to the screen
/// @param kDown Keys down this frame
/// @param Line Dialogue line to draw
/// @param String Length of the dialogue string read from the dialogue file in the manager
/// @param ScrollTime Records the time for when to display more characters of the line
/// @param Dialogue Dialogue text to render
void TextSceneBegin(u32 kDown, u16 *Line, size_t String, Time *ScrollTime, char Dialogue[])
{
    C2D_TextBufClear(dynamBuf);
    static u16 charsToprint = 0;
    C2D_Text DiaTex;
    char DialogueBuff[512];

    ScrollTime->CurrentTimeMs = osGetTime();
    ScrollTime->ElapsedTimeMs = (ScrollTime->CurrentTimeMs - ScrollTime->InitialTimeMs);

    snprintf(DialogueBuff, charsToprint, Dialogue);
    C2D_TextParse(&DiaTex, dynamBuf, DialogueBuff);
    C2D_TextOptimize(&DiaTex);

    if(ScrollTime->ElapsedTimeMs >= 60) //check if  x ms has passed
    {
        if(charsToprint < String) charsToprint += 1;
        ScrollTime->InitialTimeMs = osGetTime();
    }
    C2D_DrawText(&DiaTex, C2D_WordWrap, 10.0f, 180.0f, 0.5f, 0.65f, 0.65f, WordWrapLen);

    if(kDown & KEY_B)
    {
        if(charsToprint == String)
        {
            memset(Dialogue, 0, 256); //256 - size of dialogue buffer
            charsToprint = 0;
            String = 0;
            *Line += 1; 
        }
        else
        {
            charsToprint = String;
        }
    }
}

/// @brief Flags the first error found to be rendered
/// @param Flags Bit flag store
/// @return Error number to print
u8 RaiseTurnstartErrorFlag(u8 *Flags)
{
    u8 Error_t = 0;

    for(Error_t; Error_t < 8; Error_t++)
    {
        if((*Flags >> Error_t & 1U) == 1)
        {
            *Flags = *Flags ^ (1U << Error_t); //remove flag
            goto end;
        }
    }
    end:
    return Error_t + 1;
}

/// @brief Prints any errors that prevent turnstart
/// @param Flag Error to print
/// @param timeT Records time
void TurnStartErrResponce(u8 *Flag, Time *timeT)
{
    char Text[2][64] = {"Currently preparing combat please try again", "Not all skills are selected, please select an unused skill"};

    timeT->CurrentTimeMs = osGetTime();
    timeT->ElapsedTimeMs = (timeT->CurrentTimeMs - timeT->InitialTimeMs);

    if(timeT->ElapsedTimeMs <= 200 && *Flag != 0)
    {
        timeT->InitialTimeMs = osGetTime();
        C2D_Text Error;
        C2D_TextParse(&Error, dynamBuf, Text[*Flag - 1]);
        C2D_TextOptimize(&Error);
        C2D_DrawText(&Error, C2D_WordWrap, 5.0f, 120.0f, 0.5f, 0.5f, 0.5f, 300.0f);
        C2D_TextBufClear(dynamBuf);
    }
    else *Flag = 0;
}

/// @brief Frees text buffer
void FreeTexBuf()
{
    C2D_TextBufDelete(dynamBuf);
}