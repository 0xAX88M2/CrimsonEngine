#include "crimson.h"

#define LAYOUT_GROUP_SIZE (11)

void cr_object_manager_reset(void)
{

#if CR_DEBUG_OBJECTS
	printf("Resetting object manager...\n");
#endif // CR_DEBUG_OBJECTS

	if (cr_objects.obj_initialized) {
		for (unsigned int id = 0; id < cr_objects.obj_count; id++) {
			if (!cr_objects.objects[id]->global) {
				cr_objects.objects[id]->unload();
				cr_object_free(cr_objects.objects[id]);
			}
		}
		free(cr_objects.objects);
	}

	cr_objects.obj_count = 0;
	cr_objects.obj_initialized = false;
}

void cr_object_manager_close(void)
{

#if CR_DEBUG_OBJECTS
	printf("Resetting object manager...\n");
#endif // CR_DEBUG_OBJECTS

	// Normal objs
	if (cr_objects.obj_initialized) {
		for (unsigned int id = 0; id < cr_objects.obj_count; id++) {
			cr_objects.objects[id]->unload();
			cr_object_free(cr_objects.objects[id]);
		}
		free(cr_objects.objects);
	}

	cr_objects.obj_count = 0;
	cr_objects.obj_initialized = false;

	// Globals
	for (unsigned int id = 0; id < cr_objects.glob_count; id++) {
		cr_objects.globals[id]->global_added = false;
	}

	if (cr_objects.glob_initialized) {
		free(cr_objects.globals);
	}

	cr_objects.glob_count = 0;
	cr_objects.glob_initialized = false;
}

void cr_object_reload_globals(void)
{
	for (unsigned int i = 0; i < cr_objects.glob_count; i++) {
		cr_objects.globals[i]->load(NULL);
	}
}

void cr_object_manager_add(CR_OBJECT* parent)
{
	//if (parent->initialized) { return; }

	if (cr_objects.obj_count == 0) {
		cr_objects.objects = calloc(1, sizeof(CR_OBJECT));
	}
	else {

		CR_OBJECT** objects = realloc(cr_objects.objects, cr_objects.obj_count * sizeof(CR_OBJECT));

		if (objects != NULL) {
			cr_objects.objects = objects;
		}
		else {
#if CR_DEBUG_OBJECTS
			printf("Crimson Debug: FATAL ERROR, COULD NOT REALLOC OBJECT MEMORY. EXITING.\n");
#endif
			crimson.exitcode = 1;
		}
	}
	cr_objects.objects[cr_objects.obj_count] = parent;
	parent->obj_id = cr_objects.obj_count;
	cr_objects.obj_count++;

	cr_objects.obj_initialized = true;

	if (parent->global && !parent->global_added) {
		if (cr_objects.glob_count == 0) {
			cr_objects.globals = calloc(1, sizeof(CR_OBJECT));
		}
		else {

			CR_OBJECT** globals = realloc(cr_objects.globals, cr_objects.glob_count * sizeof(CR_OBJECT));

			if (globals != NULL) {
				cr_objects.globals = globals;
			}
			else {
#if CR_DEBUG_OBJECTS
				printf("Crimson Debug: FATAL ERROR, COULD NOT REALLOC OBJECT MEMORY. EXITING.\n");
#endif
				crimson.exitcode = 1;
			}
		}
		parent->global_added = true;

		cr_objects.globals[cr_objects.glob_count] = parent;
		cr_objects.glob_count++;

		cr_objects.glob_initialized = true;
	}
}

