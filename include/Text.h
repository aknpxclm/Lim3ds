#include "Sinner_Enemy_defin.h"

#ifndef TEXT_H_
#define TEXT_H_

void CreateTexBuf();
void LoadFont();
void SinnerTex(Characters Sinner[]);
void EnemyTex(u8 *BossOrRegular, Characters Enemy[]);
void LobbyScrollingText(C3D_RenderTarget *top, C3D_RenderTarget *bottom, u8 *MenuPos, u8 *MainSubPos, Time *timeT);
int TextWrapper(char *Dialogue);
void DialogueManager(u16 *Line, char *Dialogue);
void TextSceneBegin(C3D_RenderTarget *top, C3D_RenderTarget *bottom, u32 kDown, u16 *Line, Time *ScrollTime, char *Dialogue);
void FontFree();
void FreeTexBuf();

#endif