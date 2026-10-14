#include "Sinner_Enemy_defin.h"

#ifndef TEXT_H_
#define TEXT_H_

void CreateTexBuf();
void SinnerTex(Characters Sinner[]);
void EnemyTex(u8 *BossOrRegular, Characters Enemy[]);
void LobbyScrollingText(u8 *MenuPos, u8 *MainSubPos, Time *timeT);
void FreeTexBuf();

#endif