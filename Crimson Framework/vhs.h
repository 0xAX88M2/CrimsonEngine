#ifndef VHS_H
#define VHS_H

// Custom Object Variables:
typedef struct {

	CR_INSTANCE *inst;	// Pointer to etity's instance.
	CR_ANIM an;
	CR_ANIM play_anim;
	vec2f play_pos;
	vec2f play_spd;

	CR_ANIM counter_anim;
	vec2f counter_pos;
	vec2f counter_spd;

	float time_ms;
	short time_sec;
	short time_min;
	long time_hour;

} child_vhs;

child_vhs *vhss;

CR_OBJECT vhs;

// Per-Object functions
void vhs_update(child_vhs *ent);
void vhs_init(child_vhs* ent);

// One time functions
void vhs_run(void);
void vhs_align(void);

// Loading functions
void vhs_load(const char *const filepath);
void vhs_unload(void);

#endif // !vhs_H
