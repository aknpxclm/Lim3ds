#ifndef LOBBYREND_H_
#define LOBBYREND_H_

void InitMain_M();
void DrawMain_S(C3D_RenderTarget *top, C3D_RenderTarget *bottom, u8 MenuPosition, u8 *TintBG);
void FreeMain_M();
void SubMain(C3D_RenderTarget *top, C3D_RenderTarget *bottom, \
    u32 kDown, u32 kUp, char *LoadPath, \
    SkillInfo *SkillBuf, SkillInfo SinSkill[][4], SkillInfo Enskill[][4], \
    u8 *MenuPos, u8 *TintBG, u8 *BossCharInit);

#endif