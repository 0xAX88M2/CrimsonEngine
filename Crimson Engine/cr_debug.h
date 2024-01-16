#ifndef CR_DEBUG_H
#define CR_DEBUG_H

#define CR_DEBUG			(1)

#if CR_DEBUG
#define CR_DEBUG_STAGE		(1)
#define CR_DEBUG_INPUT		(1)
#define CR_DEBUG_SPRITES	(1)
#define CR_DEBUG_OBJECTS	(0)
#define CR_DEBUG_COLLISION	(1)
#define CR_DEBUG_AUDIO		(1)
#else
#define CR_DEBUG_STAGE		(0)
#define CR_DEBUG_INPUT		(0)
#define CR_DEBUG_SPRITES	(0)
#define CR_DEBUG_OBJECTS	(0)
#define CR_DEBUG_COLLISION	(0)
#define CR_DEBUG_AUDIO		(0)
#endif

typedef enum {
	cr_debug_sensorpoint,
} cr_debug_gfx;

typedef struct {

	CR_ANIM_DATA data;
	CR_ANIM white;
	CR_ANIM red;
	CR_ANIM white_nums;
	CR_ANIM red_nums;
	bool active;
	short selection;
	unsigned char mode;

	unsigned char mainmenu_item_count;

} CR_DEVMENU_STRUCT;

CR_DEVMENU_STRUCT cr_devmenu;

typedef struct {

	bool active;
	bool enabled;
	bool step;
	bool fastforward;
	bool draw_screen;
	short fastforward_speed;

	bool game_debug;
	bool debug_view;

	CR_DEVMENU_STRUCT devmenu;

	bool allow_devmenu;

	CR_TEXTURE* sprites[4];

} CR_DEBUG_STRUCT;

CR_DEBUG_STRUCT cr_debug;

void cr_devmenu_load(void);
void cr_devmenu_unload(void);
void cr_devmenu_run(void);

#endif // !CR_DEBUG_H
