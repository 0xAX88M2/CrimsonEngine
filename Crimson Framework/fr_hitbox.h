#ifndef FR_HBOX_H
#define FR_HBOX_H

#define FR_HITBOX_COUNT (8)
#define FR_HITBOX_COL_COUNT	(8)

// Note from your good pal TSB: These hitbox types should fit any and all objects you need.
// If they don't, it's what the HITBOX_SPECIAL type is for! It still returns useful information from the collision manager
// and allows you to program whatever interaction you want from the object's code. How neat is that?
typedef enum {
	HITBOX_PLAYER,							// Player
	HITBOX_ENEMY,							// Badniks
	HITBOX_BOSS,							// Bosses
	HITBOX_MONITOR,							// Monitors
	HITBOX_SPECIAL,							// Special edge cases (EX: Rings)
} FR_HITBOX_TYPE;

typedef enum {
	HTAG_NONE = 0x0,						// No tag placeholder
	HTAG_HURT = 0x1,						// Hurting property (Used to allow the object to hurt the player)
	HTAG_SOLID = 0x2,						// Solidity property (Used to allow the object to be solid)
	HTAG_BUSTABLE = 0x4,					// Bustable property (Used to allow the object to be busted open)
	HTAG_PUSHABLE = 0x8,					// Pushable property (Used to allow the object to be pushed and moved)
	HTAG_ATTACK = 0x10,						// Attacking property (Used to allow the object to destory bustable objects among other things)
	HTAG_CLIMB = 0x20,
	HTAG_HAZARD = 0x40,
	HTAG_SPECIAL = 0x80,
} FR_HITBOX_TAGS;

struct fr_hitbox_master_struct;

typedef enum {
	PROX_NONE = 0x0,						// No collision (just a safety value juuuust in case)
	PROX_TOP = 0x1,							// Touching the top of a hitbox
	PROX_BOTTOM = 0x2,						// Touching the bottom of a hitbox
	PROX_LEFT = 0x4,						// Touching the left side of a hitbox
	PROX_RIGHT = 0x8						// Touching the right side of a hitbox
} coll_prox;

// HITBOX STRUCT

typedef struct fr_hitbox {

	unsigned int id;				// ID within Master List.

	vec2s offset;					// Offset fromt the object the hitbox is attached to.
	vec2us size;					// Size in pixels, width/height.
	vec2b flip;						// Whether or not the hitbox is flipped on the X or Y axis, relative to the attached object.

	short type;						// Type of hitbox
	short tags;						// Tags for comparison

	bool used;						// If the hitbox is being used by a manager.
	bool active;					// If the hitbox is active within its manager.

	unsigned char col_count;		// Number of collisions 
	unsigned int colliding_ids[8];	// IDs of the hitboxes this hitbox is colliding with

	struct fr_hitbox_manager* parent_manager;	// Pointer to the parent manager this hitbox belongs to

	// Custom Variables
	float angle;
	char prox;
	bool stood;

	char hitlag;
	float power;
	float damage;

} fr_hitbox;


// HITBOX MANAGER STRUCT 

typedef struct fr_hitbox_manager {

	unsigned int box_count;						// Number of hitboxes on this manager.
	//fr_hitbox* boxes[FR_HITBOX_COUNT];		// List of pointers to attached hitboxes.
	unsigned int box_ids[FR_HITBOX_COUNT];		// IDs to each hitbox.
	unsigned int parent_id;						// ID of the parent object.
	unsigned int instance_id;					// ID of the instance within the parent object.

	bool initialized;							// Is initialized with hitboxes, meaning it can be actively used.
	bool aligned;								// 

} fr_hitbox_manager;

// MASTER HITBOX_LIST 

typedef struct {

	unsigned int count;			// Number of hitboxes total.
	fr_hitbox* hitboxes;		// Pointer list to all hitboxes.
	bool initialized;			// duh
	fr_hitbox_manager null_manager;

} fr_hitbox_master_struct;

fr_hitbox_master_struct fr_hitboxes;

// MASTER
void fr_hitbox_master_init(unsigned short total_count);
void fr_hitbox_master_close(void);
void fr_hitbox_master_reset(void);

