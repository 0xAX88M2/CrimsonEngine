#ifndef CR_OBJECTS_H
#define CR_OBJECTS_H

typedef enum {
	CR_OBJ_RESTORE,
	CR_OBJ_SHRINK,
	CR_OBJ_GROW,
	CR_OBJ_DOUBLE,
	CR_OBJ_HALVE,
	CR_OBJ_CONDENSE,
} CR_OBJ_RESIZE_FLAGS;

typedef struct {

	// Core properties
	unsigned int	id;
	unsigned int	parent_id;				// id within the object manager
	unsigned char	type;
	char			subtype;
	unsigned char	mode;
	bool			onscreen;
	bool			active;
	bool			destroyed;
	bool			initialized;

	// Display and position
	vec3f			pos;
	vec3f			spd;
	vec3b			flip;
	vec3f			scale;
	vec3us			size;
	float			angle;
	unsigned char	layer;
	bool			display;

	// Collision
	CR_BOX_3D		colbox;
	unsigned char	col_priority;

} CR_INSTANCE;

typedef struct {

	void(*load)(const char* const filepath);
	void(*unload)(void);
	void(*init)(void* ent);
	void(*align)(void);
	void(*run)(void);

	unsigned char	buffer_threshold;

	unsigned char	group;
	bool			global;
	bool			global_added;

	CR_INSTANCE* instance;
	void** objects;

	size_t			custom_obj_size;

	unsigned int	total_count;
	unsigned int	active_count;
	unsigned int	obj_id;				// id within the object manager

	bool			initialized;
	bool			loaded;

} CR_OBJECT;

typedef struct {

	CR_OBJECT** objects;
	unsigned int obj_count;
	bool obj_initialized;

	CR_OBJECT** globals;
	unsigned int glob_count;
	bool glob_initialized;

	unsigned char preset_subtype;

	//unsigned long last_spawned;
	void* last_spawned;

} CR_OBJ_MANAGER;

CR_OBJ_MANAGER cr_objects;

typedef struct {
	int coord_x;
	int coord_y;
	char type;
	unsigned char subtype;
	unsigned char layer;
} CR_OBJ_LAYOUT;

typedef struct {
	CR_OBJ_LAYOUT* array;
	unsigned int count;
	bool loaded;
} CR_LAYOUT_MANAGER;

CR_LAYOUT_MANAGER layout;

//FUNCTIONS

//OBJECTS
void cr_object_manager_reset(void);
void cr_object_manager_close(void);
void cr_object_reload_globals(void);
void cr_object_manager_add(CR_OBJECT* parent_pointer);
void cr_objects_run(unsigned char group);
void cr_object_init(const char* filepath, CR_OBJECT* parent, void** custom_objects, void(*load)(const char* const filepath), void(*unload)(void), void(*init)(void* ent), void(*align)(void), void(*run)(void), size_t custom_obj_size, unsigned short buffer_threshold);
void cr_instance_init(CR_OBJECT* parent, CR_INSTANCE* instance, unsigned int obj_id, bool active, bool destroyed, char type, float pos_x, float pos_y, float pos_z, unsigned char layer);
void cr_object_fix_active_count(CR_OBJECT* parent);
void cr_object_adjust_count(CR_OBJECT* parent, unsigned char flag);
inline void cr_object_set_count(CR_OBJECT* parent, unsigned int count);
unsigned long cr_object_get_pointer(CR_OBJECT* parent, unsigned int id);

//GENERAL
unsigned int cr_create_3d(CR_OBJECT* parent, float pos_x, float pos_y, float pos_z, char type, unsigned char layer);
unsigned int cr_create(CR_OBJECT* parent, float pos_x, float pos_y, char type, unsigned char layer);
void cr_destroy(CR_OBJECT* parent, unsigned int id);
void cr_destroy_all(CR_OBJECT* parent);
void cr_object_free(CR_OBJECT* parent);

//RUN
void cr_objects_run(unsigned char group);

//LAYOUT
void cr_layout_set(CR_OBJECT* parent);
void cr_layout_load(const char* filepath);
void cr_layout_unload(void);

// HELPERS
void cr_object_run(CR_OBJECT* parent, void(*obj_update)(void* ent), void(*screen_enter)(void* ent), void (*screen_exit)(void* ent), int cam_buffer_x, int cam_buffer_y);

void cr_spdtopos_v2(vec2f* pos, vec2f* spd);
void cr_spdtopos_grav_v2(vec2f* pos, vec2f* spd, float grav);

void cr_spdtopos_v3(vec3f* pos, vec3f* spd);
void cr_spdtopos_grav_v3(vec3f* pos, vec3f* spd, float grav);

void cr_spdtopos_obj(CR_INSTANCE* inst);
void cr_spdtopos_grav_obj(CR_INSTANCE* inst, float grav);

// EXTRAS
void cr_radial_spawn(CR_OBJECT* parent, float pos_x, float pos_y, float starting_deg, short deg_increment, unsigned char revolutions, float root_dist, float spacing_inc, float speed_x, float speed_y, float speed_inc, char type, unsigned char layer);

#endif // !__CR_OBJECTS_H__
