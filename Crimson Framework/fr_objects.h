#ifndef FR_OBJECTS_H
#define FR_OBJECTS_H

typedef enum {
	GROUP_GLOBAL,
	GROUP_GENERIC,
	GROUP_PLAYERS,
	GROUP_ENEMIES,
	GROUP_HAZARDS,
	GROUP_HUD,
	GROUP_ITEMS,
	GROUP_EARLYRUN,
} FR_OBJECT_GROUPS;

#include "fr_components.h"

#include "vhs.h"
#include "helios.h"
#include "particle.h"

#endif // !FR_OBJECTS_H
