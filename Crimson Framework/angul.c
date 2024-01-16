#include "framework.h"

// ========================
// LOCAL DECLARATIONS
// ========================

// Some declarations!

CR_ANGLEMAP anglemap;
CR_BLOCKSET blockset;

CR_TEXTURE *bg;
vec2f offs;

SDL_Surface *target_surf;
CR_TEXTURE *target_tex;
CR_TEXTURE *target_preview;
CR_TEXTURE *sidebar;
CR_TEXTURE *height;

CR_TEXTURE *cursor;
float cursorglow = 0;

vec3i selection;

unsigned char selection_angul = 0;
int root = 0;
int rooty = 0;

vec2i size;
vec2i limit;

void angul_objects_load(void)
{

}

void angul_objects_unload(void)
{

}

void angul_objects_run(void)
{

}

void angul_background(void)
{
	cr_parallax_draw(bg, offs.x, offs.y, 1, 1, CR_BG_H | CR_BG_V, CR_NO_FLIP, 0);
	offs.x -= (float)1;
	offs.y += (float)1;
}

void angul_debug(void)
{
	if (cr_debug.active) {

	}
}

char barselection = 0;
bool selected = false;
void sidebar_run(void)
{
	cr_sprite_draw_scaled(sidebar, (float)(root - 8), 0, 0, 0, 8, (float)((crimson.height / 32) + 32), 0, 4);

	cr_spritefont_draw(&cr_devmenu.white, root, 16 + rooty, CR_ALIGN_LEFT, 7, "ANGULATOR 9000", true);
	cr_spritefont_draw(&cr_devmenu.white, root, 24 + rooty, CR_ALIGN_LEFT, 7, "--------------", true);

	cr_spritefont_draw(&cr_devmenu.white, root, crimson.height - 40 + rooty, CR_ALIGN_LEFT, 7, "Block X: ", true);
	cr_counter_draw(&cr_devmenu.white_nums, root + 68, crimson.height - 40 + rooty, CR_ALIGN_LEFT, 7, selection.x);
	cr_spritefont_draw(&cr_devmenu.white, root, crimson.height - 28 + rooty, CR_ALIGN_LEFT, 7, "Block Y: ", true);
	cr_counter_draw(&cr_devmenu.white_nums, root + 68, crimson.height - 28 + rooty, CR_ALIGN_LEFT, 7, selection.y);
	cr_spritefont_draw(&cr_devmenu.white, root, crimson.height - 16 + rooty, CR_ALIGN_LEFT, 7, "Block N: ", true);
	cr_counter_draw(&cr_devmenu.white_nums, root + 68, crimson.height - 16 + rooty, CR_ALIGN_LEFT, 7, selection.z);

	cr_spritefont_draw(&cr_devmenu.white, root + 8, 128 + rooty, CR_ALIGN_LEFT, 7, "Angle (255/360):    /", true);
	cr_counter_draw(&cr_devmenu.white_nums, root + 136 + 8, 128 + rooty, CR_ALIGN_LEFT, 7, anglemap.angles[selection.z]);
	cr_counter_draw(&cr_devmenu.white_nums, root + 168 + 8, 128 + rooty, CR_ALIGN_LEFT, 7, (int)(anglemap.angles[selection.z] * 1.40625));

	if (selected) {
		switch (barselection)
		{
		case 0:
			cr_spritefont_draw(&cr_devmenu.red, root, 128 + rooty, CR_ALIGN_LEFT, 7, ">", true);

			cr_spritefont_draw(&cr_devmenu.red, root + 8, 128 + rooty, CR_ALIGN_LEFT, 7, "Angle (255/360):<   /  >", true);
			cr_counter_draw(&cr_devmenu.red_nums, root + 136 + 8, 128 + rooty, CR_ALIGN_LEFT, 7, anglemap.angles[selection.z]);
			cr_counter_draw(&cr_devmenu.red_nums, root + 168 + 8, 128 + rooty, CR_ALIGN_LEFT, 7, (int)(anglemap.angles[selection.z] * 1.40625));
			break;
		}
	}

	cr_blending_enable(CR_BLEND_ALPHA, 255);
	cr_section_draw_scaled(target_preview, (float)(root + 8), (float)(48 + rooty), (unsigned short)(0 + (selection.x * blockset.width)), (unsigned short)(0 + (selection.y * blockset.height)), (unsigned short)blockset.width, (unsigned short)blockset.height, 0, 0, (float)(blockset.width / 4), (float)(blockset.height / 4), 0, 6);
	cr_blending_disable();

	cr_blending_enable(CR_BLEND_ALPHA, 128);
	cr_section_draw_scaled(target_preview, (float)(root + 168), (float)(48 + rooty), (unsigned short)(0 + (selection.x * blockset.width)), (unsigned short)(0 + (selection.y * blockset.height)), (unsigned short)blockset.width, (unsigned short)blockset.height, 0, 0, (float)(blockset.width / 4), (float)(blockset.height / 4), 0, 7);
	cr_blending_disable();

	bool negate = false;
	for (unsigned char x = 0; x < blockset.width; x++) {
		unsigned char b3 = (unsigned char)(blockset.heightmasks[selection.z][x] >> (8));
		if (b3) { negate = true; break; }
	}
	for (unsigned char x = 0; x < blockset.width; x++) {
		unsigned char b4 = (unsigned char)(blockset.heightmasks[selection.z][x] & 0xff);
		for (unsigned char y = 0; y < b4; y++) {
			if (negate) {
				cr_sprite_draw_scaled(height, (float)(root + 88) + (x * 4), (float)((108 - ((blockset.height - 1) * 4)) + (y * 4) + rooty), 0, 0, 0.5, 0.5, 0, 6);
				cr_sprite_draw_scaled(height, (float)(root + 168) + (x * 4), (float)((108 - ((blockset.height - 1) * 4)) + (y * 4) + rooty), 0, 0, 0.5, 0.5, 0, 6);
			}
			else {
				cr_sprite_draw_scaled(height, (float)(root + 88) + (x * 4), (float)(108 - (y * 4) + rooty), 0, 0, 0.5, 0.5, 0, 6);
				cr_sprite_draw_scaled(height, (float)(root + 168) + (x * 4), (float)(108 - (y * 4) + rooty), 0, 0, 0.5, 0.5, 0, 6);
			}
		}
	}
}

