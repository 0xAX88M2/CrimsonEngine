#ifndef CR_WIN_INPUT_H
#define CR_WIN_INPUT_H

typedef enum {
	CR_WINDOWS_INPUT_KEYS,
	CR_WINDOWS_INPUT_CONTROLLER,
} CR_WINDOWS_INPUT_TYPE;

void cr_windows_input_update(CR_INPUT_STRUCT *input);

void cr_windows_controller_init(unsigned char cont_id);
void cr_windows_input_map_button(int *button_to_map, int mapping_button);
void cr_windows_input_map_default_buttons(CR_INPUT_STRUCT *input);
void cr_windows_input_map_key(int *key_to_map, SDL_Keycode mapping_key);
void cr_windows_input_map_default_keys(CR_INPUT_STRUCT *input, unsigned char count);

void cr_windows_input_adjust_mapping(void);
void cr_windows_input_adjust_keymap(int *map);
void cr_windows_input_adjust_contmap(int *map);

void cr_windows_input_restore_mapping(void);
void cr_windows_input_restore_keymap(int *map);
void cr_windows_input_restore_contmap(int *map);

#endif // !CR_INPUT_WINDOWS_H
