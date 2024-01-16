#ifndef CRIMSON_H
#define CRIMSON_H

// ================================
// DEFINITIONS
// ================================

#define CR_DIET		(0)
#define CR_WINDOWS	(1)
#define CR_LINUX	(2)
#define CR_ANDROID	(3)

#define CR_PLATFORM	(CR_WINDOWS)

// ================================
// PLATFORMS
// ================================

#if CR_PLATFORM == CR_DIET
#include "cr_diet.h"
#endif

#if CR_PLATFORM == CR_WINDOWS
#include "cr_windows.h"
#endif

#if CR_PLATFORM == CR_LINUX
#include "cr_linux.h"
#endif

#if CR_PLATFORM == CR_ANDROID
#include "cr_android.h"
#endif

// ================================
// CORE CRIMSON STRUCT
// ================================

typedef struct {

	void(*fr_init)();
	void(*fr_update_start)();
	void(*fr_update_end)();
	void(*fr_close)();

	char name[0x20];
	char version[0x20];

	bool skip_update;
	bool wipe_background;
	unsigned int clear_color;

	bool pause_anims;
	bool master_pause;

	bool gamepaused;
	bool pausegroups[8];

	unsigned char default_spawn_layer;

	unsigned long framecount;
	unsigned long rngseed;
	int exitcode;

	long draw_calls;

	unsigned short width;
	unsigned short height;
	unsigned short game_width;
	unsigned short game_height;

	unsigned short hd_width;
	unsigned short hd_height;
	unsigned short game_hd_width;
	unsigned short game_hd_height;

	float target_fps;

	float scale;
	unsigned char screenmode;

	void *entry_stage;
	bool entry_set;

	bool fr_initialized;
	bool cr_initialized;
	bool running;

	bool gameconfig_loaded;

} CRIMSON_STRUCT;

CRIMSON_STRUCT crimson;

// ================================
// CORE INCLUDES
// ================================

#include "cr_math.h"
#include "cr_memory.h"
#include "cr_input.h"
#include "cr_sprites.h"
#include "cr_camera.h"
#include "cr_stage.h"
#include "cr_collision.h"
#include "cr_objects.h"
#include "cr_sprites.h"
#include "cr_drawing.h"
#include "cr_audio.h"
#include "cr_font.h"
#include "cr_debug.h"

void crimson_run(void);

void crimson_init(unsigned short width, unsigned short height, unsigned short game_width, unsigned short game_height, unsigned short hd_width, unsigned short hd_height,unsigned short game_hd_width, unsigned short game_hd_height);
void crimson_close(void);

void cr_framework_init(const char *game_name, const char *version, void(*fr_init)(void), void(*fr_update_start)(void), void(*fr_update_end)(void), void(*fr_close)(void), float target_fps);
void cr_framework_close(void);

void cr_gameconfig_load(void);
void cr_gameconfig_save(void);

void cr_clear_background(void);

#endif // !CRIMSON_H
