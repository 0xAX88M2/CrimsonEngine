#include "crimson.h"

//LOADING FUNCTIONS

CR_TEXTURE* cr_sprite_load(const char *filepath, int transparent_color)
{

#if CR_PLATFORM == CR_WINDOWS
	return cr_windows_sprite_load(filepath, transparent_color);
#endif // !CR_PLATFORM == CR_WINDOWS

}

void cr_sprite_unload(CR_TEXTURE *sprite_id)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_sprite_unload(sprite_id);
#endif // !CR_PLATFORM == CR_WINDOWS
}

void cr_sprite_manager_reset(bool unload_globals)
{
	//if (cr_sprites.initialized) {
		for (unsigned int i = 0; i < cr_sprites.sequence_count; i++) {
			cr_anim_sequence_remove(cr_sprites.sequence_pointers[i]);
		}

		if (crimson.exitcode) {
			free(cr_sprites.sequence_pointers);
		}
		else {
			CR_ANIM** animptrs = realloc(cr_sprites.sequence_pointers, 0 * sizeof(CR_ANIM*));
			cr_sprites.sequence_pointers = animptrs;
			cr_sprites.sequence_count = 0;
		}

		if (unload_globals) {
			for (unsigned int i = 0; i < cr_sprites.global_sequence_count; i++) {
				cr_anim_sequence_remove(cr_sprites.global_sequence_pointers[i]);
			}

			if (crimson.exitcode) {
				free(cr_sprites.global_sequence_pointers);
			}
			else {
				CR_ANIM** animptrs = realloc(cr_sprites.global_sequence_pointers, 0 * sizeof(CR_ANIM*));
				cr_sprites.global_sequence_pointers = animptrs;
				cr_sprites.global_sequence_count = 0;
			}
		}

		cr_sprites.initialized = false;
	//}
}

//ANIM FUNCTIONS

