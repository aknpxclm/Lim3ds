#include "Sinner_Enemy_defin.h"
#define Array_Column_Len 4

#ifndef LOADCHARAC_H_
#define LOADCHARAC_H_

char *allocPathBuf();
void charIdPath(u16 SinId, char *Path);
SkillInfo *skillInfoBuf();
void loadSinInfo(SkillInfo *Sinner, char *Path);
void passInSkillInfo(SkillInfo *Sinner, SkillInfo *SkillBuf);
void freeSkillFileInfo(char *Path, SkillInfo *SkillBuf);

#endif