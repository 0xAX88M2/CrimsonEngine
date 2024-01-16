#include "crimson.h"
#include "framework.h"

// ========================
// LOCAL DECLARATIONS
// ========================

CR_TEXTURE* loading_gfx;

unsigned char loading_startbuffer;

void loading_update(void)
{

	cr_hires_toggle(true);
	//cr_sprite_draw(loading_gfx, 1024, 824, CR_NO_FLIP, 0);

	// GLOBAL LOADING

	if (loading_startbuffer > 0) {
		loading_startbuffer--;
	}
	else {
		fr_load();
	}
	cr_hires_toggle(false);
}

void loading_init(void)
{
	loading_startbuffer = 2;

	// Mark loading as initialized
	loading.initialized = true;
}

void loading_load(void)
{
	loading_gfx = cr_sprite_load("cd/stages/loading/loading_gfx.png", 0x000000);

	// Mark loading as loaded
	loading.loaded = true;
}
void loading_unload(void)
{
	cr_sprite_unload(loading_gfx);

	// Mark as NOT loaded and NOT initialized
	loading.loaded = false;
	loading.initialized = false;
}