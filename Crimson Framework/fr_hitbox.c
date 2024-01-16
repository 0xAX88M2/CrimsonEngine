#include "framework.h"

CR_ANIM_DATA hitbox_data;
CR_ANIM hitbox_anim;

// MASTER

void fr_hitbox_master_init(unsigned short total_count)
{
	// If the master is initialized, free the current hitboxes.
	if (fr_hitboxes.initialized) {
		free(fr_hitboxes.hitboxes);
	}
	else {	// Otherwise, load the script for displaying hitboxes.
		cr_script_load("cd/cr_debug/hitboxes.cra", &hitbox_data, &hitbox_anim, 0);
	}

	// Make space for the hitboxes.
	fr_hitboxes.hitboxes = calloc(total_count, sizeof(fr_hitbox));

	// Return if allocation failed.
	if (fr_hitboxes.hitboxes == NULL) {
		return;
	}

	// Set manager's count to new count.
	fr_hitboxes.count = total_count;

	// Reset hitboxes
	fr_hitbox_master_reset();

	fr_hitboxes.initialized = true;
}

void fr_hitbox_master_close(void)
{
	if (fr_hitboxes.initialized) {
		free(fr_hitboxes.hitboxes);
		cr_script_unload(&hitbox_data);
		fr_hitboxes.initialized = false;
	}
}

void fr_hitbox_master_reset(void)
{
	for (unsigned short i = 0; i < fr_hitboxes.count; i++) {
		fr_hitbox_reset(&fr_hitboxes.hitboxes[i]);	// Reset hitbox
		fr_hitboxes.hitboxes[i].id = i;			// Give hitbox its ID
	}
}

// HITBOXES

void fr_hitbox_reset(fr_hitbox* hitbox)
{

	// Normal properties
	hitbox->offset.x = 0;
	hitbox->offset.y = 0;

	hitbox->size.x = 0;
	hitbox->size.y = 0;

	hitbox->flip.x = false;
	hitbox->flip.y = false;

	hitbox->type = 0;
	hitbox->tags = 0;

	hitbox->used = false;
	hitbox->active = false;

	hitbox->col_count = 0;
	for (unsigned char i = 0; i < FR_HITBOX_COL_COUNT; i++) {
		hitbox->colliding_ids[i] = 0;
	}

	hitbox->parent_manager = NULL;

	// Custom Variables
	hitbox->angle = 0;
	hitbox->prox = 0;
	hitbox->stood = false;

	hitbox->hitlag = 0;
	hitbox->power = 0;
	hitbox->damage = 0;

}

void fr_hitbox_activate(unsigned int id, short offset_x, short offset_y, unsigned short width, unsigned short height, unsigned short type, unsigned short tags)
{
	fr_hitbox* hitbox = &fr_hitboxes.hitboxes[id];

	hitbox->offset.x = offset_x;
	hitbox->offset.y = offset_y;

	hitbox->size.x = width;
	hitbox->size.y = height;

	hitbox->type = type;
	hitbox->tags = tags;

	if (hitbox->parent_manager == NULL) {
		hitbox->active = false;
		hitbox->used = false;
		return;
	}

	hitbox->active = true;
	hitbox->used = true;

	hitbox->parent_manager->initialized = true;
}

void fr_hitbox_deactivate(fr_hitbox* hitbox)
{
	// Normal properties
	hitbox->offset.x = 0;
	hitbox->offset.y = 0;

	hitbox->size.x = 0;
	hitbox->size.y = 0;

	hitbox->flip.x = false;
	hitbox->flip.y = false;

	hitbox->type = 0;
	hitbox->tags = 0;

	// Custom Variables
	hitbox->angle = 0;
	hitbox->prox = 0;
	hitbox->stood = false;

	hitbox->hitlag = 0;
	hitbox->power = 0;
	hitbox->damage = 0;

	// Deactivate
	hitbox->active = false;
}

bool fr_hitbox_request(fr_hitbox_manager* manager, unsigned char count)
{
	unsigned char ncount = 0;
	if (manager->box_count < FR_HITBOX_COUNT - 1) {
		for (unsigned short i = 1; i < fr_hitboxes.count; i++) {
			if (!fr_hitboxes.hitboxes[i].used) {
				manager->box_ids[manager->box_count] = i;
				fr_hitboxes.hitboxes[i].parent_manager = manager;
				fr_hitboxes.hitboxes[i].used = true;
				fr_hitboxes.hitboxes[i].active = true;
				manager->initialized = true;
				manager->box_count++;
				ncount++;
			}

			if (ncount >= count || manager->box_count >= FR_HITBOX_COUNT) {
				return true;
			}
		}
	}

	return false;
}

// MANAGERS