void cr_object_init(const char* filepath, CR_OBJECT* parent, void** custom_objects, void(*load)(const char* const filepath), void(*unload)(void), void(*init)(void* ent), void(*align)(void), void(*run)(void), size_t custom_obj_size, unsigned short buffer_threshold)
{
	if (parent->loaded) {
		cr_destroy_all(parent);
	}

	// Determine to run layout-set routine
	bool loadlayout = false;
	if (filepath != NULL) {
		loadlayout = true;
	}

	//Set custom_obj_size
	parent->custom_obj_size = custom_obj_size;
	parent->total_count = 0;
	parent->active_count = 0;

	parent->load = load;
	parent->unload = unload;
	parent->init = init; //Set init function
	parent->align = align; //Set get function
	parent->run = run;

	parent->buffer_threshold = buffer_threshold; //Buffer amount for when new instances are created

	parent->objects = custom_objects;

	cr_object_free(parent);

	//Set up instance status array
	parent->instance = calloc(buffer_threshold, sizeof(CR_INSTANCE));
	*parent->objects = calloc(buffer_threshold, parent->custom_obj_size);

	parent->obj_id = cr_objects.obj_count;
	for (int i = 0; i < buffer_threshold; i++) {
		if (parent->instance != NULL) {
			cr_instance_init(parent, &parent->instance[i], i, false, true, 0, 0, 0, 0, crimson.default_spawn_layer);
			parent->instance[i].initialized = false;
		}
	}

	parent->total_count = buffer_threshold;

	parent->align();

	cr_object_manager_add(parent);
	parent->initialized = true;

	//Run set routine
	if (loadlayout) {
		cr_layout_load(filepath);
		cr_layout_set(parent);
	}

	//Add object to the manager registry
	//if (!parent->initialized) {
	//}
}
void cr_instance_init(CR_OBJECT* parent, CR_INSTANCE* instance, unsigned int obj_id, bool active, bool destroyed, char type, float pos_x, float pos_y, float pos_z, unsigned char layer)
{
	instance->parent_id = parent->obj_id;
	instance->id = obj_id;
	instance->type = type;
	instance->subtype = cr_objects.preset_subtype;
	instance->mode = 0;
	instance->active = active;
	instance->destroyed = destroyed;
	instance->onscreen = false;
	instance->initialized = false;

	instance->pos.x = pos_x;
	instance->pos.y = pos_y;
	instance->pos.z = pos_z;
	instance->spd.x = 0;
	instance->spd.y = 0;
	instance->spd.z = 0;
	instance->flip.x = false;
	instance->flip.y = false;
	instance->flip.z = false;
	instance->scale.x = 1;
	instance->scale.y = 1;
	instance->scale.z = 1;
	instance->size.x = 1;
	instance->size.y = 1;
	instance->size.z = 1;
	instance->angle = 0;
	instance->layer = layer;
	instance->display = true;

	// Collision
	instance->colbox.pos.x = 0;
	instance->colbox.pos.y = 0;
	instance->colbox.pos.z = 0;
	instance->colbox.size.x = 1;
	instance->colbox.size.y = 1;
	instance->colbox.size.z = 1;
	instance->col_priority = 0;

	cr_objects.preset_subtype = 0;
}

void cr_object_fix_active_count(CR_OBJECT* parent)
{
	unsigned int count = 0;
	for (unsigned int id = 0; id < parent->total_count; id++) {
		if (parent->instance[id].active) {
			count++;
		}
	}
	if (parent->active_count == 0) {
		parent->active_count = 1;
	}
	else {
		parent->active_count = count;
	}
}

void cr_object_adjust_count(CR_OBJECT* parent, unsigned char flag)
{
	unsigned int last_avaialble_obj_id = 0;
	bool break_flag = false;

	switch (flag)
	{
	case CR_OBJ_SHRINK:

		if (parent->total_count > parent->buffer_threshold) {
			parent->total_count -= parent->buffer_threshold;
		}
		else {
			parent->total_count = parent->buffer_threshold;
		}

		cr_object_set_count(parent, parent->total_count);

		break;

	case CR_OBJ_GROW:

		parent->total_count += parent->buffer_threshold;
		cr_object_set_count(parent, parent->total_count);

		break;

	case CR_OBJ_DOUBLE:

		parent->total_count *= 2;
		cr_object_set_count(parent, parent->total_count);

		break;

	case CR_OBJ_HALVE:

		parent->total_count /= 2;
		cr_object_set_count(parent, parent->total_count);

		break;

	case CR_OBJ_CONDENSE:

		while (!break_flag) {
			for (unsigned int id = 0; id < parent->total_count; id++) {
				if (!parent->instance[id].active && parent->instance[id].destroyed) {
					last_avaialble_obj_id = id;
				}
			}

			if (parent->total_count - last_avaialble_obj_id >= parent->buffer_threshold) {
				parent->total_count -= parent->buffer_threshold;
				cr_object_set_count(parent, parent->total_count);
			}
			else {
				break_flag = true;
			}
		}

		break;
	}

	parent->align();
}

