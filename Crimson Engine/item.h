#ifndef __ITEM_H__
#define __ITEM_H__

//WIP TEMPLATE

//Struct

typedef struct {

	vec2f pos;

} item_struct;

item_struct *item;

//Functions
//Per-Object functions
void item_update(unsigned int id);
void item_init(unsigned int id);

//One time functions
void item_run(void);
void item_set(void);
void item_load(const char *const filepath);
void item_unload(void);

#endif //!__ITEM_H__