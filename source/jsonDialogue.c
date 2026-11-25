#include <3ds/types.h>
#include <citro2d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c2d/text.h"
#include "cJSON.h"
#include "Sinner_Enemy_defin.h"

#define WordWrapLen 500.0f
#define TotalDialogueLines 6

static C2D_TextBuf J_logue;

int initJSON(char checker[])
{
    
}

void drawDialogue(u32 kUp, TimeRecorder *ScrollTime, int *Line, char Dialogue[])
{
    C2D_TextBufClear(J_logue);
    static u16 charsToprint = 0;
    static size_t String = 0;
    String = strlen(Dialogue);
    C2D_Text DiaTex;

    ScrollTime->CurrentTimeMs = osGetTime();
    ScrollTime->ElapsedTimeMs += (ScrollTime->CurrentTimeMs - ScrollTime->InitialTimeMs);

    C2D_TextParse(&DiaTex, J_logue, Dialogue);
    C2D_TextOptimize(&DiaTex);

    if(ScrollTime->ElapsedTimeMs >= 60) //check if  x ms has passed
    {
        if(charsToprint < String) charsToprint += 1;
        ScrollTime->InitialTimeMs = osGetTime();
    }
    C2D_DrawText(&DiaTex, C2D_WordWrap, 10.0f, 180.0f, 0.5f, 0.65f, 0.65f, WordWrapLen);

    if(kUp & KEY_B)
    {
        if(charsToprint == String)
        {
            if(*Line < TotalDialogueLines)
            {
                charsToprint = 0;
                String = 0;
                *Line += 1;
            }
        }
        else charsToprint = String;
    }
}