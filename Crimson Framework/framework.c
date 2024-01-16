#include "framework.h"

void fr_init(void)
{

}

void fr_update_start(void)
{
	
}

void fr_update_end(void)
{
	fr_hitbox_debug_draw();
}

void fr_close(void)
{

}

void fr_load(void)
{
	fr_hitbox_master_init(512);

	fr_fade_load(NULL);

	player_load(NULL);

	cr_stage_goto(&vape);
	//cr_stage_goto(&teststage);
}

void fr_unload(void)
{
	fr_hitbox_master_close();

	fr_fade_unload();

	player_unload();
}