unsigned char holdtimer = 10;
void viewer_run(void)
{
	cr_sprite_draw(target_tex, 16, 16, 0, 3);

	float glow = 0;
	if (!selected) {
		cursorglow += 16;

		if (cursorglow >= 360) {
			cursorglow = 0;
		}
		glow = (float)(324 - (32 * cr_sinf((int)cursorglow * 1)));
	}
	else {
		cursorglow = glow = 360;
	}
		cr_blending_enable(CR_BLEND_ALPHA, (unsigned char)((glow) * 2.8125));
		cr_sprite_draw(cursor, (float)(blockset.height + (selection.x * blockset.width)), (float)(blockset.width + (selection.y * blockset.height)), 0, 4);
		selection.z = selection.y * (limit.x + 1) + selection.x;
		cr_blending_disable();

	if (selected) {
		
	}
	else {

		if (cr_input[CR_P1].A_PRESS) {
			selected = true;
		}

		if (cr_input[CR_P1].B_PRESS && selected) {
			selected = false;
		}

		if (cr_input[CR_P1].UP_PRESS || (cr_input[CR_P1].UP_HOLD && holdtimer == 0)) {
			if (selection.y > 0) {
				selection.y--;
			}
		}

		if (cr_input[CR_P1].DOWN_PRESS || (cr_input[CR_P1].DOWN_HOLD && holdtimer == 0)) {
			if (selection.y < limit.y) {
				selection.y++;
			}
		}

		if (cr_input[CR_P1].LEFT_PRESS || (cr_input[CR_P1].LEFT_HOLD && holdtimer == 0)) {
			if (selection.x > 0) {
				selection.x--;
			}
			else {
				selection.x = limit.x;
				selection.y--;
				if (selection.y < 0) {
					selection.y = limit.y;
				}
			}
		}

		if (cr_input[CR_P1].RIGHT_PRESS || (cr_input[CR_P1].RIGHT_HOLD && holdtimer == 0)) {
			if (selection.x < limit.x) {
				selection.x++;
			}
			else {
				selection.x = 0;
				if (selection.y < limit.y) {
					selection.y++;
				}
				else {
					selection.y = 0;
				}
			}
		}

		if (cr_input[CR_P1].UP_HOLD) {
			if (holdtimer > 0) {
				holdtimer--;
			}
			else {
				holdtimer = 10;
			}
		}
		if (cr_input[CR_P1].UP_RELEASE) {
			holdtimer = 10;
		}

		if (cr_input[CR_P1].DOWN_HOLD) {
			if (holdtimer > 0) {
				holdtimer--;
			}
			else {
				holdtimer = 10;
			}
		}
		if (cr_input[CR_P1].DOWN_RELEASE) {
			holdtimer = 10;
		}

		if (cr_input[CR_P1].LEFT_HOLD) {
			if (holdtimer > 0) {
				holdtimer--;
			}
			else {
				holdtimer = 10;
			}
		}
		if (cr_input[CR_P1].LEFT_RELEASE) {
			holdtimer = 10;
		}

		if (cr_input[CR_P1].RIGHT_HOLD) {
			if (holdtimer > 0) {
				holdtimer--;
			}
			else {
				holdtimer = 10;
			}
		}
		if (cr_input[CR_P1].RIGHT_RELEASE) {
			holdtimer = 10;
		}
	}
}