inline void cr_object_set_count(CR_OBJECT* parent, unsigned int count)
{
	int fuck = 0;
	//CR_INSTANCE *instance = NULL;
	//instance = realloc(parent->instance, parent->total_count * sizeof(CR_INSTANCE));

	CR_INSTANCE* instance = realloc(parent->instance, parent->total_count * sizeof(CR_INSTANCE));

	if (instance != NULL) {
		parent->instance = instance;
	}
	else {
#if CR_DEBUG_OBJECTS
		printf("INSTANCE RESIZING FAILED, EXITING\n");
#endif
		crimson.exitcode = 1;
	}

	void* objects = realloc(*parent->objects, parent->total_count * parent->custom_obj_size);

	if (objects != NULL) {
		*parent->objects = objects;
	}
	else {
#if CR_DEBUG_OBJECTS
		printf("OBJECT RESIZING FAILED, EXITING\n");
#endif
		crimson.exitcode = 1;
	}

	//parent->align();
	//parent->objects = realloc(parent->objects, parent->active_count * parent->custom_obj_size);
}

unsigned long cr_object_get_pointer(CR_OBJECT* parent, unsigned int id)
{
	unsigned long obj_pointer = (unsigned long)(*parent->objects) + (parent->custom_obj_size * id);
	return obj_pointer;
}

unsigned int cr_create_3d(CR_OBJECT* parent, float pos_x, float pos_y, float pos_z, char type, unsigned char layer)
{
	if (!parent->initialized) {
#if CR_DEBUG_OBJECTS
		printf("Crimson Object Debug: This object is not initialized! Object not created, because otherwise there'd be a crash! Load the object!!\n");
#endif
		return NULL;
	}

	bool free_object_exists = false;
	unsigned int free_object_id = 0;
	unsigned int returnval = 0;

	for (unsigned int id = 0; id < parent->total_count; id++) {
		if (parent->instance[id].destroyed) {
			unsigned long obj_pointer = cr_object_get_pointer(parent, id);
			free_object_exists = true;
			free_object_id = id;
			cr_instance_init(parent, &parent->instance[id], id, true, false, type, pos_x, pos_y, pos_z, layer);
			parent->init((void*)(obj_pointer)); //INIT CUSTOM INSTANCE
			parent->instance[id].initialized = true;
			parent->align();
			id = parent->total_count;
		}
	}

	if (!free_object_exists) {

		cr_object_adjust_count(parent, CR_OBJ_GROW);

		for (unsigned int id = (parent->total_count - parent->buffer_threshold); id < (parent->total_count); id++) {
			cr_instance_init(parent, &parent->instance[id], id, false, true, type, pos_x, pos_y, 0, layer);
			parent->instance[id].initialized = false;
			//parent->init(id); //INIT CUSTOM INSTANCE
		}

		cr_instance_init(parent, &parent->instance[parent->total_count - parent->buffer_threshold], parent->total_count - parent->buffer_threshold, true, false, type, pos_x, pos_y, 0, layer);
		unsigned long obj_pointer = cr_object_get_pointer(parent, parent->total_count - parent->buffer_threshold);
		parent->init((void*)(obj_pointer)); // INIT CUSTOM INSTANCE
		parent->instance[parent->total_count - parent->buffer_threshold].initialized = true;
		parent->align();

		returnval = parent->total_count - parent->buffer_threshold;
	}
	else {
		returnval = free_object_id;
	}
	cr_objects.last_spawned = cr_object_get_pointer(parent, returnval);

	cr_object_fix_active_count(parent);

#if CR_DEBUG_OBJECTS
	if (free_object_exists) { printf("Object slot available! ID: %d. ", free_object_id); }
	else { printf("Object slot not available. Created %d more slots... ", parent->buffer_threshold); }
	printf("Total object count: %d. Active object count: %d.\n", parent->total_count, parent->active_count);
#endif // CR_DEBUG_OBJECTS

	return returnval;
}

unsigned int cr_create(CR_OBJECT * parent, float pos_x, float pos_y, char type, unsigned char layer)
{
	return cr_create_3d(parent, pos_x, pos_y, 0, type, layer);
}

void cr_destroy(CR_OBJECT * parent, unsigned int id)
{
	if (id < parent->total_count) {
		parent->instance[id].active = false;
		parent->instance[id].destroyed = true;
		parent->instance[id].initialized = false;
	}

	//cr_object_adjust_count(parent, CR_OBJ_CONDENSE);
}

