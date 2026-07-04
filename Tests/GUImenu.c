#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include <citro2d.h>

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240
#define MAX_SPRITES 13

//t3x files must fit in 1024x1024 space
//Use linear interpolation when scaling images
//16bit RGBA, as a 16bit num
//ONLY SCALE ONCE OR IT WILL BREAK 

typedef struct
{
	C2D_Sprite spr;
	float dx, dy; // velocity
} Sprite;

static C2D_SpriteSheet spriteSheet;
static Sprite sprites[MAX_SPRITES];
static uint64_t sprite_refresh_ms_time = 100;


int main(int argc, char* argv[])
{
	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	u16 start = 0;
	u16 stop = 0;
	u64 ms_time_elapsed = 0;
	u16 current_frame_index = 0;

	C3D_RenderTarget *top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);

	u16 numSprites = C2D_SpriteSheetCount(spriteSheet);

	for(int i = 0; i < numSprites; i++){
	Sprite *sprite = &sprites[i];

	C2D_SpriteFromSheet(&sprite->spr, spriteSheet, 0);
	C2D_SpriteSetCenter(&sprite->spr, 0.1f, 0.1f);
	C2D_SpriteSetPos(&sprite->spr, 30, -40);
	C2D_SpriteSetRotation(&sprite->spr, 0);
	C2D_SpriteSetScale(&sprite->spr, 1, 1);
	sprite->dx = 0;
	sprite->dy = 0;
	}
	start = osGetTime();
	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();
		
		// Your code goes here
		
		stop = osGetTime();
		ms_time_elapsed += (stop - start);

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
		C2D_SceneBegin(top);
		
		if (ms_time_elapsed >= sprite_refresh_ms_time) {
		ms_time_elapsed -= sprite_refresh_ms_time;
		current_frame_index = (current_frame_index + 1) % MAX_SPRITES;
		C2D_DrawSprite(&sprites[current_frame_index].spr);
		}
		else{
			C2D_DrawSprite(&sprites[current_frame_index].spr);
		}

		C3D_FrameEnd(0);

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
	}
	C2D_SpriteSheetFree(spriteSheet);

	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	return 0;
}
