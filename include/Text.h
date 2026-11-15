#include "Sinner_Enemy_defin.h"

#ifndef TEXT_H_
#define TEXT_H_

void createTexBuf();
void sinnerTex(Characters Sinner[]);
void enemyTex(u8 *BossOrRegular, Characters Enemy[]);
void lobbyScrollingText(u8 MenuPos, u8 *MainSubPos, TimeRecorder *timeT);
//int TextWrapper(char *Dialogue);
size_t dialogueManager(u16 *Line, char Dialogue[]);
void textSceneBegin(u32 kDown, u16 *Line, size_t String, TimeRecorder *ScrollTime, char Dialogue[]);
void dialogueLog(u16 *Line);
u8 raiseTurnstartErrorFlag(u8 *Flags);
void turnStartErrResponce(u8 *Flag, TimeRecorder *timeT);
void freeTexBuf();

#endif