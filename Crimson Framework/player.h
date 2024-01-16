#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_COLPNT_COUNT			(10)
#define PLAYER_ORIENTATION_COUNT	(9)

typedef enum {
	player_physics_normal,
	player_physics_underwater,
} player_physics_modes;

typedef enum {
	PROUTINE_NONE,
	PROUTINE_CONTROL,
	PROUTINE_HURT,
	PROUTINE_DEAD,
	PROUTINE_RESTART,
} PLAYER_ROUTINES;

typedef enum {
	PMODE_NONE,
	PMODE_GROUND,
	PMODE_AIR,
	PMODE_ACTION,
} PLAYER_MODES;

typedef enum {
	PACTION_NONE,
} PLAYER_ACTIONS;

// Custom Object Variables:
typedef struct {

	CR_INSTANCE *inst;			// Pointer to etity's instance.
	CR_ANIM anim;				// Animation struct.
	fr_hitbox_manager hitboxes;	// Hitbox manager.

	// Collision
	CR_COLLISION_POINT colpnts[PLAYER_COLPNT_COUNT];

	unsigned char offset_orientation;	// Orientation mode for sensor offsets
	unsigned char collision_mode;		// Correlates to the method used to get collision
	unsigned char angle_mode;			// Correlates to the method of angle detection 

	// Statuses
	unsigned char routine;				// Broad routine for player as a whole
	unsigned char mode;					// Mode within routine
	unsigned char action;				// Action within mode (determines what happen mode == PMODE_ACTION)
	unsigned char jump_flag;
	float gspeed;						// "Ground Speed", used for moving at angles
	char angle;							// Angle of movement (256 base)
	float angle_360;					// Above angle but in base 360
	float display_angle;				// Angle to display at
	bool jumping;
	bool grounded;						// Is the player on the ground?
	bool landed;						// True only on the frame the player landed

	// Physics
	unsigned char physics_mode;			// Player's current physics mode

	float ground_accel;					// Player's ground acceleration
	float ground_decel;					// Player's ground deceleration when moving the opposite way
	float ground_friction;				// Player's ground deceleration when not holding buttons
	float ground_top;					// Player's natural top speed on the ground

	float air_accel;					// Player's air acceleration
	float air_decel;					// Player's air deceleration when moving the opposite way
	float air_friction;					// Player's air deceleration when not holding buttons
	float air_top;						// Player's natural top speed in the air

	float grav;							// Gravity

	float jump_factor;					// Factor of multiplication for jump math

	// Data
	CR_INPUT_STRUCT* input;
	CR_INPUT_STRUCT dummyinput;

} child_player;

child_player *players;

CR_OBJECT player;

// Per-Object functions
void player_update(child_player *ent);
void player_init(child_player *ent);

// One time functions
void player_run(void);
void player_align(void);

// Loading functions
void player_load(const char *const filepath);
void player_unload(void);

#endif // !player_H
