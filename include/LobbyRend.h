#ifndef LOBBYREND_H_
#define LOBBYREND_H_

void InitMain_M();
void DrawMain_S(gfxScreen_t screen, u8 MenuPosition, u8 *TintBG);
void FreeMain_M();
void SubMain(gfxScreen_t screen, \
             u32 kDown, u32 kUp, char *LoadPath, \
             SkillInfo *SkillBuf, SkillInfo SinSkill[][4], SkillInfo Enskill[][4], Characters Sinner[], \
             u8 *MenuPos, u8 *MainSubPos, u8 *TintBG, u8 *BossCharInit);

#endif