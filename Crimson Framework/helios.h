#ifndef HELIOS_H
#define HELIOS_H

// Custom Object Variables:
typedef struct {

	CR_INSTANCE *inst;	// Pointer to etity's instance.
	CR_ANIM anim;		// Animation struct.

	float alpha;
	float radius;

	vec2f degree;

} child_helios;

child_helios *helioss;

CR_OBJECT helios;

// Per-Object functions
void helios_update(child_helios *ent);
void helios_init(child_helios* ent);

// One time functions
void helios_run(void);
void helios_align(void);

// Loading functions
void helios_load(const char *const filepath);
void helios_unload(void);

#endif // !helios_H