void cr_script_load(const char *filepath, CR_ANIM_DATA *anim_data, CR_ANIM *anim, unsigned short start_id) 
{

	if (anim_data->initialized) {
		return;
		//cr_script_unload(anim_data);
	}

	unsigned char *buffer;

	FILE *in_file = fopen(filepath, "rb");
	if (in_file == NULL) { perror("error"); }
	if (!in_file) {
#if CR_DEBUG_SPRITES == 1
		printf("Couldn't open '%s'!\n", filepath);
#endif
		return;
	}
	else {
#if CR_DEBUG_SPRITES == 1
		printf("Successfully opened anim script at '%s'\n", filepath);
#endif
	}

	fseek(in_file, 0, SEEK_END);
	int file_size = ftell(in_file);
	fseek(in_file, 0, SEEK_SET);

	buffer = malloc(file_size * sizeof(char));
	fread(buffer, file_size, 1, in_file);
	fclose(in_file);

	int file_pos = 0;
	anim_data->count = (buffer[file_pos] << 8) | buffer[file_pos + 1];		// Get the number of animations from the .cra file
	anim_data->scripts = malloc(anim_data->count * sizeof(CR_ANIM_SCRIPT*));	 // Allocate enough memory to store the file's scripts
	file_pos += 2;														// Advance in the buffer to the sheet data

	anim_data->sheet_cnt = buffer[file_pos];								// Get the number of sheets used from the .cra file
	anim_data->sheets = malloc(anim_data->sheet_cnt * sizeof(CR_TEXTURE*));	// Allocate enough memory to store the file's sheets
	file_pos++;															// Advance in the buffer to the sheet information

	// Storing the directory path to use to access each sheet relative to the script
	int dir_len = strlen(strrchr(filepath, '/'));
	char *dir_path = calloc(strlen(filepath) + 1 - dir_len + 1, sizeof(char));
	strncpy(dir_path, filepath, strlen(filepath) + 1 - dir_len);

	// For each animation sheet defined in the file
	for (int c = 0; c < anim_data->sheet_cnt; c++) {
		unsigned char sheet_filename_len = buffer[file_pos];			// Get the sheet's filename length
		file_pos++;

		char *path = calloc(sheet_filename_len + 1, sizeof(char));	// Allocate memory to store the sheet filename for the current script
		memcpy_s(path, sheet_filename_len + 1, buffer + file_pos, sheet_filename_len);	// Copy the sheet filename to the allocated memory
		file_pos += sheet_filename_len;										// Advance in the buffer to the script data
		char *fullpath = calloc(strlen(path) + strlen(dir_path) + 1, sizeof(char));
		strcpy(fullpath, dir_path);
		strcat(fullpath, path);
		//strcpy_s(fullpath, strlen(fullpath), dir_path);
		//strcat(fullpath, path);
		free(path);

		unsigned int sheet_trans_clr = (buffer[file_pos] << 16) | (buffer[file_pos + 1] << 8) | (buffer[file_pos + 2]);
		file_pos += 3;

		anim_data->sheets[c] = cr_sprite_load(fullpath, sheet_trans_clr);
		free(fullpath);
	}
	free(dir_path);

	for (int c = 0; c < anim_data->count; c++) {
		anim_data->scripts[c] = calloc(1, sizeof(CR_ANIM_SCRIPT));	// Allocate memory to store the animation script

		// Parsing the current script's data...
		anim_data->scripts[c]->sheet_id = buffer[file_pos + 0];			// Store the animation's sheet ID
		// If the defined sheet ID is greater than the amount of sheets loaded...
		if (anim_data->scripts[c]->sheet_id > anim_data->sheet_cnt) {
			anim_data->scripts[c]->sheet_id = anim_data->sheet_cnt;		// Set the ID to the highest avaliable sheet
		}
		int script_framecnt = buffer[file_pos + 1];						// Read the number of frames in the animation
		anim_data->scripts[c]->framecount = script_framecnt;				// Store this
		anim_data->scripts[c]->speed = buffer[file_pos + 2];				// Store the animation's speed
		anim_data->scripts[c]->type = buffer[file_pos + 3];				// Get the loop type of the animation
		anim_data->scripts[c]->loop_frame = buffer[file_pos + 4];		// Get the frame to loop to in the animation
		file_pos += 5;													// Advance in the buffer to the frame data

		CR_TILE* tileset = calloc(script_framecnt, sizeof(CR_TILE));	// Allocate memory to store the "tileset" of the script
		// For each of the frames...
		if (tileset != NULL) {
			for (int f = 0; f < script_framecnt; f++) {
				tileset[f].x = (buffer[file_pos + 0] << 8) | buffer[file_pos + 1];	// Get the frame's X origin
				tileset[f].y = (buffer[file_pos + 2] << 8) | buffer[file_pos + 3];	// Get the frame's Y origin
				tileset[f].width = buffer[file_pos + 4];					// Get the frame's width
				tileset[f].height = buffer[file_pos + 5];					// Get the frame's height
				tileset[f].disp_x = buffer[file_pos + 6];					// Get the frame's X displacement
				tileset[f].disp_y = buffer[file_pos + 7];					// Get the frame's Y displacement
				file_pos += 8;												// Advance to the next frame in the script
			}
			anim_data->scripts[c]->tileset = tileset;						// Store a pointer to the tileset we just created
		}
	}

	anim->id = start_id;
	cr_anim_add(anim_data->sheets[anim_data->scripts[anim->id]->sheet_id], anim, anim_data->scripts[anim->id]->tileset, anim_data->scripts[anim->id]->framecount, anim_data->scripts[anim->id]->speed, anim_data->scripts[anim->id]->type);

	anim_data->initialized = true;

	free(buffer);													// Free the file buffer
}

void cr_script_unload(CR_ANIM_DATA *anim_data) {

	if (anim_data->initialized) {
		// For every animation script loaded...
		for (int c = 0; c < anim_data->sheet_cnt; c++) {
			cr_sprite_unload(anim_data->sheets[c]);
		}

		free(anim_data->sheets);

		for (int c = 0; c < anim_data->count; c++) {
			free(anim_data->scripts[c]->tileset);							// Free the memory used to store the script's tileset
			free(anim_data->scripts[c]);									// Free the memory used to store the script
		}

		free(anim_data->scripts);											// Free the memory used to store the pointers to each script

#if CR_DEBUG_SPRITES
		printf("Successfully unloaded the animation data!\n");
#endif // CR_DEBUG_SPRITES

		anim_data->initialized = false;
	}
}

