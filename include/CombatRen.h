#ifndef COMBSPRITEREN_H_
#define COMBSPRITEREN_H_

typedef struct
{
    u64 InitialTimeMs;
    u64 CurrentTimeMs;
    u64 ElapsedTimeMs;
}Time;

typedef struct
{
	C2D_Sprite spr;
	float dx, dy; // velocity
} Sprite;

typedef struct
{
    u8 Idle;
    u8 SkillUsed;
}TotalFrameSin;

typedef struct CharacterSprites
{
    C2D_SpriteSheet CombatSheetMisc; //moving, hit, evade, block and idle frames
    C2D_SpriteSheet CombatSheetS1;
    C2D_SpriteSheet CombatSheetS2;
    C2D_SpriteSheet CombatSheetS3;
    Sprite CombatSpr[70];
    TotalFrameSin Frame;
    u8 MiscSpritesEnd;
    //[0] : start of animiation, [1] : end of animation, [2] : delay between frames <- To be added if needed
    u8 Idle[2];
    u8 SkillOne[2];
    u8 SkillTwo[2];
    u8 SkillThree[2];
}CharSprites;

void LoadSinnerSheet(u8 SinnerToloadOn, u8 SheetId);
void InitCombat_S();
void GetTotalFrames(CharSprites TotalFrames[], u8 AtkOrder[][5]);
void RenderingCombat_S(C3D_RenderTarget *top, Time *Time_t, u16 *CurrentSinner, u8 *InCombatOrGFX, u8 AtkOrder[][5]);
void FreeSpriteSheets_Combat();

#endif