void fr_hitbox_manager_prep(fr_hitbox_manager* manager, CR_INSTANCE* inst)
{
	if (inst->initialized) {
		fr_hitbox_manager_align(manager, inst);
		for (unsigned char i = 0; i < manager->box_count; i++) {
			fr_hitbox_reset(&fr_hitboxes.hitboxes[manager->box_ids[i]]);
			manager->box_ids[i] = 0;
		}

		manager->aligned = true;
	}
	else {
		for (unsigned char i = 0; i < FR_HITBOX_COUNT; i++) {
			manager->box_ids[i] = 0;
		}

		manager->aligned = false;
	}

	manager->box_count = 0;
	manager->parent_id = inst->parent_id;
	manager->instance_id = inst->id;

	manager->initialized = false;
}

void fr_hitbox_manager_close(fr_hitbox_manager* manager, CR_INSTANCE* inst)
{

}

void fr_hitbox_manager_align(fr_hitbox_manager* manager, CR_INSTANCE* inst)
{
	for (unsigned char i = 0; i < manager->box_count; i++) {
		fr_hitboxes.hitboxes[manager->box_ids[i]].parent_manager = manager;
	}
}

void fr_hitbox_manager_clear(fr_hitbox_manager* manager)
{
	for (unsigned char i = 0; i < manager->box_count; i++) {
		fr_hitbox_deactivate(&fr_hitboxes.hitboxes[manager->box_ids[i]]);
	}
}

