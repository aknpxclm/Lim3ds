#include "Sinner_Enemy_defin.h"

#ifndef LOADCHARAC_H_
#define LOADCHARAC_H_

char *AllocPathBuf();
void LoadSinInfo(SkillInfo *Sinner, char *Path);
void CharIdPath(int SinId, char *Path);
void FreePath(char *Path);

#endif