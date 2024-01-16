#include "crimson.h"

//FUNCTIONS

void cr_camera_init(CR_CAMERA *cam)
{
	cam->limit.up = cr_stage.current->cam_limit.up;
	cam->limit.down = cr_stage.current->cam_limit.down + crimson.height;
	cam->limit.left = cr_stage.current->cam_limit.left;
	cam->limit.right = cr_stage.current->cam_limit.right + crimson.width;

	cam->bound.up = cr_stage.current->level_limit.up;
	cam->bound.down = cr_stage.current->level_limit.down + crimson.height;
	cam->bound.left = cr_stage.current->level_limit.left;
	cam->bound.right = cr_stage.current->level_limit.right + crimson.width;
}

void cr_camera_apply(CR_CAMERA *target_camera)
{
	cr_defaultcam.pos.x = target_camera->pos.x;
	cr_defaultcam.pos.y = target_camera->pos.y;

	cr_defaultcam.tru.x = target_camera->tru.x;
	cr_defaultcam.tru.y = target_camera->tru.y;

	cr_defaultcam.target.x = target_camera->target.x;
	cr_defaultcam.target.y = target_camera->target.y;

	cr_defaultcam.mode = target_camera->mode;

	cr_defaultcam.step_limit = target_camera->step_limit;
}

void cr_camera_update(void)
{

	//if (cr_defaultcam.pos.x < 0) {
	//	cr_defaultcam.pos.x = 0;
	//}
	//if (cr_defaultcam.pos.y < 0) {
	//	cr_defaultcam.pos.y = 0;
	//}

	for (int i = 0; i < cr_camera.count; i++) {

		if (cr_camera.count > 0) {
			cr_camera.cams[i]->last.x = cr_camera.cams[i]->tru.x;
			cr_camera.cams[i]->last.y = cr_camera.cams[i]->tru.y;

			int screen_top = (int)cr_camera.cams[i]->pos.y - (int)crimson.game_height;
			int screen_bottom = (int)cr_camera.cams[i]->pos.y + (int)crimson.game_height;
			int screen_left = (int)cr_camera.cams[i]->pos.x - (int)crimson.game_width;
			int screen_right = (int)cr_camera.cams[i]->pos.x + (int)crimson.game_width;

			if (cr_camera.cams[i]->pos.x < 0) {
				cr_camera.cams[i]->pos.x = 0;
			}

			if (cr_camera.cams[i]->pos.y < 0) {
				cr_camera.cams[i]->pos.y = 0;
			}

			if (cr_stage.current != NULL) {
				if (cr_stage.current->tilemap_count > 0) {
					if (cr_camera.cams[i]->pos.x > ((cr_stage.current->tilemaps[0].chunk_width * cr_stage.current->tilemaps[0].tile_width) * cr_stage.current->tilemaps[0].tilemap_width) - crimson.game_width) {
						cr_camera.cams[i]->pos.x = ((cr_stage.current->tilemaps[0].chunk_width * cr_stage.current->tilemaps[0].tile_width) * cr_stage.current->tilemaps[0].tilemap_width) - crimson.game_width;
					}

					if (cr_camera.cams[i]->pos.y > ((cr_stage.current->tilemaps[0].chunk_height * cr_stage.current->tilemaps[0].tile_height) * cr_stage.current->tilemaps[0].tilemap_height) - crimson.game_height) {
						cr_camera.cams[i]->pos.y = ((cr_stage.current->tilemaps[0].chunk_height * cr_stage.current->tilemaps[0].tile_height) * cr_stage.current->tilemaps[0].tilemap_height) - crimson.game_height;
					}
				}

				if (cr_camera.cams[i]->pos.y < cr_camera.cams[i]->limit.up) {
					cr_camera.cams[i]->pos.y = cr_camera.cams[i]->limit.up;
				}

				if (cr_camera.cams[i]->pos.y > cr_camera.cams[i]->limit.down) {
					cr_camera.cams[i]->pos.y = cr_camera.cams[i]->limit.down;
				}

				if (cr_camera.cams[i]->pos.x < cr_camera.cams[i]->limit.left) {
					cr_camera.cams[i]->pos.x = cr_camera.cams[i]->limit.left;
				}

				if (cr_camera.cams[i]->pos.x > cr_camera.cams[i]->limit.right) {
					cr_camera.cams[i]->pos.x = cr_camera.cams[i]->limit.right;
				}
			}
		}
	}
}

void cr_camera_add(CR_CAMERA *cam)
{
	cr_camera.count++;

	CR_CAMERA **cams = realloc(cr_camera.cams, cr_camera.count * sizeof(CR_CAMERA));

	if (cams != NULL) {
		cr_camera.cams = cams;
	}

	cr_camera.cams[cr_camera.count - 1] = cam;

	cr_camera.initialized = true;
}

void cr_camera_replace(CR_CAMERA *cam, unsigned char index)
{
	if (index >= cr_camera.count) {
		cr_camera_add(cam);
	}
	else {
		cr_camera.cams[index] = cam;
	}

	cr_camera_init(cam);
}

void cr_camera_remove(void)
{
	if (cr_camera.count > 0) {
		cr_camera.count--;

		CR_CAMERA **cams = realloc(cr_camera.cams, cr_camera.count + 1 * sizeof(CR_CAMERA));

		if (cams != NULL) {
			cr_camera.cams = cams;
		}
	}
}

void cr_camera_set_target_cam(CR_CAMERA *cam)
{
	//cr_camera.ta
}

// Splitscreen
void cr_camera_manager_reset(void)
{
	if (cr_camera.initialized) {
		if (cr_camera.count > 0) {
			for (int i = 0; i < cr_camera.count; i++) {
				cr_camera_remove();
			}
			cr_camera.count = 0;
		}
	}
}

void cr_screen_add(void)
{
	if (cr_drawing.active_screens < CR_SCREEN_COUNT - 1) {
		cr_drawing.active_screens++;
	}
}

void cr_screen_remove(void)
{
	if (cr_drawing.active_screens > 0) {
		cr_camera_remove();

		cr_drawing.active_screens--;
	}
}

// Helpers
void cr_camera_set_pos(unsigned char cam_id, float pos_x, float pos_y)
{
	if (cr_camera.cams[cam_id] == NULL) {
		return;
	}

	cr_camera.cams[cam_id]->pos.x = pos_x;
	cr_camera.cams[cam_id]->pos.y = pos_y;
}

void cr_camera_add_pos(unsigned char cam_id, float inc_x, float inc_y)
{
	if (cr_camera.cams[cam_id] == NULL) {
		return;
	}

	cr_camera.cams[cam_id]->pos.x += inc_x;
	cr_camera.cams[cam_id]->pos.y += inc_y;
}

void cr_camera_set_pos_p(CR_CAMERA* cam, float pos_x, float pos_y)
{
	if (cam == NULL) {
		return;
	}

	cam->pos.x = pos_x;
	cam->pos.y = pos_y;
}

void cr_camera_add_pos_p(CR_CAMERA* cam, float inc_x, float inc_y)
{
	if (cam == NULL) {
		return;
	}

	cam->pos.x += inc_x;
	cam->pos.y += inc_y;
}