void cr_anim_add(CR_TEXTURE *sprite, CR_ANIM *anim_id, CR_TILE *tileset, unsigned short framecount, unsigned char speed, unsigned char type)
{

	anim_id->sprite = sprite;
	anim_id->tileset = tileset;

	anim_id->speed = speed;
	anim_id->frame_count = framecount;
	anim_id->frame_timer = 0;
	anim_id->loop_frame = 0;

	anim_id->anim_type = type;

	anim_id->frame_previous = -1;

	anim_id->changed = false;

	if (anim_id->anim_type == CR_ANIM_LOOP || anim_id->anim_type == CR_ANIM_LOOP_ONCE || anim_id->anim_type == CR_ANIM_PINGPONG) {

		anim_id->frame_current = 0;

		anim_id->spr_x = tileset[0].x;
		anim_id->spr_y = tileset[0].y;
		anim_id->width = tileset[0].width;
		anim_id->height = tileset[0].height;
		anim_id->disp_x = tileset[0].disp_x;
		anim_id->disp_y = tileset[0].disp_y;

	}
	else {

		anim_id->frame_current = framecount - 1;

		anim_id->spr_x = tileset[framecount - 1].x;
		anim_id->spr_y = tileset[framecount - 1].y;
		anim_id->width = tileset[framecount - 1].width;
		anim_id->height = tileset[framecount - 1].height;
		anim_id->disp_x = tileset[framecount - 1].disp_x;
		anim_id->disp_y = tileset[framecount - 1].disp_y;

	}

	anim_id->finished = false;
	anim_id->initialized = true;
	anim_id->sequence = false;

#if CR_DEBUG_SPRITES == 1
	printf("Anim %p loaded: %d frames\n", anim_id, anim_id->frame_count);
#endif
}

void cr_anim_sequence_add(CR_ANIM *anim_id, CR_ANIM_TYPE anim_type, const unsigned short framecount)
{
	if (anim_id != NULL) {

		anim_id->spritelist = malloc(framecount * sizeof(CR_TEXTURE*));

		if (!anim_id->global) {
			cr_sprites.sequence_count++;
			CR_ANIM** animptrs = realloc(cr_sprites.sequence_pointers, cr_sprites.sequence_count * sizeof(CR_ANIM*));

			if (animptrs != NULL) {
				cr_sprites.sequence_pointers = animptrs;
			}

			cr_sprites.sequence_pointers[cr_sprites.sequence_count - 1] = anim_id;
		}
		else {
			cr_sprites.global_sequence_count++;
			CR_ANIM** animptrs = realloc(cr_sprites.global_sequence_pointers, cr_sprites.global_sequence_count * sizeof(CR_ANIM*));

			if (animptrs != NULL) {
				cr_sprites.global_sequence_pointers = animptrs;
			}

			cr_sprites.global_sequence_pointers[cr_sprites.global_sequence_count - 1] = anim_id;
		}
		cr_sprites.initialized = true;

		anim_id->speed = 4;
		anim_id->frame_count = framecount;
		anim_id->frame_timer = 0;
		anim_id->loop_frame = 0;

		anim_id->anim_type = anim_type;

		//printf("framecount %d\n", cr_sprites.sequence_pointers[cr_sprites.sequence_count - 1]->frame_count);

		if (anim_id->anim_type == CR_ANIM_LOOP || anim_id->anim_type == CR_ANIM_LOOP_ONCE || anim_id->anim_type == CR_ANIM_PINGPONG) {
			anim_id->frame_current = 0;
		}
		else {
			anim_id->frame_current = framecount - 1;
		}

		anim_id->finished = false;
		anim_id->initialized = true;
		anim_id->sequence = true;

#if CR_DEBUG_SPRITES == 1
		printf("Sequence %p added: %d frames\n", anim_id, anim_id->frame_count);
#endif
	}
}

void cr_anim_sequence_remove(CR_ANIM *anim_id)
{
	if (!anim_id->sequence) { return; }

	if (anim_id->spritelist != NULL) {
		for (unsigned int i = 0; i < anim_id->frame_count; i++) {
			cr_sprite_unload(anim_id->spritelist[i]);
		}
		free(anim_id->spritelist);
	}
}

