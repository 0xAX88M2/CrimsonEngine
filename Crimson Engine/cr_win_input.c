#include "crimson.h"
#include "cr_win_input.h"

void cr_windows_input_update(CR_INPUT_STRUCT *input)
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	if (input->skip_update) {
		input->skip_update = false;
		return;
	}

	switch (input->input_type) {

	case CR_WINDOWS_INPUT_KEYS:

		if (keystate[input->A_KEYMAP]) {
			if (!input->locked) { cr_input_set_held(&input->A_HOLD, &input->A_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->A_HOLD, &input->A_RELEASE, &input->A_PRESS); }
		}

		if (keystate[input->B_KEYMAP]) {
			if (!input->locked) { cr_input_set_held(&input->B_HOLD, &input->B_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->B_HOLD, &input->B_RELEASE, &input->B_PRESS); }
		}

		if (keystate[input->X_KEYMAP]) {
			if (!input->locked) { cr_input_set_held(&input->X_HOLD, &input->X_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->X_HOLD, &input->X_RELEASE, &input->X_PRESS); }
		}

		if (keystate[input->Y_KEYMAP]) {
			if (!input->locked) { cr_input_set_held(&input->Y_HOLD, &input->Y_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->Y_HOLD, &input->Y_RELEASE, &input->Y_PRESS); }
		}

		if (keystate[input->L_KEYMAP]) {
			if (!input->locked) { cr_input_set_held(&input->L_HOLD, &input->L_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->L_HOLD, &input->L_RELEASE, &input->L_PRESS); }
		}

		if (keystate[input->R_KEYMAP]) {
			if (!input->locked) { cr_input_set_held(&input->R_HOLD, &input->R_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->R_HOLD, &input->R_RELEASE, &input->R_PRESS); }
		}

		if (keystate[input->UP_KEYMAP]) {
			if (!input->locked) { cr_input_set_held(&input->UP_HOLD, &input->UP_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->UP_HOLD, &input->UP_RELEASE, &input->UP_PRESS); }
		}

		if (keystate[input->DOWN_KEYMAP]) {
			if (!input->locked) { cr_input_set_held(&input->DOWN_HOLD, &input->DOWN_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->DOWN_HOLD, &input->DOWN_RELEASE, &input->DOWN_PRESS); }
		}

		if (keystate[input->LEFT_KEYMAP]) {
			if (!input->locked) { cr_input_set_held(&input->LEFT_HOLD, &input->LEFT_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->LEFT_HOLD, &input->LEFT_RELEASE, &input->LEFT_PRESS); }
		}

		if (keystate[input->RIGHT_KEYMAP]) {
			if (!input->locked) { cr_input_set_held(&input->RIGHT_HOLD, &input->RIGHT_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->RIGHT_HOLD, &input->RIGHT_RELEASE, &input->RIGHT_PRESS); }
		}

		if (keystate[input->START_KEYMAP]) {
			if (!input->locked) { cr_input_set_held(&input->START_HOLD, &input->START_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->START_HOLD, &input->START_RELEASE, &input->START_PRESS); }
		}

		if (keystate[input->SELECT_KEYMAP]) {
			if (!input->locked) { cr_input_set_held(&input->SELECT_HOLD, &input->SELECT_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->SELECT_HOLD, &input->SELECT_RELEASE, &input->SELECT_PRESS); }
		}

		break;

	case CR_WINDOWS_INPUT_CONTROLLER:

		if (SDL_GameControllerGetButton(input->controller, input->A_CONTMAP)) {
			if (!input->locked) { cr_input_set_held(&input->A_HOLD, &input->A_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->A_HOLD, &input->A_RELEASE, &input->A_PRESS); }
		}

		if (SDL_GameControllerGetButton(input->controller, input->B_CONTMAP)) {
			if (!input->locked) { cr_input_set_held(&input->B_HOLD, &input->B_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->B_HOLD, &input->B_RELEASE, &input->B_PRESS); }
		}

		if (SDL_GameControllerGetButton(input->controller, input->X_CONTMAP)) {
			if (!input->locked) { cr_input_set_held(&input->X_HOLD, &input->X_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->X_HOLD, &input->X_RELEASE, &input->X_PRESS); }
		}

		if (SDL_GameControllerGetButton(input->controller, input->Y_CONTMAP)) {
			if (!input->locked) { cr_input_set_held(&input->Y_HOLD, &input->Y_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->Y_HOLD, &input->Y_RELEASE, &input->Y_PRESS); }
		}

		if (SDL_GameControllerGetButton(input->controller, input->L_CONTMAP)) {
			if (!input->locked) { cr_input_set_held(&input->L_HOLD, &input->L_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->L_HOLD, &input->L_RELEASE, &input->L_PRESS); }
		}

		if (SDL_GameControllerGetButton(input->controller, input->R_CONTMAP)) {
			if (!input->locked) { cr_input_set_held(&input->R_HOLD, &input->R_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->R_HOLD, &input->R_RELEASE, &input->R_PRESS); }
		}

		if (SDL_GameControllerGetButton(input->controller, input->UP_CONTMAP)) {
			if (!input->locked) { cr_input_set_held(&input->UP_HOLD, &input->UP_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (SDL_GameControllerGetAxis(input->controller, SDL_CONTROLLER_AXIS_LEFTY) < -20000) {
				if (!input->locked) { cr_input_set_held(&input->UP_HOLD, &input->UP_PRESS); }
				if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
			}
			else {
				if (!input->locked) { cr_input_set_released(&input->UP_HOLD, &input->UP_RELEASE, &input->UP_PRESS); }
			}
		}

		if (SDL_GameControllerGetButton(input->controller, input->DOWN_CONTMAP)) {
			if (!input->locked) { cr_input_set_held(&input->DOWN_HOLD, &input->DOWN_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (SDL_GameControllerGetAxis(input->controller, SDL_CONTROLLER_AXIS_LEFTY) > 20000) {
				if (!input->locked) { cr_input_set_held(&input->DOWN_HOLD, &input->DOWN_PRESS); }
				if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
			}
			else {
				if (!input->locked) { cr_input_set_released(&input->DOWN_HOLD, &input->DOWN_RELEASE, &input->DOWN_PRESS); }
			}
		}

		if (SDL_GameControllerGetButton(input->controller, input->LEFT_CONTMAP)) {
			if (!input->locked) { cr_input_set_held(&input->LEFT_HOLD, &input->LEFT_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (SDL_GameControllerGetAxis(input->controller, SDL_CONTROLLER_AXIS_LEFTX) < -20000) {
				if (!input->locked) { cr_input_set_held(&input->LEFT_HOLD, &input->LEFT_PRESS); }
				if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
			}
			else {
				if (!input->locked) { cr_input_set_released(&input->LEFT_HOLD, &input->LEFT_RELEASE, &input->LEFT_PRESS); }
			}
		}

		if (SDL_GameControllerGetButton(input->controller, input->RIGHT_CONTMAP)) {
			if (!input->locked) { cr_input_set_held(&input->RIGHT_HOLD, &input->RIGHT_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (SDL_GameControllerGetAxis(input->controller, SDL_CONTROLLER_AXIS_LEFTX) > 20000) {
				if (!input->locked) { cr_input_set_held(&input->RIGHT_HOLD, &input->RIGHT_PRESS); }
				if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
			}
			else {
				if (!input->locked) { cr_input_set_released(&input->RIGHT_HOLD, &input->RIGHT_RELEASE, &input->RIGHT_PRESS); }
			}
		}

		if (SDL_GameControllerGetButton(input->controller, input->START_CONTMAP)) {
			if (!input->locked) { cr_input_set_held(&input->START_HOLD, &input->START_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->START_HOLD, &input->START_RELEASE, &input->START_PRESS); }
		}

		if (SDL_GameControllerGetButton(input->controller, input->SELECT_CONTMAP)) {
			if (!input->locked) { cr_input_set_held(&input->SELECT_HOLD, &input->SELECT_PRESS); }
			if (!input->locked) { cr_input_set_held(&input->BUTTON_HOLD, &input->BUTTON_PRESS); }
		}
		else {
			if (!input->locked) { cr_input_set_released(&input->SELECT_HOLD, &input->SELECT_RELEASE, &input->SELECT_PRESS); }
		}

		break;
	}

	if (
		keystate[input->A_KEYMAP] ||
		keystate[input->B_KEYMAP] ||
		keystate[input->X_KEYMAP] ||
		keystate[input->Y_KEYMAP] ||
		keystate[input->L_KEYMAP] ||
		keystate[input->R_KEYMAP] ||
		keystate[input->UP_KEYMAP] ||
		keystate[input->DOWN_KEYMAP] ||
		keystate[input->LEFT_KEYMAP] ||
		keystate[input->RIGHT_KEYMAP] ||
		keystate[input->START_KEYMAP]
		) {
		input->input_type = CR_WINDOWS_INPUT_KEYS;
	}
	else {
		if (input->input_type == CR_WINDOWS_INPUT_KEYS) {
			if (!input->locked) { cr_input_set_released(&input->BUTTON_HOLD, &input->BUTTON_RELEASE, &input->BUTTON_PRESS); }
		}
	}

	if (
		SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_A) ||
		SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_B) ||
		SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_X) ||
		SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_Y) ||
		SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) ||
		SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) ||
		SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_DPAD_UP) ||
		SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN) ||
		SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT) ||
		SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) ||
		SDL_GameControllerGetButton(input->controller, SDL_CONTROLLER_BUTTON_START)
		) {
		input->input_type = CR_WINDOWS_INPUT_CONTROLLER;
	}
	else {
		if (input->input_type == CR_WINDOWS_INPUT_CONTROLLER) {
			if (!input->locked) { cr_input_set_released(&input->BUTTON_HOLD, &input->BUTTON_RELEASE, &input->BUTTON_PRESS); }
		}
	}
}

