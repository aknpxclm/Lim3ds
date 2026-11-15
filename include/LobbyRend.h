#ifndef LOBBYREND_H_
#define LOBBYREND_H_

void initMain_M();
void drawMain_S(gfxScreen_t screen, u8 MenuPosition, u8 *TintBG);
void subMain(u32 kDown, u32 kUp, Characters *Sinner, Characters *Enemy ,u8 *MenuPos, u8 *MainSubPos, u8 *TintBG, u8 *BossCharInit);
void freeMain_M();
#endif