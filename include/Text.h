#include "Sinner_Enemy_defin.h"

#ifndef TEXT_H_
#define TEXT_H_

void CreateTexBuf();
void SinnerTex(Characters Sinner[]);
void EnemyTex(u8 *BossOrRegular, Characters Enemy[]);
void LobbyScrollingText(u8 *MenuPos, u8 *MainSubPos, Time *timeT);
//int TextWrapper(char *Dialogue);
size_t DialogueManager(u16 *Line, char Dialogue[]);
void TextSceneBegin(u32 kDown, u16 *Line, size_t String, Time *ScrollTime, char Dialogue[]);
u8 RaiseTurnstartErrorFlag(u8 *Flags);
void TurnStartErrResponce(u8 *Flag, Time *timeT);
void FreeTexBuf();

#endif