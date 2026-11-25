#include <3ds/types.h>
#include "Sinner_Enemy_defin.h"

#ifndef JSONDIALOGUE_H_
#define JSONDIALOGUE_H_

int initJSON(char checker[]);
void drawDialogue(u32 kUp, TimeRecorder *ScrollTime, int *Line, char Dialogue[]);

#endif