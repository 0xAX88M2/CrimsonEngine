#ifndef PARTICLE_H
#define PARTICLE_H

typedef enum {
	particle_heartstar,
	particle_flashorb,
	particle_lovelove,
	particle_loveexp1,
	particle_loveexp2,
	particle_lovetrail,
} PARTICLE_TYPES;

// Custom Object Variables:
typedef struct {

	CR_INSTANCE *inst;	// Pointer to etity's instance.
	CR_ANIM anim;		// Animation struct.

	unsigned char frame;

	int timer;
	int flash;
	bool show;

} child_particle;

child_particle *particles;

CR_OBJECT particle;

unsigned int special_id;

// Per-Object functions
void particle_update(child_particle *ent);
void particle_init(child_particle* ent);

// One time functions
void particle_run(void);
void particle_align(void);

// Loading functions
void particle_load(const char *const filepath);
void particle_unload(void);

#endif // !particle_H
