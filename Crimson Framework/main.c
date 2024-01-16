#include "framework.h"

int main(int argc, char *argv[])
{
	if (argc || argv) { printf("Crimson Engine has started! (This text is actually here to get rid of a useless warning!)\n"); }

	cr_debug.enabled = true;

	// ========================
	// Stage Registry
	// ========================

	cr_stage_add(&loading, "Initial Load", loading_load, loading_unload, loading_init, loading_update);

	cr_stage_add(&vape, "Vaporwave Test", vape_load, vape_unload, vape_init, vape_update);
	cr_stage_add(&teststage, "Test Stage", teststage_load, teststage_unload, teststage_init, teststage_update);
	cr_stage_add(&level6, "Level 6 - Hyper Zone", level6_load, level6_unload, level6_init, level6_update);

	cr_framework_init("Crimson Template Game", "Version Number Goes Here", fr_init, fr_update_start, fr_update_end, fr_close, 60.0);

	//crimson_init(640, 360, 640, 360, 1920, 1080, 1920, 1080);
	//crimson_init(424, 240, 424, 240, 1920, 1080, 1920, 1080);
	crimson_init(400, 224, 400, 224, 1920, 1080, 1920, 1080);

	cr_stage_goto(&loading);

	crimson.scale = 2;
	crimson.default_spawn_layer = 4;
	crimson_run();

	//system("pause");

	return 0;
}