void cr_anim_reset(CR_ANIM *anim_id)
{
	//if (anim_id->sequence) {
	//	free(anim_id->spritelist);
	//}

	anim_id->sprite = NULL;
	anim_id->tileset = NULL;

	anim_id->speed = 4;
	anim_id->frame_count = 0;
	anim_id->frame_current = 0;
	anim_id->frame_timer = 0;
	anim_id->loop_frame = 0;

	anim_id->spr_x = 0;
	anim_id->spr_y = 0;
	anim_id->width = 0;
	anim_id->height = 0;

	anim_id->anim_type = 0;

	anim_id->finished = false;
	anim_id->initialized = false;
	anim_id->sequence = false;

#if CR_DEBUG_SPRITES == 1
	printf("Anim %p reset.\n", anim_id);
#endif
}

void cr_anim_unload(CR_ANIM *anim_id)
{
	if (anim_id->sequence) {
		cr_anim_sequence_remove(anim_id);
	}
	else {
		cr_sprite_unload(anim_id->sprite);
	}

	anim_id->sprite = NULL;
	anim_id->tileset = NULL;

	anim_id->speed = 4;
	anim_id->frame_count = 0;
	anim_id->frame_current = 0;
	anim_id->frame_timer = 0;
	anim_id->loop_frame = 0;

	anim_id->spr_x = 0;
	anim_id->spr_y = 0;
	anim_id->width = 0;
	anim_id->height = 0;

	anim_id->anim_type = 0;

	anim_id->finished = false;
	anim_id->initialized = false;
	anim_id->sequence = false;

#if CR_DEBUG_SPRITES == 1
	if (anim_id->sequence) {
		printf("Sequence %p unloaded.\n", anim_id);
	}
	else {
		printf("Anim %p unloaded.\n", anim_id);
	}
#endif
}

void cr_script_add(CR_ANIM_DATA *anim_data, CR_ANIM *anim_id, unsigned short anim_num)
{
	cr_script_add_sheet(anim_data, anim_id, anim_num, anim_num);
}

void cr_script_add_sheet(CR_ANIM_DATA* anim_data, CR_ANIM* anim_id, unsigned short anim_num, unsigned short sheet_num)
{
	cr_anim_add(anim_data->sheets[anim_data->scripts[sheet_num]->sheet_id], anim_id, anim_data->scripts[anim_num]->tileset, anim_data->scripts[anim_num]->framecount, anim_data->scripts[anim_num]->speed, anim_data->scripts[anim_num]->type);
	anim_id->id = anim_num;
	anim_id->loop_frame = anim_data->scripts[anim_num]->loop_frame;
}

void cr_anim_transfer(CR_ANIM *target_anim, CR_ANIM *source_anim)
{
	target_anim->sprite = source_anim->sprite;
	target_anim->tileset = source_anim->tileset;

	target_anim->speed = source_anim->speed;
	target_anim->frame_count = source_anim->frame_count;
	target_anim->frame_current = source_anim->frame_current;
	target_anim->frame_timer = source_anim->frame_timer;
	target_anim->loop_frame = source_anim->loop_frame;

	target_anim->spr_x = source_anim->tileset[source_anim->frame_current].x;
	target_anim->spr_y = source_anim->tileset[source_anim->frame_current].y;
	target_anim->width = source_anim->tileset[source_anim->frame_current].width;
	target_anim->height = source_anim->tileset[source_anim->frame_current].height;
	target_anim->disp_x = source_anim->tileset[source_anim->frame_current].disp_x;
	target_anim->disp_y = source_anim->tileset[source_anim->frame_current].disp_y;

	target_anim->anim_type = source_anim->anim_type;

	target_anim->finished = source_anim->finished;
	target_anim->initialized = source_anim->initialized;
	target_anim->sequence = source_anim->sequence;
}

//Change the way an animation plays
void cr_anim_set_type(CR_ANIM *anim_id, CR_ANIM_TYPE animtype)
{
	anim_id->anim_type = animtype;
}

//Get/Set post frame
unsigned short cr_anim_get_frame(CR_ANIM *anim_id) { return anim_id->frame_current; }
void cr_anim_set_frame(CR_ANIM *anim_id, unsigned short frame)
{
	if (!anim_id->sequence) {
		if (frame >= anim_id->frame_count) {
			frame = anim_id->frame_count - 1;
		}
		anim_id->frame_current = frame;
		anim_id->frame_timer = 0;
		anim_id->spr_x = anim_id->tileset[frame].x;
		anim_id->spr_y = anim_id->tileset[frame].y;
		anim_id->width = anim_id->tileset[frame].width;
		anim_id->height = anim_id->tileset[frame].height;
		anim_id->disp_x = anim_id->tileset[frame].disp_x;
		anim_id->disp_y = anim_id->tileset[frame].disp_y;
	}
	else {
		anim_id->frame_current = frame;
	}

	anim_id->finished = false;
}