void cr_windows_controller_init(unsigned char cont_id)
{
	cr_input[cont_id].controller = SDL_GameControllerOpen(cont_id);
}

void cr_windows_input_map_button(int *button_to_map, int mapping_button)
{
	*button_to_map = mapping_button;
}

void cr_windows_input_map_default_buttons(CR_INPUT_STRUCT *input)
{
	cr_windows_input_map_button(&input->A_CONTMAP, SDL_CONTROLLER_BUTTON_A);
	cr_windows_input_map_button(&input->B_CONTMAP, SDL_CONTROLLER_BUTTON_B);
	cr_windows_input_map_button(&input->X_CONTMAP, SDL_CONTROLLER_BUTTON_X);
	cr_windows_input_map_button(&input->Y_CONTMAP, SDL_CONTROLLER_BUTTON_Y);

	cr_windows_input_map_button(&input->L_CONTMAP, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	cr_windows_input_map_button(&input->R_CONTMAP, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);

	cr_windows_input_map_button(&input->UP_CONTMAP, SDL_CONTROLLER_BUTTON_DPAD_UP);
	cr_windows_input_map_button(&input->DOWN_CONTMAP, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	cr_windows_input_map_button(&input->LEFT_CONTMAP, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	cr_windows_input_map_button(&input->RIGHT_CONTMAP, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

	cr_windows_input_map_button(&input->START_CONTMAP, SDL_CONTROLLER_BUTTON_START);
	cr_windows_input_map_button(&input->SELECT_CONTMAP, SDL_CONTROLLER_BUTTON_BACK);

	input->input_type = CR_WINDOWS_INPUT_CONTROLLER;
}

void cr_windows_input_map_key(int *key_to_map, SDL_Keycode mapping_key)
{
	*key_to_map = mapping_key;
}

void cr_windows_input_map_default_keys(CR_INPUT_STRUCT *input, unsigned char count)
{
	switch (count)
	{
	case 0:
		cr_windows_input_map_key(&input->A_KEYMAP, SDL_SCANCODE_Z);
		cr_windows_input_map_key(&input->B_KEYMAP, SDL_SCANCODE_X);
		cr_windows_input_map_key(&input->X_KEYMAP, SDL_SCANCODE_A);
		cr_windows_input_map_key(&input->Y_KEYMAP, SDL_SCANCODE_S);

		cr_windows_input_map_key(&input->L_KEYMAP, SDL_SCANCODE_Q);
		cr_windows_input_map_key(&input->R_KEYMAP, SDL_SCANCODE_W);

		cr_windows_input_map_key(&input->UP_KEYMAP, SDL_SCANCODE_UP);
		cr_windows_input_map_key(&input->DOWN_KEYMAP, SDL_SCANCODE_DOWN);
		cr_windows_input_map_key(&input->LEFT_KEYMAP, SDL_SCANCODE_LEFT);
		cr_windows_input_map_key(&input->RIGHT_KEYMAP, SDL_SCANCODE_RIGHT);

		cr_windows_input_map_key(&input->START_KEYMAP, SDL_SCANCODE_RETURN);
		cr_windows_input_map_key(&input->SELECT_KEYMAP, SDL_SCANCODE_BACKSPACE);
		break;

	case 1:
		cr_windows_input_map_key(&input->A_KEYMAP, SDL_SCANCODE_C);
		cr_windows_input_map_key(&input->B_KEYMAP, SDL_SCANCODE_V);
		cr_windows_input_map_key(&input->X_KEYMAP, SDL_SCANCODE_D);
		cr_windows_input_map_key(&input->Y_KEYMAP, SDL_SCANCODE_F);

		cr_windows_input_map_key(&input->UP_KEYMAP, SDL_SCANCODE_I);
		cr_windows_input_map_key(&input->DOWN_KEYMAP, SDL_SCANCODE_K);
		cr_windows_input_map_key(&input->LEFT_KEYMAP, SDL_SCANCODE_J);
		cr_windows_input_map_key(&input->RIGHT_KEYMAP, SDL_SCANCODE_L);
		break;
	}
}

void cr_windows_input_adjust_mapping(void)
{
	for (int i = 0; i < 4; i++) {

		// Keyboard

		cr_windows_input_adjust_keymap(&cr_input[i].A_KEYMAP);
		cr_windows_input_adjust_keymap(&cr_input[i].B_KEYMAP);
		cr_windows_input_adjust_keymap(&cr_input[i].X_KEYMAP);
		cr_windows_input_adjust_keymap(&cr_input[i].Y_KEYMAP);

		cr_windows_input_adjust_keymap(&cr_input[i].L_KEYMAP);
		cr_windows_input_adjust_keymap(&cr_input[i].R_KEYMAP);

		cr_windows_input_adjust_keymap(&cr_input[i].UP_KEYMAP);
		cr_windows_input_adjust_keymap(&cr_input[i].DOWN_KEYMAP);
		cr_windows_input_adjust_keymap(&cr_input[i].LEFT_KEYMAP);
		cr_windows_input_adjust_keymap(&cr_input[i].RIGHT_KEYMAP);

		cr_windows_input_adjust_keymap(&cr_input[i].START_KEYMAP);
		cr_windows_input_adjust_keymap(&cr_input[i].SELECT_KEYMAP);

		// Controller 

		cr_windows_input_adjust_contmap(&cr_input[i].A_CONTMAP);
		cr_windows_input_adjust_contmap(&cr_input[i].B_CONTMAP);
		cr_windows_input_adjust_contmap(&cr_input[i].X_CONTMAP);
		cr_windows_input_adjust_contmap(&cr_input[i].Y_CONTMAP);

		cr_windows_input_adjust_contmap(&cr_input[i].L_CONTMAP);
		cr_windows_input_adjust_contmap(&cr_input[i].R_CONTMAP);

		cr_windows_input_adjust_contmap(&cr_input[i].UP_CONTMAP);
		cr_windows_input_adjust_contmap(&cr_input[i].DOWN_CONTMAP);
		cr_windows_input_adjust_contmap(&cr_input[i].LEFT_CONTMAP);
		cr_windows_input_adjust_contmap(&cr_input[i].RIGHT_CONTMAP);

		cr_windows_input_adjust_contmap(&cr_input[i].START_CONTMAP);
		cr_windows_input_adjust_contmap(&cr_input[i].SELECT_CONTMAP);
	}
}

void cr_windows_input_adjust_keymap(int *map)
{
	switch (*map)
	{
	case CR_KEYCODE_0:
		*map = SDL_SCANCODE_0;
		break;
	case CR_KEYCODE_1:
		*map = SDL_SCANCODE_1;
		break;
	case CR_KEYCODE_2:
		*map = SDL_SCANCODE_2;
		break;
	case CR_KEYCODE_3:
		*map = SDL_SCANCODE_3;
		break;
	case CR_KEYCODE_4:
		*map = SDL_SCANCODE_4;
		break;
	case CR_KEYCODE_5:
		*map = SDL_SCANCODE_5;
		break;
	case CR_KEYCODE_6:
		*map = SDL_SCANCODE_6;
		break;
	case CR_KEYCODE_7:
		*map = SDL_SCANCODE_7;
		break;
	case CR_KEYCODE_8:
		*map = SDL_SCANCODE_8;
		break;
	case CR_KEYCODE_9:
		*map = SDL_SCANCODE_9;
		break;
	case CR_KEYCODE_Q:
		*map = SDL_SCANCODE_Q;
		break;
	case CR_KEYCODE_W:
		*map = SDL_SCANCODE_W;
		break;
	case CR_KEYCODE_E:
		*map = SDL_SCANCODE_E;
		break;
	case CR_KEYCODE_R:
		*map = SDL_SCANCODE_R;
		break;
	case CR_KEYCODE_T:
		*map = SDL_SCANCODE_T;
		break;
	case CR_KEYCODE_Y:
		*map = SDL_SCANCODE_Y;
		break;
	case CR_KEYCODE_U:
		*map = SDL_SCANCODE_U;
		break;
	case CR_KEYCODE_I:
		*map = SDL_SCANCODE_I;
		break;
	case CR_KEYCODE_O:
		*map = SDL_SCANCODE_O;
		break;
	case CR_KEYCODE_P:
		*map = SDL_SCANCODE_P;
		break;
	case CR_KEYCODE_A:
		*map = SDL_SCANCODE_A;
		break;
	case CR_KEYCODE_S:
		*map = SDL_SCANCODE_S;
		break;
	case CR_KEYCODE_D:
		*map = SDL_SCANCODE_D;
		break;
	case CR_KEYCODE_F:
		*map = SDL_SCANCODE_F;
		break;
	case CR_KEYCODE_G:
		*map = SDL_SCANCODE_G;
		break;
	case CR_KEYCODE_H:
		*map = SDL_SCANCODE_H;
		break;
	case CR_KEYCODE_J:
		*map = SDL_SCANCODE_J;
		break;
	case CR_KEYCODE_K:
		*map = SDL_SCANCODE_K;
		break;
	case CR_KEYCODE_L:
		*map = SDL_SCANCODE_L;
		break;
	case CR_KEYCODE_Z:
		*map = SDL_SCANCODE_Z;
		break;
	case CR_KEYCODE_X:
		*map = SDL_SCANCODE_X;
		break;
	case CR_KEYCODE_C:
		*map = SDL_SCANCODE_C;
		break;
	case CR_KEYCODE_V:
		*map = SDL_SCANCODE_V;
		break;
	case CR_KEYCODE_B:
		*map = SDL_SCANCODE_B;
		break;
	case CR_KEYCODE_N:
		*map = SDL_SCANCODE_N;
		break;
	case CR_KEYCODE_M:
		*map = SDL_SCANCODE_M;
		break;
	case CR_KEYCODE_GRAVE:
		*map = SDL_SCANCODE_GRAVE;
		break;
	case CR_KEYCODE_MINUS:
		*map = SDL_SCANCODE_MINUS;
		break;
	case CR_KEYCODE_EQUAL:
		*map = SDL_SCANCODE_EQUALS;
		break;
	case CR_KEYCODE_TAB:
		*map = SDL_SCANCODE_TAB;
		break;
	case CR_KEYCODE_BRACKET_OPEN:
		*map = SDL_SCANCODE_LEFTBRACKET;
		break;
	case CR_KEYCODE_BRACKET_CLOSE:
		*map = SDL_SCANCODE_RIGHTBRACKET;
		break;
	case CR_KEYCODE_BACKSLASH:
		*map = SDL_SCANCODE_BACKSLASH;
		break;
	case CR_KEYCODE_SEMICOLON:
		*map = SDL_SCANCODE_SEMICOLON;
		break;
	case CR_KEYCODE_APOSTROPHE:
		*map = SDL_SCANCODE_APOSTROPHE;
		break;
	case CR_KEYCODE_RETURN:
		*map = SDL_SCANCODE_RETURN;
		break;
	case CR_KEYCODE_LEFT_SHIFT:
		*map = SDL_SCANCODE_LSHIFT;
		break;
	case CR_KEYCODE_COMMA:
		*map = SDL_SCANCODE_COMMA;
		break;
	case CR_KEYCODE_PERIOD:
		*map = SDL_SCANCODE_PERIOD;
		break;
	case CR_KEYCODE_FORWARD_SLASH:
		*map = SDL_SCANCODE_SLASH;
		break;
	case CR_KEYCODE_RIGHT_SHIFT:
		*map = SDL_SCANCODE_RSHIFT;
		break;
	case CR_KEYCODE_LEFT_CONTROL:
		*map = SDL_SCANCODE_LCTRL;
		break;
	case CR_KEYCODE_LEFT_ALT:
		*map = SDL_SCANCODE_LALT;
		break;
	case CR_KEYCODE_SPACE:
		*map = SDL_SCANCODE_SPACE;
		break;
	case CR_KEYCODE_RIGHT_ALT:
		*map = SDL_SCANCODE_RALT;
		break;
	case CR_KEYCODE_RIGHT_CONTROL:
		*map = SDL_SCANCODE_RCTRL;
		break;
	case CR_KEYCODE_KEY_UP:
		*map = SDL_SCANCODE_UP;
		break;
	case CR_KEYCODE_KEY_DOWN:
		*map = SDL_SCANCODE_DOWN;
		break;
	case CR_KEYCODE_KEY_LEFT:
		*map = SDL_SCANCODE_LEFT;
		break;
	case CR_KEYCODE_KEY_RIGHT:
		*map = SDL_SCANCODE_RIGHT;
		break;
	}
}

void cr_windows_input_adjust_contmap(int *map)
{
	switch (*map)
	{
	case CR_BUTTON_A:
		*map = SDL_CONTROLLER_BUTTON_A;
		break;
	case CR_BUTTON_B:
		*map = SDL_CONTROLLER_BUTTON_B;
		break;
	case CR_BUTTON_X:
		*map = SDL_CONTROLLER_BUTTON_X;
		break;
	case CR_BUTTON_Y:
		*map = SDL_CONTROLLER_BUTTON_Y;
		break;
	case CR_BUTTON_L:
		*map = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
		break;
	case CR_BUTTON_R:
		*map = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
		break;
	case CR_BUTTON_UP:
		*map = SDL_CONTROLLER_BUTTON_DPAD_UP;
		break;
	case CR_BUTTON_DOWN:
		*map = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
		break;
	case CR_BUTTON_LEFT:
		*map = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
		break;
	case CR_BUTTON_RIGHT:
		*map = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
		break;
	case CR_BUTTON_START:
		*map = SDL_CONTROLLER_BUTTON_START;
		break;
	case CR_BUTTON_SELECT:
		*map = SDL_CONTROLLER_BUTTON_BACK;
		break;
	}
}

void cr_windows_input_restore_mapping(void)
{
	for (int i = 0; i < 4; i++) {

		// Keyboard

		cr_windows_input_restore_keymap(&cr_input[i].A_KEYMAP);
		cr_windows_input_restore_keymap(&cr_input[i].B_KEYMAP);
		cr_windows_input_restore_keymap(&cr_input[i].X_KEYMAP);
		cr_windows_input_restore_keymap(&cr_input[i].Y_KEYMAP);

		cr_windows_input_restore_keymap(&cr_input[i].L_KEYMAP);
		cr_windows_input_restore_keymap(&cr_input[i].R_KEYMAP);

		cr_windows_input_restore_keymap(&cr_input[i].UP_KEYMAP);
		cr_windows_input_restore_keymap(&cr_input[i].DOWN_KEYMAP);
		cr_windows_input_restore_keymap(&cr_input[i].LEFT_KEYMAP);
		cr_windows_input_restore_keymap(&cr_input[i].RIGHT_KEYMAP);

		cr_windows_input_restore_keymap(&cr_input[i].START_KEYMAP);
		cr_windows_input_restore_keymap(&cr_input[i].SELECT_KEYMAP);

		// Controller 

		cr_windows_input_restore_contmap(&cr_input[i].A_CONTMAP);
		cr_windows_input_restore_contmap(&cr_input[i].B_CONTMAP);
		cr_windows_input_restore_contmap(&cr_input[i].X_CONTMAP);
		cr_windows_input_restore_contmap(&cr_input[i].Y_CONTMAP);

		cr_windows_input_restore_contmap(&cr_input[i].L_CONTMAP);
		cr_windows_input_restore_contmap(&cr_input[i].R_CONTMAP);

		cr_windows_input_restore_contmap(&cr_input[i].UP_CONTMAP);
		cr_windows_input_restore_contmap(&cr_input[i].DOWN_CONTMAP);
		cr_windows_input_restore_contmap(&cr_input[i].LEFT_CONTMAP);
		cr_windows_input_restore_contmap(&cr_input[i].RIGHT_CONTMAP);

		cr_windows_input_restore_contmap(&cr_input[i].START_CONTMAP);
		cr_windows_input_restore_contmap(&cr_input[i].SELECT_CONTMAP);
	}
}

void cr_windows_input_restore_keymap(int *map)
{
	switch (*map)
	{
	case SDL_SCANCODE_0:
		*map = CR_KEYCODE_0;
		break;
	case SDL_SCANCODE_1:
		*map = CR_KEYCODE_1;
		break;
	case SDL_SCANCODE_2:
		*map = CR_KEYCODE_2;
		break;
	case SDL_SCANCODE_3:
		*map = CR_KEYCODE_3;
		break;
	case SDL_SCANCODE_4:
		*map = CR_KEYCODE_4;
		break;
	case SDL_SCANCODE_5:
		*map = CR_KEYCODE_5;
		break;
	case SDL_SCANCODE_6:
		*map = CR_KEYCODE_6;
		break;
	case SDL_SCANCODE_7:
		*map = CR_KEYCODE_7;
		break;
	case SDL_SCANCODE_8:
		*map = CR_KEYCODE_8;
		break;
	case SDL_SCANCODE_9:
		*map = CR_KEYCODE_9;
		break;
	case SDL_SCANCODE_Q:
		*map = CR_KEYCODE_Q;
		break;
	case SDL_SCANCODE_W:
		*map = CR_KEYCODE_W;
		break;
	case SDL_SCANCODE_E:
		*map = CR_KEYCODE_E;
		break;
	case SDL_SCANCODE_R:
		*map = CR_KEYCODE_R;
		break;
	case SDL_SCANCODE_T:
		*map = CR_KEYCODE_T;
		break;
	case SDL_SCANCODE_Y:
		*map = CR_KEYCODE_Y;
		break;
	case SDL_SCANCODE_U:
		*map = CR_KEYCODE_U;
		break;
	case SDL_SCANCODE_I:
		*map = CR_KEYCODE_I;
		break;
	case SDL_SCANCODE_O:
		*map = CR_KEYCODE_O;
		break;
	case SDL_SCANCODE_P:
		*map = CR_KEYCODE_P;
		break;
	case SDL_SCANCODE_A:
		*map = CR_KEYCODE_A;
		break;
	case SDL_SCANCODE_S:
		*map = CR_KEYCODE_S;
		break;
	case SDL_SCANCODE_D:
		*map = CR_KEYCODE_D;
		break;
	case SDL_SCANCODE_F:
		*map = CR_KEYCODE_F;
		break;
	case SDL_SCANCODE_G:
		*map = CR_KEYCODE_G;
		break;
	case SDL_SCANCODE_H:
		*map = CR_KEYCODE_H;
		break;
	case SDL_SCANCODE_J:
		*map = CR_KEYCODE_J;
		break;
	case SDL_SCANCODE_K:
		*map = CR_KEYCODE_K;
		break;
	case SDL_SCANCODE_L:
		*map = CR_KEYCODE_L;
		break;
	case SDL_SCANCODE_Z:
		*map = CR_KEYCODE_Z;
		break;
	case SDL_SCANCODE_X:
		*map = CR_KEYCODE_X;
		break;
	case SDL_SCANCODE_C:
		*map = CR_KEYCODE_C;
		break;
	case SDL_SCANCODE_V:
		*map = CR_KEYCODE_V;
		break;
	case SDL_SCANCODE_B:
		*map = CR_KEYCODE_B;
		break;
	case SDL_SCANCODE_N:
		*map = CR_KEYCODE_N;
		break;
	case SDL_SCANCODE_M:
		*map = CR_KEYCODE_M;
		break;
	case SDL_SCANCODE_GRAVE:
		*map = CR_KEYCODE_GRAVE;
		break;
	case SDL_SCANCODE_MINUS:
		*map = CR_KEYCODE_MINUS;
		break;
	case SDL_SCANCODE_EQUALS:
		*map = CR_KEYCODE_EQUAL;
		break;
	case SDL_SCANCODE_TAB:
		*map = CR_KEYCODE_TAB;
		break;
	case SDL_SCANCODE_LEFTBRACKET:
		*map = CR_KEYCODE_BRACKET_OPEN;
		break;
	case SDL_SCANCODE_RIGHTBRACKET:
		*map = CR_KEYCODE_BRACKET_CLOSE;
		break;
	case SDL_SCANCODE_BACKSLASH:
		*map = CR_KEYCODE_BACKSLASH;
		break;
	case SDL_SCANCODE_SEMICOLON:
		*map = CR_KEYCODE_SEMICOLON;
		break;
	case SDL_SCANCODE_APOSTROPHE:
		*map = CR_KEYCODE_APOSTROPHE;
		break;
	case SDL_SCANCODE_RETURN:
		*map = CR_KEYCODE_RETURN;
		break;
	case SDL_SCANCODE_LSHIFT:
		*map = CR_KEYCODE_LEFT_SHIFT;
		break;
	case SDL_SCANCODE_COMMA:
		*map = CR_KEYCODE_COMMA;
		break;
	case SDL_SCANCODE_PERIOD:
		*map = CR_KEYCODE_PERIOD;
		break;
	case SDL_SCANCODE_SLASH:
		*map = CR_KEYCODE_FORWARD_SLASH;
		break;
	case SDL_SCANCODE_RSHIFT:
		*map = CR_KEYCODE_RIGHT_SHIFT;
		break;
	case SDL_SCANCODE_LCTRL:
		*map = CR_KEYCODE_LEFT_CONTROL;
		break;
	case SDL_SCANCODE_LALT:
		*map = CR_KEYCODE_LEFT_ALT;
		break;
	case SDL_SCANCODE_SPACE:
		*map = CR_KEYCODE_SPACE;
		break;
	case SDL_SCANCODE_RALT:
		*map = CR_KEYCODE_RIGHT_ALT;
		break;
	case SDL_SCANCODE_RCTRL:
		*map = CR_KEYCODE_RIGHT_CONTROL;
		break;
	case SDL_SCANCODE_UP:
		*map = CR_KEYCODE_KEY_UP;
		break;
	case SDL_SCANCODE_DOWN:
		*map = CR_KEYCODE_KEY_DOWN;
		break;
	case SDL_SCANCODE_LEFT:
		*map = CR_KEYCODE_KEY_LEFT;
		break;
	case SDL_SCANCODE_RIGHT:
		*map = CR_KEYCODE_KEY_RIGHT;
		break;
	}
}

void cr_windows_input_restore_contmap(int *map)
{
	switch (*map)
	{
	case SDL_CONTROLLER_BUTTON_A:
		*map = CR_BUTTON_A;
		break;
	case SDL_CONTROLLER_BUTTON_B:
		*map = CR_BUTTON_B;
		break;
	case SDL_CONTROLLER_BUTTON_X:
		*map = CR_BUTTON_X;
		break;
	case SDL_CONTROLLER_BUTTON_Y:
		*map = CR_BUTTON_Y;
		break;
	case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
		*map = CR_BUTTON_L;
		break;
	case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
		*map = CR_BUTTON_R;
		break;
	case SDL_CONTROLLER_BUTTON_DPAD_UP:
		*map = CR_BUTTON_UP;
		break;
	case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
		*map = CR_BUTTON_DOWN;
		break;
	case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
		*map = CR_BUTTON_LEFT;
		break;
	case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
		*map = CR_BUTTON_RIGHT;
		break;
	case SDL_CONTROLLER_BUTTON_START:
		*map = CR_BUTTON_START;
		break;
	case SDL_CONTROLLER_BUTTON_BACK:
		*map = CR_BUTTON_SELECT;
		break;
	}
}