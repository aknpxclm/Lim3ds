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

int main(int argc, char* argv[])
{
	
	gfxInitDefault();
	romfsInit();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	C3D_RenderTarget *top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);

	Sprite *sprite = &sprites[0];
	C2D_SpriteFromSheet(&sprite->spr, spriteSheet, 0);
	C2D_SpriteSetCenter(&sprite->spr, 0.1f, 0.1f);
	C2D_SpriteSetPos(&sprite->spr, 30, -40);
	C2D_SpriteSetRotation(&sprite->spr, 0);
	C2D_SpriteSetScale(&sprite->spr, 1, 1);
	sprite->dx = 0;
	sprite->dy = 0;

	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();
		
		// Your code goes here

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
		C2D_SceneBegin(top);
		C2D_DrawSprite(&sprites[0].spr);
		C3D_FrameEnd(0);

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
	}
	C2D_SpriteSheetFree(spriteSheet);

	C2D_Fini();
	C3D_Fini();
	romfsExit();
	gfxExit();
	
	return 0;
}
