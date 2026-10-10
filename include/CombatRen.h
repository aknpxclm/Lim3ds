#ifndef COMBSPRITEREN_H_
#define COMBSPRITEREN_H_

typedef struct
{
    u64 InitialTimeMs;
    u64 CurrentTimeMs;
    u64 ElapsedTimeMs;
}Time;
void LoadSinnerSheet(u8 SinnerToloadOn, u8 SheetId);
void RenderingCombat_S(C3D_RenderTarget *top, Time *Time_t, size_t *SkillSprites, u16 *CurrentSinner, u8 *InCombatOrGFX);

#endif