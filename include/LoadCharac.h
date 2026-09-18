#include "Sinner_Enemy_defin.h"

#ifndef LOADCHARAC_H_
#define LOADCHARAC_H_

char *AllocPathBuf();
SkillInfo *SkillInfoBuf();
void LoadSinInfo(SkillInfo *Sinner, char *Path);
void PassInSkillInfo(SkillInfo Sinner[][3], SkillInfo *SkillBuf, u8 LoadOnSinner);
void CharIdPath(int SinId, char *Path);
void FreeSkillFileInfo(char *Path, SkillInfo *SkillBuf);

#endif