// COLLISIONS
inline bool fr_hitbox_check(fr_hitbox* hbox1, fr_hitbox* hbox2)
{
	//if (hbox1->parent_manager != NULL && hbox2->parent_manager != NULL && hbox1->parent_manager->self_id != hbox2->parent_manager->self_id && hbox1->id != hbox2->id && hbox1->active && hbox1->used && hbox2->active && hbox2->used) {
	if (hbox1->id != hbox2->id && hbox1->used && hbox2->used && hbox1->parent_manager != NULL && hbox2->parent_manager != NULL) {
		unsigned int obj_id_1 = hbox1->parent_manager->parent_id;
		unsigned int inst_id_1 = hbox1->parent_manager->instance_id;

		unsigned int obj_id_2 = hbox2->parent_manager->parent_id;
		unsigned int inst_id_2 = hbox2->parent_manager->instance_id;

		if (obj_id_1 > cr_objects.obj_count || obj_id_2 > cr_objects.obj_count) {
			return false;
		}

		if (inst_id_1 > cr_objects.objects[obj_id_1]->total_count || inst_id_2 > cr_objects.objects[obj_id_2]->total_count) {
			return false;
		}

		if (inst_id_1 == inst_id_2 && obj_id_1 == obj_id_2) {
			return false;
		}

		if (cr_objects.objects == NULL) {
			return false;
		}

		if (cr_objects.objects[obj_id_1]->instance == NULL || cr_objects.objects[obj_id_2]->instance == NULL) {
			return false;
		}

		vec2f pos1;
		pos1.x = cr_objects.objects[obj_id_1]->instance[inst_id_1].pos.x;
		pos1.y = cr_objects.objects[obj_id_1]->instance[inst_id_1].pos.y;
		vec2us size1;
		size1.x = hbox1->size.x;
		size1.y = hbox1->size.y;
		vec2s off1;
		off1.y = hbox1->offset.y;
		off1.x = hbox1->offset.x;
		if (hbox1->flip.x) { off1.x = -off1.x - size1.x; }
		if (hbox1->flip.y) { off1.y = -off1.y - size1.y; }

		vec2f pos2;
		pos2.x = cr_objects.objects[obj_id_2]->instance[inst_id_2].pos.x;
		pos2.y = cr_objects.objects[obj_id_2]->instance[inst_id_2].pos.y;
		vec2us size2;
		size2.x = hbox2->size.x;
		size2.y = hbox2->size.y;
		vec2s off2;
		off2.y = hbox2->offset.y;
		off2.x = hbox2->offset.x;
		if (hbox2->flip.x) { off2.x = -off2.x - size2.x; }
		if (hbox2->flip.y) { off2.y = -off2.y - size2.y; }

		//First check on X axis
		if (((pos1.x + off1.x) + size1.x) > (pos2.x + off2.x)) { //If right side of box 1 is less than left side box 2
			//First check on Y axis
			if (((pos1.y + off1.y) + size1.y) > (pos2.y + off2.y)) {
				//Second check on X axis
				if ((pos1.x + off1.x) < ((pos2.x + off2.x) + size2.x)) { //If left side of box 1 is less than right side of box 2
					//Second check on Y axis
					if ((pos1.y + off1.y) < ((pos2.y + off2.y) + size2.y)) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

void fr_hitbox_update_collisions(void)
{
	//Clear col_count before we run the main checking routine
	for (unsigned int id = 1; id < fr_hitboxes.count; id++) {
		for (unsigned int i = 0; i < fr_hitboxes.hitboxes->col_count; i++) {
			fr_hitboxes.hitboxes[id].colliding_ids[i] = 0;
		}
		fr_hitboxes.hitboxes[id].col_count = 0;
		fr_hitboxes.hitboxes[id].prox = PROX_NONE;

		if (!fr_hitboxes.hitboxes[id].active) {
			//break;
		}

		if (!fr_hitboxes.hitboxes[id].used) {
			//break;
		}

		if (fr_hitboxes.hitboxes[id].parent_manager != NULL) {
			unsigned int oid = fr_hitboxes.hitboxes[id].parent_manager->parent_id;
			unsigned int iid = fr_hitboxes.hitboxes[id].parent_manager->instance_id;
			if (oid > cr_objects.obj_count) {
				break;
			}
			else {
				if (iid > cr_objects.objects[oid]->total_count - 1) {
					break;
				}
			}
			if (cr_objects.objects[oid]->instance[iid].destroyed || !cr_objects.objects[oid]->instance[iid].active || !cr_objects.objects[oid]->instance[iid].initialized) {
				fr_hitbox_deactivate(&fr_hitboxes.hitboxes[id]);
				fr_hitboxes.hitboxes[id].active = false;
				fr_hitboxes.hitboxes[id].used = false;
			}
		}
	}

	for (unsigned int id_0 = 1; id_0 < fr_hitboxes.count; id_0++) { // Main hitbox cycle
		if (fr_hitboxes.hitboxes[id_0].active && fr_hitboxes.hitboxes[id_0].used) { // If hitbox 1 is active 

			for (unsigned int id_1 = 1; id_1 < fr_hitboxes.count; id_1++) { // Secondary hitbox cycle
				if (id_0 != id_1 && fr_hitboxes.hitboxes[id_1].active && fr_hitboxes.hitboxes[id_1].used) { // If hitbox 2 is active and is not itself...
					if (fr_hitbox_check(&fr_hitboxes.hitboxes[id_0], &fr_hitboxes.hitboxes[id_1])) { // Run basic bounding box check, if succesful register col_count.
						if (fr_hitboxes.hitboxes[id_0].col_count < FR_HITBOX_COL_COUNT) {
							fr_hitboxes.hitboxes[id_0].colliding_ids[fr_hitboxes.hitboxes[id_0].col_count++] = id_1;
						}
					}
				}
			}
		}
	}

}

bool fr_hitbox_has_tags(fr_hitbox* box, unsigned short tags)
{
	if ((box->tags & tags) == tags) {
		return true;
	}
	else {
		return false;
	}
}

bool fr_hitbox_check_self_type(fr_hitbox_manager* manager, unsigned short self_type)
{
	if (manager->box_count == 0) { return false; }

	for (unsigned int i = 0; i < manager->box_count; i++) {
		//for (unsigned int j = 0; j < fr_hitboxes.hitboxes[manager->box_ids[i]].col_count; j++) {
		if (fr_hitboxes.hitboxes[manager->box_ids[i]].col_count > 0) {
			if (fr_hitboxes.hitboxes[manager->box_ids[i]].type == self_type) {
				return true;
			}
		}
		//}
	}
	return false;
}

bool fr_hitbox_check_self_tags(fr_hitbox_manager* manager, unsigned short self_tags)
{
	if (manager->box_count == 0) { return false; }

	for (unsigned int i = 0; i < manager->box_count; i++) {
		//for (unsigned int j = 0; j < fr_hitboxes.hitboxes[manager->box_ids[i]].col_count; j++) {
			//if(fr_hitboxes.hitboxes[manager->box_ids[i]].)
		if (fr_hitboxes.hitboxes[manager->box_ids[i]].col_count > 0) {
			if ((fr_hitboxes.hitboxes[manager->box_ids[i]].tags & self_tags) == self_tags) {
				return true;
			}
		}
		//}
	}
	return false;
}

bool fr_hitbox_check_target_type(fr_hitbox_manager* manager, unsigned short target_type)
{
	if (manager->box_count == 0) {
		return false;
	}
	for (unsigned int i = 0; i < manager->box_count; i++) {
		for (unsigned int j = 0; j < fr_hitboxes.hitboxes[manager->box_ids[i]].col_count; j++) {
			unsigned int id = fr_hitboxes.hitboxes[manager->box_ids[i]].colliding_ids[j];
			if (fr_hitboxes.hitboxes[id].type == target_type) {
				return true;
			}
		}
	}
	return false;
}

bool fr_hitbox_check_target_tags(fr_hitbox_manager* manager, unsigned short target_tags)
{
	if (manager->box_count == 0) { return false; }

	for (unsigned int i = 0; i < manager->box_count; i++) {
		for (unsigned int j = 0; j < fr_hitboxes.hitboxes[manager->box_ids[i]].col_count; j++) {
			unsigned int id = fr_hitboxes.hitboxes[manager->box_ids[i]].colliding_ids[j];
			if ((fr_hitboxes.hitboxes[id].tags & target_tags) == target_tags) {
				return true;
			}
		}
	}
	return false;
}

fr_hitbox* fr_hitbox_get_self_info(fr_hitbox_manager* manager)
{
	if (manager->box_count == 0) { return &fr_hitboxes.hitboxes[0]; }

	for (unsigned int i = 0; i < manager->box_count; i++) {
		if (fr_hitboxes.hitboxes[manager->box_ids[i]].col_count > 0) {
			return &fr_hitboxes.hitboxes[manager->box_ids[i]];
		}
	}
	return &fr_hitboxes.hitboxes[0];
}

fr_hitbox* fr_hitbox_get_target_info(fr_hitbox_manager* manager)
{
	if (manager->box_count == 0) { return &fr_hitboxes.hitboxes[0]; }

	for (unsigned int i = 0; i < manager->box_count; i++) {
		for (unsigned int j = 0; j < fr_hitboxes.hitboxes[manager->box_ids[i]].col_count; j++) {
			unsigned int id = fr_hitboxes.hitboxes[manager->box_ids[i]].colliding_ids[j];
			if (fr_hitboxes.hitboxes[id].col_count) {
				return &fr_hitboxes.hitboxes[id];
			}
		}
	}
	return &fr_hitboxes.hitboxes[0];
}

fr_hitbox* fr_hitbox_get_self_info_type(fr_hitbox_manager* manager, unsigned short self_type)
{
	if (manager->box_count == 0) { return &fr_hitboxes.hitboxes[0]; }

	for (unsigned int i = 0; i < manager->box_count; i++) {
		if (fr_hitboxes.hitboxes[manager->box_ids[i]].col_count > 0) {
			if (fr_hitboxes.hitboxes[manager->box_ids[i]].type == self_type) {
				return &fr_hitboxes.hitboxes[manager->box_ids[i]];
			}
		}
	}
	return &fr_hitboxes.hitboxes[0];
}

fr_hitbox* fr_hitbox_get_target_info_type(fr_hitbox_manager* manager, unsigned short target_type)
{
	if (manager->box_count == 0) { return &fr_hitboxes.hitboxes[0]; }

	for (unsigned int i = 0; i < manager->box_count; i++) {
		for (unsigned int j = 0; j < fr_hitboxes.hitboxes[manager->box_ids[i]].col_count; j++) {
			unsigned int id = fr_hitboxes.hitboxes[manager->box_ids[i]].colliding_ids[j];
			if ((fr_hitboxes.hitboxes[id].type) == target_type) {
				if (fr_hitboxes.hitboxes[id].parent_manager != NULL) {
					return &fr_hitboxes.hitboxes[id];
				}
			}
		}
	}
	return &fr_hitboxes.hitboxes[0];
}

fr_hitbox* fr_hitbox_get_self_info_tags(fr_hitbox_manager* manager, unsigned short self_tags)
{
	if (manager->box_count == 0) { return &fr_hitboxes.hitboxes[0]; }

	for (unsigned int i = 0; i < manager->box_count; i++) {
		if (fr_hitboxes.hitboxes[manager->box_ids[i]].col_count > 0) {
			if ((fr_hitboxes.hitboxes[manager->box_ids[i]].tags & self_tags) == self_tags) {
				return &fr_hitboxes.hitboxes[manager->box_ids[i]];
			}
		}
	}
	return &fr_hitboxes.hitboxes[0];
}

fr_hitbox* fr_hitbox_get_target_info_tags(fr_hitbox_manager* manager, unsigned short target_tags)
{
	if (manager->box_count == 0) { return &fr_hitboxes.hitboxes[0]; }

	for (unsigned int i = 0; i < manager->box_count; i++) {
		for (unsigned int j = 0; j < fr_hitboxes.hitboxes[manager->box_ids[i]].col_count; j++) {
			unsigned int id = fr_hitboxes.hitboxes[manager->box_ids[i]].colliding_ids[j];
			if ((fr_hitboxes.hitboxes[id].tags & target_tags) == target_tags) {
				return &fr_hitboxes.hitboxes[id];
			}
		}
	}
	return &fr_hitboxes.hitboxes[0];
}

bool fr_hitbox_check_both_tags(fr_hitbox_manager* manager, unsigned short self_tags, unsigned short target_tags)
{
	if (manager->box_count == 0) { return &fr_hitboxes.hitboxes[0]; }

	for (unsigned int i = 0; i < manager->box_count; i++) { // Cycle through manager
		if (fr_hitboxes.hitboxes[manager->box_ids[i]].col_count > 0) { // If a hitbox has some collisions
			if ((fr_hitboxes.hitboxes[manager->box_ids[i]].tags & self_tags) == self_tags) { // Check to see if its tags match
				for (unsigned int j = 0; j < fr_hitboxes.hitboxes[manager->box_ids[i]].col_count; j++) { // If so, cycle through its colliding hitboxes to see if any of those tags match the target
					if ((fr_hitboxes.hitboxes[fr_hitboxes.hitboxes[manager->box_ids[i]].colliding_ids[j]].tags & target_tags) == target_tags) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool fr_hitbox_check_both_type(fr_hitbox_manager* manager, unsigned short self_type, unsigned short target_type)
{
	if (manager->box_count == 0) { return &fr_hitboxes.hitboxes[0]; }

	for (unsigned int i = 0; i < manager->box_count; i++) { // Cycle through manager
		if (fr_hitboxes.hitboxes[manager->box_ids[i]].col_count > 0) { // If a hitbox has some collisions
			if (fr_hitboxes.hitboxes[manager->box_ids[i]].type == self_type) { // Check to see if its tags match
				for (unsigned int j = 0; j < fr_hitboxes.hitboxes[manager->box_ids[i]].col_count; j++) { // If so, cycle through its colliding hitboxes to see if any of those tags match the target
					if (fr_hitboxes.hitboxes[fr_hitboxes.hitboxes[manager->box_ids[i]].colliding_ids[j]].type == target_type) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

fr_hitbox* fr_hitbox_get_both_tags_info(fr_hitbox_manager* manager, unsigned short self_tags, unsigned short target_tags)
{
	if (manager->box_count == 0) { return &fr_hitboxes.hitboxes[0]; }

	for (unsigned int i = 0; i < manager->box_count; i++) { // Cycle through manager
		if (fr_hitboxes.hitboxes[manager->box_ids[i]].col_count > 0) { // If a hitbox has some collisions
			if ((fr_hitboxes.hitboxes[manager->box_ids[i]].tags & self_tags) == self_tags) { // Check to see if its tags match
				for (unsigned int j = 0; j < fr_hitboxes.hitboxes[manager->box_ids[i]].col_count; j++) { // If so, cycle through its colliding hitboxes to see if any of those tags match the target
					if ((fr_hitboxes.hitboxes[fr_hitboxes.hitboxes[manager->box_ids[i]].colliding_ids[j]].tags & target_tags) == target_tags) {
						return &fr_hitboxes.hitboxes[fr_hitboxes.hitboxes[manager->box_ids[i]].colliding_ids[j]];
					}
				}
			}
		}
	}
	return &fr_hitboxes.hitboxes[0];
}

fr_hitbox* fr_hitbox_get_both_type_info(fr_hitbox_manager* manager, unsigned short self_type, unsigned short target_type)
{
	if (manager->box_count == 0) { return &fr_hitboxes.hitboxes[0]; }

	for (unsigned int i = 0; i < manager->box_count; i++) { // Cycle through manager
		if (fr_hitboxes.hitboxes[manager->box_ids[i]].col_count > 0) { // If a hitbox has some collisions
			if (fr_hitboxes.hitboxes[manager->box_ids[i]].tags == self_type) { // Check to see if its tags match
				for (unsigned int j = 0; j < fr_hitboxes.hitboxes[manager->box_ids[i]].col_count; j++) { // If so, cycle through its colliding hitboxes to see if any of those tags match the target
					if (fr_hitboxes.hitboxes[fr_hitboxes.hitboxes[manager->box_ids[i]].colliding_ids[j]].type == target_type) {
						return &fr_hitboxes.hitboxes[fr_hitboxes.hitboxes[manager->box_ids[i]].colliding_ids[j]];
					}
				}
			}
		}
	}
	return &fr_hitboxes.hitboxes[0];
}

fr_hitbox* fr_hitbox_get(unsigned short hitbox_id)
{
	return &fr_hitboxes.hitboxes[hitbox_id];
}

void fr_hitbox_proximity_update(fr_hitbox* hbox1, fr_hitbox* hbox2)
{

	vec2f pos1;
	vec2f pos2;

	fr_hitbox_get_pos(hbox1, &pos1.x, &pos1.y);
	fr_hitbox_get_pos(hbox1, &pos2.x, &pos2.y);

	// Calculate the player's distance from the left side of the hitbox
	float x_dist = (pos1.x + hbox1->size.x) - (pos2.x - (hbox2->size.x / 2));
	// Calculate the player's distance from the top of the hitbox
	float y_dist = (pos1.y + hbox1->size.y) - (pos2.y - (hbox2->size.y / 2));

	if (x_dist > hbox2->size.x) {
		x_dist = (pos2.x + (hbox2->size.x / 2)) - (pos1.x - hbox1->size.x);
		x_dist = -(x_dist);
	}
	if (y_dist > hbox2->size.y) {
		y_dist = (pos2.y + (hbox2->size.y / 2)) - (pos1.y - hbox1->size.y);
		y_dist = -(y_dist);
	}

	// If the player is on the top or bottom of the object...
	if (abs(x_dist) > abs(y_dist)) {
		// If Sonic is on top of the object...
		if (y_dist >= 0) {

			hbox1->prox = (hbox1->prox | PROX_TOP);
			hbox2->prox = (hbox2->prox | PROX_TOP);
		}

		// Otherwise, if the player is below the object...
		else {
			hbox1->prox = (hbox1->prox | PROX_BOTTOM);
			hbox2->prox = (hbox2->prox | PROX_BOTTOM);
		}
	}

	// Otherwise, if the player is on one of the sides of the object...
	else {
		// If Sonic is to the right of the object...
		if (x_dist <= (hbox2->size.x / 1)) {
			hbox1->prox = (hbox1->prox | PROX_RIGHT);
			hbox2->prox = (hbox2->prox | PROX_RIGHT);
		}
		// If Sonic is to the left of the object...
		if (x_dist >= (hbox2->size.x / 1)) {
			hbox1->prox = (hbox1->prox | PROX_LEFT);
			hbox2->prox = (hbox2->prox | PROX_LEFT);
		}
	}
}

bool fr_hitbox_proximity_check(unsigned char prox, fr_hitbox * hbox1, fr_hitbox * hbox2)
{
	if ((hbox1->prox & prox) && (hbox2->prox & prox)) {
		return (true);		// Indicate that the player is touching the object at the given proximity
	}
	return false;
}

void fr_hitbox_adjust(fr_hitbox_manager * manager, unsigned char hitbox_id, short offset_x, short offset_y, unsigned short width, unsigned short height, unsigned short type, unsigned short tags)
{
	fr_hitboxes.hitboxes[manager->box_ids[hitbox_id]].active = true;
	fr_hitboxes.hitboxes[manager->box_ids[hitbox_id]].offset.x = offset_x;
	fr_hitboxes.hitboxes[manager->box_ids[hitbox_id]].offset.y = offset_y;

	fr_hitboxes.hitboxes[manager->box_ids[hitbox_id]].size.x = width;
	fr_hitboxes.hitboxes[manager->box_ids[hitbox_id]].size.y = height;

	fr_hitboxes.hitboxes[manager->box_ids[hitbox_id]].type = type;
	fr_hitboxes.hitboxes[manager->box_ids[hitbox_id]].tags = tags;
}

void fr_hitbox_move(fr_hitbox_manager * manager, unsigned char hitbox_id, short offset_x, short offset_y, unsigned short width, unsigned short height)
{
	fr_hitboxes.hitboxes[manager->box_ids[hitbox_id]].active = true;
	fr_hitboxes.hitboxes[manager->box_ids[hitbox_id]].offset.x = offset_x;
	fr_hitboxes.hitboxes[manager->box_ids[hitbox_id]].offset.y = offset_y;

	fr_hitboxes.hitboxes[manager->box_ids[hitbox_id]].size.x = width;
	fr_hitboxes.hitboxes[manager->box_ids[hitbox_id]].size.y = height;
}

void fr_hitbox_manager_flip(fr_hitbox_manager * manager, bool flip_x, bool flip_y)
{
	for (unsigned int i = 0; i < manager->box_count; i++) {
		fr_hitboxes.hitboxes[manager->box_ids[i]].flip.x = flip_x;
		fr_hitboxes.hitboxes[manager->box_ids[i]].flip.y = flip_y;
	}
}

void fr_hitbox_get_pos(fr_hitbox *hitbox, float* pos_x, float* pos_y)
{
	if (hitbox == NULL) {
		return;

	}
	if (hitbox->parent_manager == NULL) {
		return;
	}

	unsigned int obj_id = hitbox->parent_manager->parent_id;
	unsigned int inst_id = hitbox->parent_manager->instance_id;

	if (obj_id >= cr_objects.obj_count) {
		return;
	}
	else {
		if (inst_id >= cr_objects.objects[obj_id]->total_count) {
			return;
		}
	}

	if (!hitbox->flip.x) {
		*pos_x = cr_objects.objects[obj_id]->instance[inst_id].pos.x + hitbox->offset.x;
	}
	else {
		*pos_x = cr_objects.objects[obj_id]->instance[inst_id].pos.x - hitbox->offset.x - hitbox->size.x;
	}

	if (!hitbox->flip.y) {
		*pos_y = cr_objects.objects[obj_id]->instance[inst_id].pos.y + hitbox->offset.y;
	}
	else {
		*pos_y = cr_objects.objects[obj_id]->instance[inst_id].pos.y - hitbox->offset.y - hitbox->size.y;
	}
}

void fr_hitbox_get_obj_pos(fr_hitbox *hitbox, float* pos_x, float* pos_y)
{
	if (hitbox == NULL) {
		return;
	}

	if (hitbox->parent_manager == NULL) {
		return;
	}

	unsigned int obj_id = hitbox->parent_manager->parent_id;
	unsigned int inst_id = hitbox->parent_manager->instance_id;

	if (obj_id >= cr_objects.obj_count) {
		return;
	}
	else {
		if (inst_id >= cr_objects.objects[obj_id]->total_count) {
			return;
		}
	}

	*pos_x = cr_objects.objects[obj_id]->instance[inst_id].pos.x;
	*pos_y = cr_objects.objects[obj_id]->instance[inst_id].pos.y;
}

void fr_hitbox_get_obj_size(fr_hitbox *hitbox, unsigned int* width, unsigned int* height)
{
	if (hitbox == NULL) {
		return;
	}

	if (hitbox->parent_manager == NULL) {
		return;
	}

	unsigned int obj_id = hitbox->parent_manager->parent_id;
	unsigned int inst_id = hitbox->parent_manager->instance_id;

	if (obj_id >= cr_objects.obj_count) {
		return;
	}
	else {
		if (inst_id >= cr_objects.objects[obj_id]->total_count) {
			return;
		}
	}

	*width = cr_objects.objects[obj_id]->instance[inst_id].size.x;
	*height = cr_objects.objects[obj_id]->instance[inst_id].size.y;
}

void fr_hitbox_id_get_pos(unsigned int hitbox_id, float* pos_x, float* pos_y)
{
	if (fr_hitboxes.hitboxes[hitbox_id].parent_manager == NULL) {
		return;
	}

	unsigned int obj_id = fr_hitboxes.hitboxes[hitbox_id].parent_manager->parent_id;
	unsigned int inst_id = fr_hitboxes.hitboxes[hitbox_id].parent_manager->instance_id;

	if (obj_id >= cr_objects.obj_count) {
		return;
	}
	else {
		if (inst_id >= cr_objects.objects[obj_id]->total_count) {
			return;
		}
	}

	if (!fr_hitboxes.hitboxes[hitbox_id].flip.x) {
		*pos_x = cr_objects.objects[obj_id]->instance[inst_id].pos.x + fr_hitboxes.hitboxes[hitbox_id].offset.x;
	}
	else {
		*pos_x = cr_objects.objects[obj_id]->instance[inst_id].pos.x - fr_hitboxes.hitboxes[hitbox_id].offset.x - fr_hitboxes.hitboxes[hitbox_id].size.x;
	}

	if (!fr_hitboxes.hitboxes[hitbox_id].flip.y) {
		*pos_y = cr_objects.objects[obj_id]->instance[inst_id].pos.y + fr_hitboxes.hitboxes[hitbox_id].offset.y;
	}
	else {
		*pos_y = cr_objects.objects[obj_id]->instance[inst_id].pos.y - fr_hitboxes.hitboxes[hitbox_id].offset.y - fr_hitboxes.hitboxes[hitbox_id].size.y;
	}
}

void fr_hitbox_id_get_obj_pos(unsigned int hitbox_id, float* pos_x, float* pos_y)
{
	if (fr_hitboxes.hitboxes[hitbox_id].parent_manager == NULL) {
		return;
	}

	unsigned int obj_id = fr_hitboxes.hitboxes[hitbox_id].parent_manager->parent_id;
	unsigned int inst_id = fr_hitboxes.hitboxes[hitbox_id].parent_manager->instance_id;

	if (obj_id >= cr_objects.obj_count) {
		return;
	}
	else {
		if (inst_id >= cr_objects.objects[obj_id]->total_count) {
			return;
		}
	}

	*pos_x = cr_objects.objects[obj_id]->instance[inst_id].pos.x;
	*pos_y = cr_objects.objects[obj_id]->instance[inst_id].pos.y;
}

void fr_hitbox_id_get_obj_size(unsigned int hitbox_id, unsigned int* width, unsigned int* height)
{
	if (fr_hitboxes.hitboxes[hitbox_id].parent_manager == NULL) {
		return;
	}

	unsigned int obj_id = fr_hitboxes.hitboxes[hitbox_id].parent_manager->parent_id;
	unsigned int inst_id = fr_hitboxes.hitboxes[hitbox_id].parent_manager->instance_id;

	if (obj_id >= cr_objects.obj_count) {
		return;
	}
	else {
		if (inst_id >= cr_objects.objects[obj_id]->total_count) {
			return;
		}
	}

	*width = cr_objects.objects[obj_id]->instance[inst_id].size.x;
	*height = cr_objects.objects[obj_id]->instance[inst_id].size.y;
}

void fr_hitbox_object_run(CR_OBJECT* parent, void(*obj_update)(void* ent), void(*screen_enter)(void* ent), void (*screen_exit)(void* ent), void(*hitbox_request)(void* ent), unsigned short hitbox_offset, int cam_buffer_x, int cam_buffer_y)
{
	// If the object is loaded...
	if (parent->loaded) {
		// If there is at least one of the object in total...
		if (parent->total_count > 0) {
			// For every instance of the object in memory...
			for (unsigned int i = 0; i < parent->total_count; i++) {
				// If the object's init routine has been run...
				unsigned long obj_pointer = cr_object_get_pointer(parent, i);
				if (parent->instance[i].initialized) {
					// Calculate and store the raw pointer of the current object's child memory
					// If the current instance is active and hasn't been destroyed...
					if (parent->instance[i].active == true && parent->instance[i].destroyed == false) {
						// Variable to determine if the object should run. Runs by default.
						bool do_run = true;

						// If the cam buffers are above -1, run the camera check.
						if (cam_buffer_x > -1 || cam_buffer_y > -1) {
							// If the object is on-screen, let the object run.
							if (cr_coord_is_within(parent->instance[i].pos.x, parent->instance[i].pos.y,
								(float)-((parent->instance[i].size.x) + cam_buffer_x), (float)(crimson.width + parent->instance[i].size.x + cam_buffer_x),
								(float)-((parent->instance[i].size.y) + cam_buffer_y), (float)(crimson.height + parent->instance[i].size.y + cam_buffer_y),
								true)) {
								do_run = true;

								if (!parent->instance[i].onscreen) {
									if (screen_enter != NULL) { screen_enter((void*)(obj_pointer)); }
									parent->instance[i].onscreen = true;
								}
							}
							else { // If the object is offscreen...
								// free up the hitboxes and prepare the manager for next time, and don't let the object run.
								fr_hitbox_manager_prep((fr_hitbox_manager*)(obj_pointer + hitbox_offset), &parent->instance[i]);

								if (parent->instance[i].onscreen) {
									if (screen_exit != NULL) { screen_exit((void*)(obj_pointer)); }
									parent->instance[i].onscreen = false;
								}

								do_run = false;
							}
						}
						else {
							if (!parent->instance[i].onscreen) {
								if (screen_enter != NULL) { screen_enter((void*)(obj_pointer)); }
								parent->instance[i].onscreen = true;
							}
						}

						// If the object is set to run...
						if (do_run) {
							// If the object has defined a hitbox requesting routine...
							if (hitbox_request != NULL) {
								// If the object's hitbox manager is not algined, run the align function.
								if (!(*(bool*)(obj_pointer + hitbox_offset + offsetof(fr_hitbox_manager, aligned)))) {
									fr_hitbox_manager_align((fr_hitbox_manager*)(obj_pointer + hitbox_offset), &parent->instance[i]);
								}
								// If the object's manager is not already initialized, call the request hitbox function.
								if (hitbox_request != NULL && !(*(bool*)(obj_pointer + hitbox_offset + offsetof(fr_hitbox_manager, initialized)))) {
									hitbox_request((void*)(obj_pointer));
								}
							}
							obj_update((void*)(obj_pointer)); // Update the current object instance
						}
					}
					else { // If the object is not running, free up the hitboxes and prepare the manager for next time.
						fr_hitbox_manager_prep((fr_hitbox_manager*)(obj_pointer + hitbox_offset), &parent->instance[i]);
					}
				}
				else { // If the object is not running, free up the hitboxes and prepare the manager for next time.
					fr_hitbox_manager_prep((fr_hitbox_manager*)(obj_pointer + hitbox_offset), &parent->instance[i]);
				}
			}
		}
	}
}

// DEBUG

void fr_hitbox_debug_draw(void)
{
	if (cr_debug.game_debug) {
		//printf("count %d\n", fr_hitboxes.count);
		for (int i = 1; i < fr_hitboxes.count; i++) { // Cycle through master list
			if (fr_hitboxes.hitboxes[i].active && fr_hitboxes.hitboxes[i].used) { // If the hitbox isn't used

				if (fr_hitboxes.hitboxes[i].parent_manager == NULL) {
					break;
				}

				int obj_id = fr_hitboxes.hitboxes[i].parent_manager->parent_id;
				int inst_id = fr_hitboxes.hitboxes[i].parent_manager->instance_id;

				if (obj_id > cr_objects.obj_count) {
					break;
				}
				else {
					if (inst_id > cr_objects.objects[obj_id]->total_count - 1) {
						break;
					}
				}

				vec2f pos;
				pos.x = cr_objects.objects[obj_id]->instance[inst_id].pos.x;
				pos.y = cr_objects.objects[obj_id]->instance[inst_id].pos.y;
				vec2f off;
				off.x = fr_hitboxes.hitboxes[i].offset.x;
				off.y = fr_hitboxes.hitboxes[i].offset.y;
				vec2f size;
				size.x = fr_hitboxes.hitboxes[i].size.x;
				size.y = fr_hitboxes.hitboxes[i].size.y;
				if (fr_hitboxes.hitboxes[i].flip.x) { off.x = -off.x - size.x; }
				if (fr_hitboxes.hitboxes[i].flip.y) { off.y = -off.y - size.y; }

				cr_blending_enable(CR_BLEND_ALPHA, 128);
				cr_anim_display_frame_scaled(&hitbox_anim, pos.x + off.x, pos.y + off.y, 0, 0, size.x, size.y, (unsigned char)fr_hitboxes.hitboxes[i].type, 0, 7);
				cr_blending_disable();
				cr_counter_draw(&cr_devmenu.white_nums, (int)(pos.x + off.x), (int)(pos.y + off.y), 0, 7, fr_hitboxes.hitboxes[i].id);
				cr_counter_draw(&cr_devmenu.white_nums, (int)(pos.x + off.x), (int)(pos.y + off.y + 8), 0, 7, fr_hitboxes.hitboxes[i].col_count);
			}
		}
	}
}