//Get/Set post speed
unsigned short cr_anim_get_speed(CR_ANIM *anim_id) { return anim_id->speed; }
void cr_anim_set_speed(CR_ANIM *anim_id, unsigned char anim_speed)
{
	anim_id->speed = anim_speed;
}

//Get/Set loop point
unsigned short cr_anim_get_loop_frame(CR_ANIM *anim_id) { return anim_id->loop_frame; }
void cr_anim_set_loop_frame(CR_ANIM *anim_id, unsigned short loopframe)
{
	anim_id->loop_frame = loopframe;
}

void cr_anim_play(CR_ANIM *anim_id) //Play an animation without drawing
{
	if (!crimson.pause_anims) {
		anim_id->frame_timer++;

		switch (anim_id->anim_type) {

		case CR_ANIM_LOOP_ONCE:
			if (anim_id->frame_timer >= anim_id->speed)
			{
				anim_id->frame_previous = anim_id->frame_current;
				anim_id->frame_current++;

				if (anim_id->frame_current >= anim_id->frame_count) {
					anim_id->frame_current = anim_id->frame_count - 1;
					anim_id->finished = true;
				}
				else {
					anim_id->finished = false;
				}

				anim_id->frame_timer = 0;
			}

			break;

		case CR_ANIM_LOOP:

			if (anim_id->frame_timer >= anim_id->speed)
			{
				anim_id->frame_current++;

				if (anim_id->frame_current >= anim_id->frame_count) {
					anim_id->frame_current = anim_id->loop_frame;
					anim_id->finished = true;
				}
				else {
					anim_id->finished = false;
				}

				anim_id->frame_timer = 0;
			}

			break;

		case CR_ANIM_REVERSE:

			if (anim_id->frame_timer >= anim_id->speed)
			{
				anim_id->frame_current--;

				if (anim_id->frame_current < 0) {
					if (anim_id->loop_frame != 0) {
						anim_id->frame_current = anim_id->loop_frame;
					}
					else {
						anim_id->frame_current = anim_id->frame_count - 1;
					}
					anim_id->finished = true;
				}
				else {
					anim_id->finished = false;
				}

				anim_id->frame_timer = 0;
			}

			break;

		case CR_ANIM_REVERSE_ONCE:

			if (!anim_id->finished) {
				if (anim_id->frame_current == 0) {
					anim_id->frame_current = anim_id->frame_count - 1;
				}
			}

			if (anim_id->frame_timer >= anim_id->speed)
			{
				anim_id->frame_current--;

				if (!anim_id->finished) {
					if (anim_id->frame_current < 0) {
						anim_id->frame_current = anim_id->frame_count - 1;
					}
					if (anim_id->frame_current == 0) {
						anim_id->finished = true;
					}
				}
				else {
					anim_id->frame_current = 0;
				}

				anim_id->frame_timer = 0;
			}

			break;

		case CR_ANIM_PINGPONG:

			if (anim_id->frame_timer >= anim_id->speed)
			{
				if (!anim_id->finished) {
					anim_id->frame_current++;

					if (anim_id->frame_current >= anim_id->frame_count) {
						anim_id->frame_current = anim_id->frame_count - 2;
						anim_id->finished = true;
					}

					anim_id->frame_timer = 0;
				}
				else {
					anim_id->frame_current--;
					if (anim_id->frame_current <= 0) {
						anim_id->frame_current = 0;
						anim_id->finished = false;
					}

					anim_id->frame_timer = 0;
				}
			}

			break;
		}

		if (!anim_id->sequence) {
			anim_id->spr_x = anim_id->tileset[anim_id->frame_current].x;
			anim_id->spr_y = anim_id->tileset[anim_id->frame_current].y;
			anim_id->width = anim_id->tileset[anim_id->frame_current].width;
			anim_id->height = anim_id->tileset[anim_id->frame_current].height;
			anim_id->disp_x = anim_id->tileset[anim_id->frame_current].disp_x;
			anim_id->disp_y = anim_id->tileset[anim_id->frame_current].disp_y;
		}
	}
}