void cr_destroy_all(CR_OBJECT * parent)
{
	//if (parent->initialized) {
		//cr_object_set_count(parent, parent->buffer_threshold);
		//for (unsigned int id = 0; id < parent->buffer_threshold; id++) {
		for (unsigned int id = 0; id < parent->total_count; id++) {
			parent->instance[id].active = false;
			parent->instance[id].destroyed = true;
		}
		//parent->initialized = false;
	//}
}

void cr_object_free(CR_OBJECT * parent)
{
	if (parent->initialized) {
		if (parent->instance != NULL) {
			free(parent->instance);
		}
		if (parent->objects != NULL) {
			free(*parent->objects);
		}
		parent->total_count = 0;
		parent->active_count = 0;

		parent->initialized = false;
	}
}

//RUN
void cr_objects_run(unsigned char group)
{
	for (unsigned int i = 0; i < cr_objects.obj_count; i++) {
		if (cr_objects.objects[i]->loaded && cr_objects.objects[i]->group == group) {
			cr_objects.objects[i]->run();
		}
	}
}

void cr_layout_set(CR_OBJECT * parent)
{
	//if (parent->loaded) {
		cr_destroy_all(parent);
	//}
	for (unsigned int i = 0; i < layout.count; i++) {
		cr_objects.preset_subtype = layout.array[i].subtype;
		cr_create(parent, layout.array[i].coord_x, layout.array[i].coord_y, layout.array[i].type, layout.array[i].layer);
	}
}

void cr_layout_load(const char* filepath)
{
	if (layout.loaded) {
		free(layout.array);
	}

	unsigned char* buffer;

	FILE* in_file = fopen(filepath, "rb");
	if (!in_file) {
#if CR_DEBUG_OBJECTS
		printf("Couldn't open '%s'!\n", filepath);
#endif
		return;
	}
	else {
#if CR_DEBUG_OBJECTS
		printf("Successfully opened object layout at '%s'\n", filepath);
#endif
	}

	fseek(in_file, 0, SEEK_END);
	size_t file_size = ftell(in_file);
	fseek(in_file, 0, SEEK_SET);

	buffer = malloc(file_size * sizeof(unsigned char));
	if (buffer != NULL) {
		fread(buffer, file_size, 1, in_file);

		//READ FILE
		layout.count = buffer[3] | (buffer[2] << 8) | (buffer[1] << 16) | (buffer[0] << 24);

		layout.array = malloc(layout.count * sizeof(CR_OBJ_LAYOUT));

		unsigned int buffpos = 4;

		for (unsigned int i = 0; i < layout.count; i++) {
			layout.array[i].coord_x =
				buffer[buffpos + ((i * LAYOUT_GROUP_SIZE) + 3)] |
				(buffer[buffpos + ((i * LAYOUT_GROUP_SIZE) + 2)] << 8) |
				(buffer[buffpos + ((i * LAYOUT_GROUP_SIZE) + 1)] << 16) |
				(buffer[buffpos + ((i * LAYOUT_GROUP_SIZE) + 0)] << 24);

			layout.array[i].coord_y =
				buffer[buffpos + ((i * LAYOUT_GROUP_SIZE) + 7)] |
				(buffer[buffpos + ((i * LAYOUT_GROUP_SIZE) + 6)] << 8) |
				(buffer[buffpos + ((i * LAYOUT_GROUP_SIZE) + 5)] << 16) |
				(buffer[buffpos + ((i * LAYOUT_GROUP_SIZE) + 4)] << 24);

			layout.array[i].type = buffer[buffpos + ((i * LAYOUT_GROUP_SIZE) + 8)];
			layout.array[i].subtype = buffer[buffpos + ((i * LAYOUT_GROUP_SIZE) + 9)];

			layout.array[i].layer = buffer[buffpos + ((i * LAYOUT_GROUP_SIZE) + 10)];
		}

		layout.loaded = true;
		fclose(in_file);
		free(buffer);
	}
}

void cr_layout_unload(void)
{
	if (layout.loaded) {
		free(layout.array);
	}
}

