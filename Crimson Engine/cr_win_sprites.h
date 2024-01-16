#ifndef CR_WIN_SPRITES_H
#define CR_WIN_SPRITES_H

//LOADING FUNCTIONS
CR_TEXTURE* cr_windows_sprite_load(const char *filepath, int transparent_color);
void cr_windows_sprite_unload(CR_TEXTURE *sprite_id);

#endif // !CR_WIN_SPRITES_H