// HITBOXES
void fr_hitbox_reset(fr_hitbox* hitbox);
void fr_hitbox_activate(unsigned int id, short offset_x, short offset_y, unsigned short width, unsigned short height, unsigned short type, unsigned short tags);
void fr_hitbox_deactivate(fr_hitbox* hitbox);
bool fr_hitbox_request(fr_hitbox_manager* manager, unsigned char count);

// MANAGERS
void fr_hitbox_manager_prep(fr_hitbox_manager* manager, CR_INSTANCE* inst);
void fr_hitbox_manager_align(fr_hitbox_manager* manager, CR_INSTANCE* inst);
void fr_hitbox_manager_clear(fr_hitbox_manager* manager);

// COLLISION
inline bool fr_hitbox_check(fr_hitbox* hbox1, fr_hitbox* hbox2);
void fr_hitbox_update_collisions(void);

bool fr_hitbox_has_tags(fr_hitbox* box, unsigned short tags);

bool fr_hitbox_check_self_type(fr_hitbox_manager* manager, unsigned short self_type);
bool fr_hitbox_check_self_tags(fr_hitbox_manager* manager, unsigned short self_tags);

bool fr_hitbox_check_target_type(fr_hitbox_manager* manager, unsigned short target_type);
bool fr_hitbox_check_target_tags(fr_hitbox_manager* manager, unsigned short target_tags);

fr_hitbox* fr_hitbox_get_self_info(fr_hitbox_manager* manager);
fr_hitbox* fr_hitbox_get_target_info(fr_hitbox_manager* manager);

fr_hitbox* fr_hitbox_get_self_info_type(fr_hitbox_manager* manager, unsigned short self_type);
fr_hitbox* fr_hitbox_get_target_info_type(fr_hitbox_manager* manager, unsigned short target_type);

fr_hitbox* fr_hitbox_get_self_info_tags(fr_hitbox_manager* manager, unsigned short self_tags);
fr_hitbox* fr_hitbox_get_target_info_tags(fr_hitbox_manager* manager, unsigned short target_tags);

bool fr_hitbox_check_both_tags(fr_hitbox_manager* manager, unsigned short self_tags, unsigned short target_tags);
bool fr_hitbox_check_both_type(fr_hitbox_manager* manager, unsigned short self_type, unsigned short target_type);

fr_hitbox* fr_hitbox_get_both_tags_info(fr_hitbox_manager* manager, unsigned short self_tags, unsigned short target_tags);
fr_hitbox* fr_hitbox_get_both_type_info(fr_hitbox_manager* manager, unsigned short self_type, unsigned short target_type);

fr_hitbox* fr_hitbox_get(unsigned short hitbox_id);

void fr_hitbox_proximity_update(fr_hitbox* hbox1, fr_hitbox* hbox2);
bool fr_hitbox_proximity_check(unsigned char prox, fr_hitbox* hbox1, fr_hitbox* hbox2);
void fr_hitbox_adjust(fr_hitbox_manager* manager, unsigned char hitbox_id, short offset_x, short offset_y, unsigned short width, unsigned short height, unsigned short type, unsigned short tags);
void fr_hitbox_move(fr_hitbox_manager* manager, unsigned char hitbox_id, short offset_x, short offset_y, unsigned short width, unsigned short height);

void fr_hitbox_manager_flip(fr_hitbox_manager* manager, bool flip_x, bool flip_y);

void fr_hitbox_get_pos(fr_hitbox* hitbox, float* pos_x, float* pos_y);
void fr_hitbox_get_obj_pos(fr_hitbox* hitbox, float* pos_x, float* pos_y);
void fr_hitbox_get_obj_size(fr_hitbox* hitbox, unsigned int* width, unsigned int* height);

void fr_hitbox_id_get_pos(unsigned int hitbox_id, float* pos_x, float* pos_y);
void fr_hitbox_id_get_obj_pos(unsigned int hitbox_id, float* pos_x, float* pos_y);
void fr_hitbox_id_get_obj_size(unsigned int hitbox_id, unsigned int* width, unsigned int* height);

void fr_hitbox_object_run(CR_OBJECT* parent, void(*obj_update)(void* ent), void(*screen_enter)(void* ent), void (*screen_exit)(void* ent), void(*hitbox_request)(void* ent), unsigned short hitbox_offset, int cam_buffer_x, int cam_buffer_y);

// DEBUG
void fr_hitbox_debug_draw(void);

// COLLISION

#endif // !FR_HBOX_H