unsigned short cr_count_tiles(const CR_TILE *tiles)
{
	unsigned short count = (sizeof(*tiles) / sizeof(CR_TILE));
	return count;
}

//BLEND FUNCTIONS
void cr_blending_enable(unsigned char blend_mode, unsigned char blend_amount)
{
	cr_drawing.blending_enabled = true;

	cr_drawing.blend_mode = blend_mode;
	cr_drawing.blend_amount = blend_amount;
}

void cr_blending_disable(void)
{
	cr_drawing.blending_enabled = false;

	cr_drawing.blend_mode = CR_BLEND_ALPHA;
	cr_drawing.blend_amount = 255;
}

void cr_counter_draw(CR_ANIM *anim_id, int pos_x, int pos_y, CR_ALIGNMENT alignment, unsigned char draw_layer, int draw_number)
{
	if (draw_number > 0) {
		int num, temp, factor = 1;

		num = draw_number;

		temp = num;
		while (temp) {
			temp = temp / 10;
			factor = factor * 10;
		}

		int numdigits = 0;
		if (alignment == CR_ALIGN_RIGHT) {
			temp = draw_number;
			while (temp != 0) {
				temp /= 10;
				numdigits++;
			}
		}

		int loop = 0;
		while (factor > 1) {
			factor = factor / 10;

			switch (alignment)
			{
			case CR_ALIGN_LEFT:
				cr_anim_set_frame(anim_id, num / factor);
				cr_anim_display(anim_id, pos_x + (loop * (anim_id->tileset[num / factor].width)), pos_y, CR_NO_FLIP, draw_layer);
				break;
			case CR_ALIGN_RIGHT:
				cr_anim_set_frame(anim_id, num / factor);
				cr_anim_display(anim_id, (pos_x + anim_id->tileset[num / factor].width) - ((numdigits - loop) * (anim_id->tileset[num / factor].width)), pos_y, CR_NO_FLIP, draw_layer);
				break;
			}

			num = num % factor;
			loop++;

		}
	}
	else {
		cr_anim_set_frame(anim_id, 0);
		switch (alignment)
		{
		case CR_ALIGN_LEFT:
			cr_anim_display(anim_id, pos_x /*- anim_id->tileset[0].width*/, pos_y, CR_NO_FLIP, draw_layer);
			break;
		case CR_ALIGN_RIGHT:
			cr_anim_display(anim_id, pos_x, pos_y, CR_NO_FLIP, draw_layer);
			break;
		}
	}
}

void cr_spritefont_draw(CR_ANIM *anim_id, int pos_x, int pos_y, CR_ALIGNMENT alignment, unsigned char draw_layer, const char *const draw_string, bool follow_cam)
{
	if (draw_string != NULL) {
		unsigned int string_size = strlen(draw_string);

		int drawpos = pos_x;
		switch (alignment)
		{
		case CR_ALIGN_LEFT:
			drawpos = pos_x;
			break;
		case CR_ALIGN_RIGHT:
			drawpos = pos_x - (anim_id->width * (string_size - 1));
			break;
		}

		for (unsigned int i = 0; i < string_size; i++) {
			cr_anim_set_frame(anim_id, /*cr_get_letter_number(*/draw_string[i]/*)*/);
			if (follow_cam) {
				cr_anim_display(anim_id, drawpos, pos_y, CR_NO_FLIP, draw_layer);
			}
			else {
				//cr_anim_display(anim_id, drawpos + cr_camera.cams[0]->pos.x, pos_y + cr_camera.cams[0]->pos.y, CR_NO_FLIP, draw_layer);
				cr_anim_display(anim_id, drawpos, pos_y, CR_NO_FLIP, draw_layer);
			}
			drawpos += anim_id->width;
		}
	}
}

unsigned int cr_get_letter_number(char product)
{

	int num = product - 65;
	return num;
}

void cr_lnscroll_wave(short degree, short root, short osc_mag, float osc_width, float osc_freq)
{
	for (int i = 0; i < 1024; i++) {
		cr_lnscroll[i] = root + ((osc_width) * cr_cosf((degree * osc_mag) - (i * osc_freq)));
	}
}