void angul_update(void)
{
	// X Cam
	if ((blockset.height + (selection.x * blockset.width)) > (crimson.width / 3) + cr_camera.cams[0]->pos.x) {
		cr_camera.cams[0]->pos.x += 8;
	}

	if ((blockset.height + (selection.x * blockset.width)) < (crimson.width / 3) + cr_camera.cams[0]->pos.x) {
		if (cr_camera.cams[0]->pos.x > 0) {
			cr_camera.cams[0]->pos.x -= 8;
		}
		else {
			cr_camera.cams[0]->pos.x = 0;
		}
	}

	// Y Cam
	if ((blockset.height + (selection.y * blockset.height)) > (crimson.height / 2) + cr_camera.cams[0]->pos.y) {
		cr_camera.cams[0]->pos.y += 8;
	}

	if ((blockset.height + (selection.y * blockset.height)) < (crimson.height / 2) + cr_camera.cams[0]->pos.y) {
		if (cr_camera.cams[0]->pos.y > 0) {
			cr_camera.cams[0]->pos.y -= 8;
		}
		else {
			cr_camera.cams[0]->pos.y = 0;
		}
	}

	//if ((blockset.height + (selection.x * blockset.width)) < (crimson.width / 3) + cr_camera.cams[0]->pos.x) {
	//	if (cr_camera.cams[0]->pos.x > 0) {
	//		cr_camera.cams[0]->pos.x -= 8;
	//	}
	//	else {
	//		cr_camera.cams[0]->pos.x = 0;
	//	}
	//}

	root = (int)(crimson.width - 248 + cr_camera.cams[0]->pos.x);
	rooty = (int)(cr_camera.cams[0]->pos.y);

	viewer_run();
	sidebar_run();

	if (cr_input[CR_P1].B_PRESS && !selected) {
		cr_stage_goto(&toolbox);
	}

	angul_background();
}

void angul_init(void)
{

	// Mark angul as initialized
	angul.initialized = true;
}

void angul_load(void)
{
	bg = cr_sprite_load("cd/stages/angul/bg.png", 0x00ff00);

	target_surf = IMG_Load("cd/stages/angul/target.png");
	size.x = target_surf->w;
	size.y = target_surf->h;
	limit.x = (size.x / 16) - 1;
	limit.y = (size.y / 16) - 1;

	target_tex = cr_sprite_load("cd/stages/angul/target.png", 0x000000);
	Uint32 rgb = get_pixel(target_surf, 0, 0);
	target_preview = cr_sprite_load("cd/stages/angul/target.png", rgb);

	sidebar = cr_sprite_load("cd/stages/angul/sidebar.png", 0x000000);
	height = cr_sprite_load("cd/stages/angul/height.png", 0x000000);

	cursor = cr_sprite_load("cd/stages/angul/cursor.png", 0x00ff00);

	cr_stage_load_blockset("cd/stages/angul/blockset.crb", &blockset);
	cr_stage_load_anglemap("cd/stages/angul/anglemap.crd", &anglemap);

	//printf("blockset width/height %d, %d\n", blockset.width, blockset.height);

	// Mark angul as loaded
	angul.loaded = true;
}
void angul_unload(void)
{
	cr_sprite_unload(bg);

	SDL_FreeSurface(target_surf);
	cr_sprite_unload(target_tex);
	cr_sprite_unload(target_preview);

	cr_sprite_unload(sidebar);
	cr_sprite_unload(height);

	cr_sprite_unload(cursor);

	cr_stage_unload_blockset(&blockset);
	cr_stage_unload_anglemap(&anglemap);

	// Mark as NOT loaded and NOT initialized
	angul.loaded = false;
	angul.initialized = false;
}