void cr_object_run(CR_OBJECT* parent, void(*obj_update)(void* ent), void(*screen_enter)(void* ent), void (*screen_exit)(void* ent), int cam_buffer_x, int cam_buffer_y)
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
								//fr_hitbox_manager_prep((fr_hitbox_manager*)(obj_pointer + hitbox_offset), &parent->instance[i]);

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
							//// If the object has defined a hitbox requesting routine...
							//if (hitbox_request != NULL) {
							//	// If the object's hitbox manager is not algined, run the align function.
							//	if (!(*(bool*)(obj_pointer + hitbox_offset + offsetof(fr_hitbox_manager, aligned)))) {
							//		fr_hitbox_manager_align((fr_hitbox_manager*)(obj_pointer + hitbox_offset), &parent->instance[i]);
							//	}
							//	// If the object's manager is not already initialized, call the request hitbox function.
							//	if (hitbox_request != NULL && !(*(bool*)(obj_pointer + hitbox_offset + offsetof(fr_hitbox_manager, initialized)))) {
							//		hitbox_request((void*)(obj_pointer));
							//	}
							//}
							obj_update((void*)(obj_pointer)); // Update the current object instance
						}
					}
					else { // If the object is not running, free up the hitboxes and prepare the manager for next time.
						//fr_hitbox_manager_prep((fr_hitbox_manager*)(obj_pointer + hitbox_offset), &parent->instance[i]);
					}
				}
				else { // If the object is not running, free up the hitboxes and prepare the manager for next time.
					//fr_hitbox_manager_prep((fr_hitbox_manager*)(obj_pointer + hitbox_offset), &parent->instance[i]);
				}
			}
		}
	}
}

// 2 Floats
void cr_spdtopos(float* pos_x, float *pos_y, float *spd_x, float *spd_y)
{
	*pos_x += *spd_x;
	*pos_y += *spd_y;
}

void cr_spdtopos_grav(float* pos_x, float* pos_y, float *spd_x, float *spd_y, float grav)
{
	*pos_x += *spd_x;
	*pos_y += *spd_y;
	*spd_y += grav;
}

// cec2f
void cr_spdtopos_v2(vec2f* pos, vec2f* spd)
{
	pos->x += spd->x;
	pos->y += spd->y;
}

void cr_spdtopos_grav_v2(vec2f* pos, vec2f* spd, float grav)
{
	pos->x += spd->x;
	pos->y += spd->y;
	spd->y += grav;
}

// vec3f
void cr_spdtopos_v3(vec3f* pos, vec3f* spd)
{
	pos->x += spd->x;
	pos->y += spd->y;
	pos->z += spd->z;
}

void cr_spdtopos_grav_v3(vec3f* pos, vec3f* spd, float grav)
{
	pos->x += spd->x;
	pos->y += spd->y;
	pos->z += spd->z;
	spd->y += grav;
}

// Object
void cr_spdtopos_obj(CR_INSTANCE* inst)
{
	inst->pos.x += inst->spd.x;
	inst->pos.y += inst->spd.y;
	inst->pos.z += inst->spd.z;
}

void cr_spdtopos_grav_obj(CR_INSTANCE* inst, float grav)
{
	inst->pos.x += inst->spd.x;
	inst->pos.y += inst->spd.y;
	inst->pos.z += inst->spd.z;
	inst->spd.y += grav;
}

void cr_radial_spawn(CR_OBJECT* parent, float pos_x, float pos_y, float starting_deg, short deg_increment, unsigned char revolutions, float root_dist, float spacing_inc, float speed_x, float speed_y, float speed_inc, char type, unsigned char layer)
{
	unsigned int id = 0;
	bool do_pos = true;

	unsigned short total_inc = 0;

	starting_deg += 180;

	if (deg_increment > 0) {

	}
	else {
		do_pos = false;
		if (deg_increment == 0) {
			return;
		}
	}


	for (unsigned short r = 0; r < revolutions; r++) {
		if (do_pos) {
			for (short d = starting_deg; d < 360 + starting_deg - deg_increment; d += deg_increment) {
				id = cr_create(parent, pos_x + ((root_dist + (spacing_inc * total_inc)) * cr_sinf(d)), pos_y + ((root_dist + (spacing_inc * total_inc)) * cr_cosf(d)), type, layer);
				parent->instance[id].spd.x = (speed_x * cr_sinf(d)) + ((speed_inc * total_inc) * cr_sinf(d));
				parent->instance[id].spd.y = (speed_y * cr_cosf(d)) + ((speed_inc * total_inc) * cr_cosf(d));

				total_inc++;
			}
		}
	}
}

void* cr_fetchobject_lastspawn(void)
{

}
