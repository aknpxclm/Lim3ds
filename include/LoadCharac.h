#include "Sinner_Enemy_defin.h"
#define Array_Column_Len 4

#ifndef LOADCHARAC_H_
#define LOADCHARAC_H_

char *AllocPathBuf();
SkillInfo *SkillInfoBuf();
void LoadSinInfo(SkillInfo *Sinner, char *Path);
void PassInSkillInfo(SkillInfo Sinner[][Array_Column_Len], SkillInfo *SkillBuf, u8 LoadOnSin);
void CharIdPath(u16 SinId, char *Path);
void FreeSkillFileInfo(char *Path, SkillInfo *SkillBuf);

#endif