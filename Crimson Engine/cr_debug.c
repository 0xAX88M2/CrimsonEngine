#include "crimson.h"

#if CR_PLATFORM == CR_WINDOWS
#include "cr_win_debug.h"
#endif // CR_PLATFORM == CR_WINDOWS

void cr_devmenu_load(void)
{
	cr_debug.fastforward_speed = 10;
	cr_script_load("cd/cr_debug/devmenu/font.cra", &cr_devmenu.data, &cr_devmenu.white, 1);
	cr_script_add(&cr_devmenu.data, &cr_devmenu.white, 0);
	cr_script_add(&cr_devmenu.data, &cr_devmenu.red, 1);
	cr_script_add(&cr_devmenu.data, &cr_devmenu.white_nums, 2);
	cr_script_add(&cr_devmenu.data, &cr_devmenu.red_nums, 3);

	cr_debug.sprites[cr_debug_sensorpoint] = cr_sprite_load("cd/cr_debug/sensorpoint.png", 0x00ff00);
}

void cr_devmenu_unload(void)
{
	cr_script_unload(&cr_devmenu.data);

	cr_sprite_unload(cr_debug.sprites[cr_debug_sensorpoint]);
}

void cr_devmenu_run(void)
{
	
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_devmenu_run();
#endif // CR_PLATFORM == CR_WINDOWS

}
