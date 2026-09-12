#ifndef COMBSPRITEREN_H_
#define COMBSPRITEREN_H_

typedef struct
{
    u64 InitialTimeMs;
    u64 CurrentTimeMs;
    u64 ElapsedTimeMs;
}Time;

void RenderingCombat_S(C3D_RenderTarget *top, Time *Time_t, size_t *SkillSprites, u32 *CurrentFrameIndex,  u16 *CurrentSinner, u8 IdleIndex[], u8 IdleMax[], u8 *InCombatOrGFX);

#endif