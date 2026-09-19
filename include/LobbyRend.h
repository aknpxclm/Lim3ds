#ifndef LOBBYREND_H_
#define LOBBYREND_H_

void InitMain_M();
void DrawMain_S(C3D_RenderTarget *top, C3D_RenderTarget *bottom, u8 MenuPosition);
void FreeMain_M();
void SubMain(u8 *MainSubPos, u32 kDown, u32 kUp, char *LoadPath, SkillInfo *SkillBuf, SkillInfo SinSkill[][3]);

#endif