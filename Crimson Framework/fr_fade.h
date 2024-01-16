#ifndef FR_FADE_H
#define FR_FADE_H

typedef enum {
	FR_FADE_BLACK,
	FR_FADE_WHITE,
} FR_FADE_TYPES;

typedef enum {
	FR_FADE_OUT,
	FR_FADE_IN,
} FR_FADE_IN_OUT;

typedef enum {
	FR_FADE_MODE_ALL,
	FR_FADE_MODE_DELEGATE,
} FR_FADE_MODES;

typedef struct {

	CR_INSTANCE* inst;

	CR_ANIM anim;

	short fade;
	unsigned short rate;
	bool fade_in;

} child_fr_fade;

child_fr_fade* fr_fades;

CR_OBJECT fr_fade;

//Per-Object functions
unsigned int fr_fade_create(unsigned char fade_type, bool fade_in, unsigned short fade_rate);

void fr_fade_update(child_fr_fade* ent);
void fr_fade_init(child_fr_fade* ent);

//One time functions
void fr_fade_run(void);
void fr_fade_align(void);
void fr_fade_load(const char* const filepath);
void fr_fade_unload(void);

#endif // !FR_FADE_H
