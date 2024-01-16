#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include "crimson.h"

typedef struct {
	int fuck;
} CR_FRAMEWORK_STRUCT;

CR_FRAMEWORK_STRUCT framework;

#include "fr_hitbox.h"

#include "fr_stages.h"
#include "fr_objects.h"
#include "fr_players.h"

void fr_init(void);

void fr_update_start(void);

void fr_update_end(void);

void fr_close(void);

#endif // !FRAMEWORK_H
