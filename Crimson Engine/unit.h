#ifndef __UNIT_H__
#define __UNIT_H__

//WIP TEMPLATE

//Data
bool unit_loaded;

//Struct
typedef struct {

	vec2f pos;

} unit_struct;

unit_struct item;

//Functions
void unit_update(void);
void unit_load(void);
void unit_unload(void);

#endif //!__UNIT_H__