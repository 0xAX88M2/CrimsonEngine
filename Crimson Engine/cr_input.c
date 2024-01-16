#include "crimson.h"

#if CR_PLATFORM == CR_WINDOWS
#include "cr_win_input.h"
#endif

void cr_input_set_held(bool *button_hold_value, bool *button_pressed_value)
{
	//Register button press (true for 1 frame)
	*button_pressed_value = false;
	if (*button_hold_value == false) {
		*button_pressed_value = true;
	}
	else {
		*button_pressed_value = false;
	}

	//Set "button is being held" to true
	*button_hold_value = true;
}

void cr_input_set_released(bool *button_hold_value, bool *button_released_value, bool *button_pressed_value)
{
	//Here, we check if the button is held before it gets set back to false. If it IS true, then that means the button WAS held on the last frame. We can set the release flag.
	*button_pressed_value = false;
	if (*button_hold_value == true) {
		*button_released_value = true;
	}
	else { //Reset the release value if the button was not held.
		*button_released_value = false;
	}

	//Set held to false.
	*button_hold_value = false;
}

void cr_input_clear(CR_INPUT_STRUCT *input)
{
	input->A_PRESS = false;
	input->A_HOLD = false;
	input->A_RELEASE = false;

	input->B_PRESS = false;
	input->B_HOLD = false;
	input->B_RELEASE = false;

	input->X_PRESS = false;
	input->X_HOLD = false;
	input->X_RELEASE = false;

	input->Y_PRESS = false;
	input->Y_HOLD = false;
	input->Y_RELEASE = false;

	input->L_PRESS = false;
	input->L_HOLD = false;
	input->L_RELEASE = false;

	input->R_PRESS = false;
	input->R_HOLD = false;
	input->R_RELEASE = false;

	input->UP_PRESS = false;
	input->UP_HOLD = false;
	input->UP_RELEASE = false;

	input->DOWN_PRESS = false;
	input->DOWN_HOLD = false;
	input->DOWN_RELEASE = false;

	input->LEFT_PRESS = false;
	input->LEFT_HOLD = false;
	input->LEFT_RELEASE = false;

	input->RIGHT_PRESS = false;
	input->RIGHT_HOLD = false;
	input->RIGHT_RELEASE = false;

	input->START_PRESS = false;
	input->START_HOLD = false;
	input->START_RELEASE = false;

	input->SELECT_PRESS = false;
	input->SELECT_HOLD = false;
	input->SELECT_RELEASE = false;

	input->skip_update = true;
}

void cr_input_adjust_mapping(void)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_input_adjust_mapping();
#endif

}

void cr_input_restore_mapping(void)
{

#if CR_PLATFORM == CR_WINDOWS
	cr_windows_input_restore_mapping();
#endif

}