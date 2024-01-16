#include "crimson.h"
#pragma warning (disable : 6386)
//^ do something about this at some point lol

void cr_stage_goto(CR_STAGE *stage)
{
	if (!crimson.entry_set) {
		crimson.entry_stage = stage;
		crimson.entry_set = true;
	}

	bool skip_update = false;
	if (cr_stage.current != NULL) {
		//if (crimson.entry_set && stage != cr_stage.current) {
			cr_stage.current->unload();
	}
	else {
		skip_update = true;
	}

	//if (cr_stage.current != NULL) { cr_stage.previous = cr_stage.current; }
	//else { cr_stage.previous = stage; }
	cr_music_finished_hook(NULL);
	cr_stage.current = stage;
	stage->bgmode = 0;

	//Reset managers
	cr_camera_manager_reset();
	cr_object_manager_reset();
	cr_object_reload_globals();
	cr_sprite_manager_reset(false); // ???
	crimson.master_pause = false;
	crimson.pause_anims = false;
	cr_camera_replace(&cr_defaultcam, 0);

	for (unsigned char i = 0; i < CR_DEFAULT_INPUT_COUNT; i++) {
		cr_input_clear(&cr_input[i]);
	}

	if (!skip_update) {
		stage->load();
		stage->init();
		if (!crimson.skip_update) {
			stage->update();
		}
	}
	cr_clear_background();

	for (unsigned char i = 0; i < CR_DEFAULT_INPUT_COUNT; i++) {
		cr_input_clear(&cr_input[i]);
	}

	crimson.skip_update = false;
	//cr_channel_stop(-1);
}

void cr_stage_add(CR_STAGE *stage, const char *const name, void(*load)(void), void(*unload)(void), void(*init)(void), void(*update)(void))
{
	cr_stage.stage_count++;

	CR_STAGE **stgptrs = realloc(cr_stage.stage_pointers, cr_stage.stage_count * sizeof(CR_STAGE*));

	if (stgptrs != NULL) {
		cr_stage.stage_pointers = stgptrs;
	}

	cr_stage.stage_pointers[cr_stage.stage_count - 1] = stage;

	stage->name = malloc((strlen(name) + 1) * sizeof(char));
	if (stage->name != NULL) {
		strcpy(stage->name, name);
	}

	stage->load = load;
	stage->unload = unload;
	stage->init = init;
	stage->update = update;

	stage->id = cr_stage.stage_count;

	stage->bgmode = 0;

#if CR_DEBUG_STAGE
	printf("Stage added: %s\n", cr_stage.stage_pointers[cr_stage.stage_count - 1]->name);
#endif

	cr_stage.initialized = true;
}

void cr_stage_manager_reset(void)
{
	if (cr_stage.initialized) {
#if CR_DEBUG_STAGE
		printf("Resetting stage manager...\n");
#endif // CR_DEBUG_STAGE
		for (unsigned short i = 0; i < cr_stage.stage_count; i++) {
			if (cr_stage.stage_pointers[i]->loaded) {
				cr_stage.stage_pointers[i]->unload();
			}
#if CR_DEBUG_STAGE
			printf("Freeing %s...\n", cr_stage.stage_pointers[i]->name);
#endif // CR_DEBUG_STAGE
			free(cr_stage.stage_pointers[i]->name);
		}
		free(cr_stage.stage_pointers);
		cr_stage.initialized = false;
		cr_stage.stage_count = 0;
	}
}

void cr_stage_unload_all(void)
{
	for (unsigned short i = 0; i < cr_stage.stage_count; i++) {
		cr_stage.stage_pointers[i]->unload();
	}
}

void cr_stage_load_info(CR_STAGE *stage, const char *filepath)
{
	if (stage->info_loaded) {
		cr_stage_unload_info(stage);
	}

	stage->bgmode = 0;

	unsigned char *buffer;

	FILE *in_file = fopen(filepath, "rb");
	if (in_file == NULL) { perror("error"); return; }

	fseek(in_file, 0, SEEK_END);
	unsigned int file_size = ftell(in_file);
	fseek(in_file, 0, SEEK_SET);

	buffer = malloc(file_size * sizeof(char));

	if (in_file != NULL && buffer != NULL) {
		fread(buffer, file_size * sizeof(char), 1, in_file);

		stage->level_limit.up = buffer[3] | (buffer[2] << 8) | (buffer[1] << 16) | (buffer[0] << 24);
		stage->level_limit.down = buffer[7] | (buffer[6] << 8) | (buffer[5] << 16) | (buffer[4] << 24);
		stage->level_limit.left = buffer[11] | (buffer[10] << 8) | (buffer[9] << 16) | (buffer[8] << 24);
		stage->level_limit.right = buffer[15] | (buffer[14] << 8) | (buffer[13] << 16) | (buffer[12] << 24);

		stage->cam_limit.up = buffer[3] | (buffer[2] << 8) | (buffer[1] << 16) | (buffer[0] << 24);
		stage->cam_limit.down = buffer[7] | (buffer[6] << 8) | (buffer[5] << 16) | (buffer[4] << 24);
		stage->cam_limit.left = buffer[11] | (buffer[10] << 8) | (buffer[9] << 16) | (buffer[8] << 24);
		stage->cam_limit.right = buffer[15] | (buffer[14] << 8) | (buffer[13] << 16) | (buffer[12] << 24);

		int file_pos = 0x20;

		unsigned char filename_length;
		unsigned short angle_count;

		// Storing the directory path to use to access each file relative to the stage file
		int dir_len = strlen(strrchr(filepath, '/'));
		char *dir_path = calloc(strlen(filepath) + 1 - dir_len + 1, sizeof(char));
		strncpy(dir_path, filepath, strlen(filepath) + 1 - dir_len);

		// Get number of blocksets to get
		stage->blockset_count = buffer[file_pos++];

		if (stage->blockset_count > 0) {
			stage->blocksets = calloc((int)stage->blockset_count, sizeof(CR_BLOCKSET));  // Allocate enough memory to store the blocksets

			// For each blockset
			for (unsigned int i = 0; i < stage->blockset_count; i++) {
				filename_length = buffer[file_pos++];   // Get the blockset's filename length

				char *path = calloc(filename_length + 1, sizeof(char)); // Allocate memory to store the blockset filename
				memcpy_s(path, filename_length + 1, buffer + file_pos, filename_length);    // Copy the blockset filename to the allocated memory
				file_pos += filename_length;    // Advance the buffer
				size_t pathsize = (size_t)(strlen(path) + strlen(dir_path));
				char *fullpath = calloc(pathsize + 1, sizeof(char));
				strcpy(fullpath, dir_path);
				strcat(fullpath, path);
				free(path);

				stage->blocksets[i].loaded = false; //Make sure there's no garbage data in here
				cr_stage_load_blockset(fullpath, &stage->blocksets[i]);
				free(fullpath);
			}
		}

		// Get number of anglemaps
		stage->anglemap_count = buffer[file_pos++];

		char *fullpath = NULL;

		if (stage->anglemap_count > 0) {
			stage->anglemaps = calloc((int)stage->anglemap_count, sizeof(CR_ANGLEMAP));  // Allocate enough memory to store the anglemaps

			// For each anglemap
			for (unsigned int i = 0; i < stage->anglemap_count; i++) {
				angle_count = buffer[file_pos + 1] | (buffer[file_pos] << 8);   // Get the anglemap's angle total_count
				file_pos += 2;
				filename_length = buffer[file_pos++];   // Get the anglemap's filename length

				char *path = calloc(filename_length + 1, sizeof(char)); // Allocate memory to store the anglemap filename
				if (path != NULL) {
					memcpy(path, buffer + file_pos, filename_length);   // Copy the anglepmap filename to the allocated memory
					file_pos += filename_length;    // Advance the buffer

					size_t size = (size_t)(strlen(path) + strlen(dir_path) + 1);
					fullpath = calloc(size, sizeof(char));
					if (fullpath != NULL) {
						strcpy(fullpath, dir_path);
						strcat(fullpath, path);
					}
					free(path);

					stage->anglemaps[i].loaded = false;
					cr_stage_load_anglemap(fullpath, &stage->anglemaps[i]);
					free(fullpath);
				}
			}
		}

		// Get number of tilemaps
		stage->tilemap_count = buffer[file_pos++];

		if (stage->tilemap_count > 0) {
			stage->tilemaps = calloc(stage->tilemap_count, sizeof(CR_TILE_MAP));     // Allocate enough memory to store the tilemaps

			// For each tilemap
			for (unsigned int i = 0; i < stage->tilemap_count; i++) {

				filename_length = buffer[file_pos++];   // Get the tilemap's filename length

				char *path = calloc(filename_length + 1, sizeof(char)); // Allocate memory to store the tilemap filename
				if (path != NULL) {
					memcpy(path, buffer + file_pos, filename_length);   // Copy the tilemap filename to the allocated memory
					file_pos += filename_length;    // Advance the buffer

					size_t size = (size_t)(strlen(path) + strlen(dir_path) + 1);
					fullpath = calloc(size, sizeof(char));
					if (fullpath != NULL && path != NULL) {
						strcpy(fullpath, dir_path);
						strcat(fullpath, path);
						free(path);

						if (stage->tilemaps != NULL) {
							stage->tilemaps[i].loaded = false;
							cr_stage_load_tilemap(fullpath, &stage->tilemaps[i]);
							stage->tilemaps[i].layer = buffer[file_pos++];
						}

						free(fullpath);
					}
				}
			}
		}

		// Get number of collision maps
		stage->colmap_count = buffer[file_pos++];

		if (stage->colmap_count > 0) {
			stage->colmaps = calloc(stage->colmap_count, sizeof(CR_TILE_MAP));   // Allocate enough memory to store the colmaps

			unsigned char *priority = malloc(stage->tilemap_count * sizeof(unsigned char));
			if (priority != NULL) {
				for (int i = 0; i < stage->tilemap_count; i++) {
					priority[i] = buffer[file_pos++];
				}

				// For each colmap
				for (int i = 0; i < stage->colmap_count; i++) {
					int v = priority[i];
					angle_count = buffer[file_pos + 1] | (buffer[file_pos] << 8);   // Get the colmap's filename length
					filename_length = buffer[file_pos++];   // Get the colmap's filename length

					char *path = calloc(filename_length + 1, sizeof(char)); // Allocate memory to store the colmap filename
					memcpy_s(path, filename_length + 1, buffer + file_pos, filename_length);    // Copy the colmap filename to the allocated memory
					file_pos += filename_length;    // Advance the buffer

					char *fullpath = calloc(strlen(path) + strlen(dir_path) + 1, sizeof(char));
					strcpy(fullpath, dir_path);
					strcat(fullpath, path);
					free(path);

					stage->colmaps[v].loaded = false;
					cr_stage_load_collision(fullpath, &stage->colmaps[v]);

					//stage->colmaps[v].layer = buffer[file_pos++];

					free(fullpath);
				}

				free(priority);
			}
		}

		//Set defaults;
		stage->blockset_id = 0;
		stage->anglemap_id = 0;

		stage->info_loaded = true;

		//CLOSE OUT
		fclose(in_file);
		free(dir_path);
		free(buffer);
	}
}

void cr_stage_unload_info(CR_STAGE *stage)
{
	if (stage->loaded) {
		for (int i = 0; i < stage->blockset_count; i++) {
			cr_stage_unload_blockset(&stage->blocksets[i]);
		}
		for (int i = 0; i < stage->anglemap_count; i++) {
			cr_stage_unload_anglemap(&stage->anglemaps[i]);
		}
		for (int i = 0; i < stage->tilemap_count; i++) {
			cr_stage_unload_tilemap(&stage->tilemaps[i]);
		}
		for (int i = 0; i < stage->colmap_count; i++) {
			cr_stage_unload_collision(&stage->colmaps[i]);
		}

		free(stage->blocksets);
		free(stage->anglemaps);
		free(stage->tilemaps);
		free(stage->colmaps);

		stage->info_loaded = false;
	}
}


void cr_stage_load_tilemap(const char *filepath, CR_TILE_MAP *map)
{
	if (map->loaded) {
		cr_stage_unload_tilemap(map);
	}
	unsigned char *buffer;

	FILE *in_file = fopen(filepath, "rb");
	if (in_file == NULL) { return; }

	fseek(in_file, 0, SEEK_END);
	unsigned int file_size = ftell(in_file);
	fseek(in_file, 0, SEEK_SET);

	size_t size = file_size;
	buffer = malloc(size);
	fread(buffer, size, 1, in_file);

	map->sheet_width = buffer[3] | (buffer[2] << 8) | (buffer[1] << 16) | (buffer[0] << 24);
	map->sheet_height = buffer[7] | (buffer[6] << 8) | (buffer[5] << 16) | (buffer[4] << 24);

	map->tile_width = buffer[11] | (buffer[10] << 8) | (buffer[9] << 16) | (buffer[8] << 24);
	map->tile_height = buffer[15] | (buffer[14] << 8) | (buffer[13] << 16) | (buffer[12] << 24);

	map->chunk_width = buffer[19] | (buffer[18] << 8) | (buffer[17] << 16) | (buffer[16] << 24);
	map->chunk_height = buffer[23] | (buffer[22] << 8) | (buffer[21] << 16) | (buffer[20] << 24);

	map->tilemap_width = buffer[27] | (buffer[26] << 8) | (buffer[25] << 25) | (buffer[24] << 24);
	map->tilemap_height = buffer[31] | (buffer[30] << 8) | (buffer[29] << 16) | (buffer[28] << 24);

	map->chunk_count = buffer[33] | (buffer[32] << 8);

	if (map->chunk_count == 1) {
		map->draw_mode = CR_TILE_DRAW_ONECHUNK;
	}
	else {
		map->draw_mode = CR_TILE_DRAW_CHUNKS;
	}

	map->chunks = malloc(map->chunk_count * sizeof(unsigned int*));
	for (unsigned int i = 0; i < map->chunk_count; i++) {
		map->chunks[i] = malloc((map->chunk_width * map->chunk_height) * sizeof(unsigned int*));
	}

	map->layout = malloc(map->tilemap_height * sizeof(unsigned int*));
	for (unsigned int i = 0; i < map->tilemap_height; i++) {
		map->layout[i] = malloc(map->tilemap_width * sizeof(unsigned int*));
	}

	unsigned int bufferpos = 34;
	unsigned int chunk_size = (map->chunk_width * map->chunk_height);
	unsigned int chunk_range = chunk_size * 2;
	for (unsigned int i = 0; i < map->chunk_count; i++) {
		for (unsigned int j = 0; j < chunk_size; j++) {
			map->chunks[i][j] = buffer[(bufferpos + 1) + (chunk_range * i) + (j * 2)] | (buffer[(bufferpos)+(chunk_range * i) + (j * 2)] << 8);
		}
	}

	bufferpos = 34 + ((((map->chunk_width * map->chunk_height) * map->chunk_count) * 2) + 0);
	for (unsigned int i = 0; i < map->tilemap_height; i++) {
		for (unsigned int j = 0; j < map->tilemap_width; j++) {
			if (map->layout[i] != NULL) {
				map->layout[i][j] = buffer[(bufferpos + 1) + ((map->tilemap_width * 2) * i) + (j * 2)] | (buffer[(bufferpos)+((map->tilemap_width * 2) * i) + (j * 2)] << 8);
			}
		}
	}

	fclose(in_file);
	free(buffer);

	map->loaded = true;
}

void cr_stage_unload_tilemap(CR_TILE_MAP *map)
{
	if (map->loaded) {
		for (unsigned int i = 0; i < map->chunk_count; i++) {
			free(map->chunks[i]);
		}
		free(map->chunks);

		for (unsigned int i = 0; i < map->tilemap_height; i++) {
			free(map->layout[i]);
		}
		free(map->layout);
	}
	map->loaded = false;
}

void cr_stage_load_collision(const char *filepath, CR_COLLISION_MAP *map)
{
	if (map->loaded) {
		cr_stage_unload_collision(map);
	}
	unsigned char *buffer;

	FILE *in_file = fopen(filepath, "rb");
	if (in_file == NULL) { perror("error"); return; }

	fseek(in_file, 0, SEEK_END);
	unsigned int file_size = ftell(in_file);
	fseek(in_file, 0, SEEK_SET);

	size_t size = file_size;
	buffer = malloc(size);
	fread(buffer, size, 1, in_file);

	map->block_width = buffer[3] | (buffer[2] << 8) | (buffer[1] << 16) | (buffer[0] << 24);
	map->block_height = buffer[7] | (buffer[6] << 8) | (buffer[5] << 16) | (buffer[4] << 24);

	map->metablock_width = buffer[11] | (buffer[10] << 8) | (buffer[9] << 16) | (buffer[8] << 24);
	map->metablock_height = buffer[15] | (buffer[14] << 8) | (buffer[13] << 16) | (buffer[12] << 24);

	map->level_width = buffer[19] | (buffer[18] << 8) | (buffer[17] << 16) | (buffer[16] << 24);
	map->level_height = buffer[23] | (buffer[22] << 8) | (buffer[21] << 16) | (buffer[20] << 24);

	map->metablock_count = buffer[25] | (buffer[24] << 8);

	map->metablocks = malloc(map->metablock_count * sizeof(unsigned int*));
	for (unsigned int i = 0; i < map->metablock_count; i++) {
		map->metablocks[i] = malloc((map->metablock_width * map->metablock_height) * sizeof(unsigned int*));
	}

	map->layout = malloc((map->level_height) * sizeof(unsigned int*));
	for (unsigned int i = 0; i < map->level_height; i++) {
		map->layout[i] = malloc(map->level_width * sizeof(unsigned int*));
	}

	unsigned int bufferpos = 26;
	unsigned int metablock_size = (map->metablock_width * map->metablock_height);
	unsigned int metablock_range = metablock_size * 2;
	for (unsigned int i = 0; i < map->metablock_count; i++) {
		for (unsigned int j = 0; j < metablock_size; j++) {
			map->metablocks[i][j] = buffer[(bufferpos + 1) + (metablock_range * i) + (j * 2)] | (buffer[(bufferpos)+(metablock_range * i) + (j * 2)] << 8);
		}
	}

	bufferpos = 26 + ((((map->metablock_width * map->metablock_height) * map->metablock_count) * 2) + 0);
	unsigned int level_row = map->level_width * 2;
	unsigned int level_column = map->level_height * 2;
	for (unsigned int i = 0; i < map->level_height; i++) {
		for (unsigned int j = 0; j < map->level_width; j++) {
			map->layout[i][j] = buffer[(bufferpos + 1) + ((map->level_width * 2) * i) + (j * 2)] | (buffer[(bufferpos)+((map->level_width * 2) * i) + (j * 2)] << 8);
		}
	}

	fclose(in_file);
	free(buffer);

	map->loaded = true;
}
void cr_stage_unload_collision(CR_COLLISION_MAP *map)
{
	if (map->loaded) {
		for (unsigned int i = 0; i < map->metablock_count; i++) {
			free(map->metablocks[i]);
		}
		free(map->metablocks);

		for (unsigned int i = 0; i < map->level_height; i++) {
			free(map->layout[i]);
		}
		free(map->layout);
	}
	map->loaded = false;
}

void cr_stage_load_blockset(const char *filepath, CR_BLOCKSET *blocks)
{
	if (blocks->loaded) {
		cr_stage_unload_blockset(blocks);
	}

	unsigned char *buffer;

	FILE *in_file = fopen(filepath, "rb");

	fseek(in_file, 0, SEEK_END);
	unsigned int file_size = ftell(in_file);
	fseek(in_file, 0, SEEK_SET);

	size_t size = file_size;
	buffer = malloc(size);
	fread(buffer, size, 1, in_file);

	blocks->width = buffer[3] | (buffer[2] << 8) | (buffer[1] << 16) | (buffer[0] << 24);
	blocks->height = buffer[7] | (buffer[6] << 8) | (buffer[5] << 16) | (buffer[4] << 24);
	blocks->count = buffer[11] | (buffer[10] << 8) | (buffer[9] << 16) | (buffer[8] << 24);
	blocks->count++; // Correct total_count so the thing doesn't crash

	// Allocate memory for both the normal and rotated heightmasks
	//int **ptr = malloc(blocks->total_count * sizeof(signed int*));
	//int **ptrs = malloc(1 * sizeof(int));
	blocks->heightmasks = malloc(blocks->count * sizeof(signed int*));
	blocks->rot_heightmasks = malloc(blocks->count * sizeof(signed int*));
	//blocks->rot_heightmasks = malloc(blocks->total_count * sizeof(unsigned int*));
	for (unsigned int i = 0; i < blocks->count; i++) {
		blocks->heightmasks[i] = malloc((blocks->width * blocks->height) * sizeof(unsigned int*));
		blocks->rot_heightmasks[i] = malloc((blocks->width * blocks->height) * sizeof(unsigned int*));
	}
	//free(blocks->heightmasks);

	unsigned int bufferpos = 0xC;
	unsigned int block_size = (blocks->width * blocks->height);
	unsigned int block_range = blocks->width * 2;

	for (unsigned int i = 0; i < blocks->count; i++) {
		for (unsigned int j = 0; j < blocks->width; j++) {
			blocks->heightmasks[i][j] = buffer[(bufferpos + 1) + (block_range * i) + (j * 2)] | (buffer[(bufferpos)+(block_range * i) + (j * 2)] << 8);
		}
	}

	// Completely clear the rotated heightmask array
	for (unsigned int i = 0; i < blocks->count; i++) {
		for (unsigned int j = 0; j < blocks->width; j++) {
			blocks->rot_heightmasks[i][j] = 0;
		}
	}

	// Convert heightmasks into rotated heightmasks
	for (unsigned int i = 0; i < blocks->count; i++) {
		// What this does is subtract a threshold value from either side of the heightmask to see if the result is less than 0
		// If it is, we enter an according mode for either making a negative or positive heightmask
		// If both sides have a positive result, the rotated heightmask must be maximum height
		// !! AS A GENERAL RULE OF THUMB FOR NORMAL HEIGHTMASKS, NEGATIVE AND POSITVE VALUES WILL NEVER OCCUPY THE SAME SPACE !!
		// !! This is because it will lead to heightmasks that can't rotate properly !!

		// Check before conversion to see if the heightmask uses positive of negative values
		bool positive = false;
		for (unsigned int m = 0; m < blocks->width; m++) {
			if (blocks->heightmasks[i][m] < 0) {
				positive = false;
				break;					// If any negative values are found, break away right away
										// (the reason positve values are continued to be checked is because 0x10 may occupy a negative heightmask)
			}
			else {
				positive = true;
			}
		}

		for (signed int h = 0; h < blocks->height; h++) {
			// Positive heightmask mode
			if (positive) {
				if ((blocks->heightmasks[i][0] - (h + 1)) < 0) {
					int new_height = 0;
					for (signed int w = blocks->width; w > 0; w--) {
						if ((blocks->heightmasks[i][w] - (h + 1)) >= 0) {
							new_height++;
						}
					}
					blocks->rot_heightmasks[i][blocks->height - 1 - h] = new_height;
				}

				else if ((blocks->heightmasks[i][blocks->width - 1] - (h + 1)) < 0) {
					int new_height = 0;
					for (signed  int w = 0; w < blocks->width; w++) {
						if ((blocks->heightmasks[i][w] - (h + 1)) >= 0) {
							new_height--;
						}
					}
					blocks->rot_heightmasks[i][blocks->height - 1 - h] = new_height;
				}

				else {
					blocks->rot_heightmasks[i][blocks->height - 1 - h] = 0x10;
				}
			}

			// Negative heightmask mode
			else {
				int check1 = blocks->heightmasks[i][0];
				int check2 = blocks->heightmasks[i][blocks->width - 1];
				if (check1 == blocks->height) {
					check1 = -check1;
				}
				if (check2 == blocks->height) {
					check2 = -check2;
				}

				if ((check1 + (h + 1)) > 0) {
					int new_height = 0;
					for (signed int w = blocks->width; w > 0; w--) {
						int check = blocks->heightmasks[i][w];
						if (check == blocks->height) {
							check = -check;
						}
						if ((check + (h + 1)) <= 0) {
							new_height++;
						}
					}
					blocks->rot_heightmasks[i][h] = new_height;
				}

				else if ((check2 + (h + 1)) > 0) {
					int new_height = 0;
					for (signed int w = 0; w < blocks->width; w++) {
						int check = blocks->heightmasks[i][w];
						if (check == blocks->height) {
							check = -check;
						}
						if ((check + (h + 1)) <= 0) {
							new_height--;
						}
					}
					blocks->rot_heightmasks[i][h] = new_height;
				}

				else {
					blocks->rot_heightmasks[i][h] = 0x10;
				}
			}
		}
	}

	fclose(in_file);
	free(buffer);

	blocks->loaded = true;
}
void cr_stage_unload_blockset(CR_BLOCKSET *blocks)
{
	if (blocks->loaded) {
		for (unsigned int i = 0; i < blocks->count; i++) {
			free(blocks->heightmasks[i]);
			free(blocks->rot_heightmasks[i]);
		}
		free(blocks->heightmasks);
		free(blocks->rot_heightmasks);
	}
	blocks->loaded = false;
}

void cr_stage_load_anglemap(const char *filepath, CR_ANGLEMAP *angles)
{
	if (angles->loaded) {
		cr_stage_unload_anglemap(angles);
	}

	unsigned char *buffer;

	FILE *in_file = fopen(filepath, "rb");
	if (in_file == NULL) { return; }

	fseek(in_file, 0, SEEK_END);
	unsigned int file_size = ftell(in_file);
	fseek(in_file, 0, SEEK_SET);

	size_t size = file_size;
	buffer = malloc(size);

	if (in_file != NULL && buffer != NULL) {
		fread(buffer, size, 1, in_file);

		angles->count = file_size;
		angles->angles = malloc(file_size);

		if (angles->angles != NULL) {
			for (unsigned short i = 0; i < file_size; i++) {
				angles->angles[i] = buffer[i];
			}
		}

		fclose(in_file);
		free(buffer);

		angles->loaded = true;
	}
}

void cr_stage_unload_anglemap(CR_ANGLEMAP *angles)
{
	if (angles->loaded) {
		free(angles->angles);
		angles->count = 0;
	}

	angles->loaded = false;
}

void cr_tilemap_draw(CR_TEXTURE *tileset, CR_TILE_MAP *map, unsigned char draw_layer)
{
	int prev_x = 0;
	int prev_y = 0;

	for (int screen = 0; screen <= cr_drawing.active_screens; screen++) {
		unsigned char delegate = screen + 1;
		float camera_x = cr_camera.cams[screen]->pos.x;
		float camera_y = cr_camera.cams[screen]->pos.y;

		signed int block_x_id = 0;
		signed int block_y_id = 0;

		if (cr_camera.cams[screen]->unbound) {
			while (camera_x >= ((map->chunk_width * map->tile_width) * map->tilemap_width)) {
				camera_x -= ((map->chunk_width * map->tile_width) * map->tilemap_width);
			}
			while (camera_y >= ((map->chunk_height * map->tile_height) * map->tilemap_height)) {
				camera_y -= ((map->chunk_height * map->tile_height) * map->tilemap_height);
			}

			while (cr_camera.cams[screen]->pos.x < ((map->chunk_width * map->tile_width) * map->tilemap_width)) {
				cr_camera.cams[screen]->pos.x += ((map->chunk_width * map->tile_width) * map->tilemap_width);
			}
			while (cr_camera.cams[screen]->pos.x >= ((map->chunk_width * map->tile_width) * map->tilemap_width)) {
				cr_camera.cams[screen]->pos.x -= ((map->chunk_width * map->tile_width) * map->tilemap_width);
			}

			while (cr_camera.cams[screen]->pos.y < ((map->chunk_height * map->tile_height) * map->tilemap_height)) {
				cr_camera.cams[screen]->pos.y += ((map->chunk_height * map->tile_height) * map->tilemap_height);
			}
			while (cr_camera.cams[screen]->pos.y >= ((map->chunk_height * map->tile_height) * map->tilemap_height)) {
				cr_camera.cams[screen]->pos.y -= ((map->chunk_height * map->tile_height) * map->tilemap_height);
			}
		}
		else {
			if (camera_x > ((map->chunk_width * map->tile_width) * map->tilemap_width)) {
				camera_x = ((map->chunk_width * map->tile_width) * map->tilemap_width);
			}

			if (camera_y > ((map->chunk_height * map->tile_height) * map->tilemap_height)) {
				camera_y = ((map->chunk_height * map->tile_height) * map->tilemap_height);
			}

			if (cr_camera.cams[screen]->pos.x < 0) {
				cr_camera.cams[screen]->pos.x = 0;
			}

			if (cr_camera.cams[screen]->pos.y < 0) {
				cr_camera.cams[screen]->pos.y = 0;
			}

			if (cr_camera.cams[screen]->pos.x > ((map->chunk_width * map->tile_width) * map->tilemap_width) - crimson.game_width) {
				cr_camera.cams[screen]->pos.x = ((map->chunk_width * map->tile_width) * map->tilemap_width) - crimson.game_width;
			}

			if (cr_camera.cams[screen]->pos.y > ((map->chunk_height * map->tile_height) * map->tilemap_height) - crimson.game_height) {
				cr_camera.cams[screen]->pos.y = ((map->chunk_height * map->tile_height) * map->tilemap_height) - crimson.game_height;
			}
		}

		//cr_stage_chunk_draw(tileset, map, 0, 0, draw_layer);

		switch (map->draw_mode) {
		case CR_TILE_DRAW_CHUNKS:

			for (unsigned int i = 0; i < ((crimson.game_height / (map->chunk_height * map->tile_height)) + 2); i++) {
				//if (i == ((crimson.game_height / (map->chunk_height * map->tile_height)) + 1)) {
				//	prev_y = ((crimson.game_height / (map->chunk_height * map->tile_height)) + 2);
				//}
				for (unsigned int j = 0; j < ((crimson.game_width / (map->chunk_width * map->tile_width)) + 2); j++) {
					//if (j == ((crimson.game_width / (map->chunk_width * map->tile_width)) + 1)) {
					//	prev_x = ((crimson.game_height / (map->chunk_height * map->tile_height)) + 2);
					//}

					int chunk_x_id = ((int)cr_camera.cams[screen]->pos.x / (map->chunk_width * map->tile_width)) + j;
					int chunk_y_id = ((int)cr_camera.cams[screen]->pos.y / (map->chunk_height * map->tile_height)) + i;
					// chunk_x_id = (camera_x / (map->chunk_width * map->tile_width)) + j;
					// chunk_y_id = (camera_y / (map->chunk_height * map->tile_height)) + i;

					if ((unsigned int)chunk_x_id > map->tilemap_width) {
						chunk_x_id = map->tilemap_width;
					}
					if ((unsigned int)chunk_y_id > map->tilemap_height) {
						chunk_y_id -= map->tilemap_height;
					}

					//if (chunk_x_id >= map->tilemap_width) {
					//	chunk_x_id = 1;
					//}
					//if (chunk_y_id >= map->tilemap_height) {
					//	chunk_y_id = 0;
					//}

					cr_tilemap_chunk_draw(tileset, map, chunk_x_id, chunk_y_id, delegate, draw_layer);
				}
			}

			break;

		case CR_TILE_DRAW_ONECHUNK:

			block_x_id = ((int)cr_camera.cams[screen]->pos.x / (map->tile_width));
			block_y_id = ((int)cr_camera.cams[screen]->pos.y / (map->tile_height));

			cr_tilemap_chunk_draw(tileset, map, block_x_id, block_y_id, delegate, draw_layer);

			break;
		}
	}
}

void cr_tilemap_chunk_draw(CR_TEXTURE *tileset, CR_TILE_MAP *map, int layout_x, int layout_y, unsigned char screen_delegate, unsigned char draw_layer)
{

	if (map->loaded) {

		int draw_x = layout_x * (map->chunk_width * map->tile_width);
		int draw_y = layout_y * (map->chunk_height * map->tile_height);

		int mask = ((1 << 14) - 1) << 0;

		bool flip_h = 0;
		bool flip_v = 0;

		int chunk_id = 0;

		if (map->draw_mode == CR_TILE_DRAW_CHUNKS) {
			flip_h = (map->layout[layout_y][layout_x] >> 15) & 1;
			flip_v = (map->layout[layout_y][layout_x] >> 14) & 1;

			chunk_id = map->layout[layout_y][layout_x] & mask;
		}
		else {
			flip_h = (map->layout[0][0] >> 15) & 1;
			flip_v = (map->layout[0][0] >> 14) & 1;

			chunk_id = map->layout[0][0] & mask;
		}

		switch (map->draw_mode) {

		case CR_TILE_DRAW_CHUNKS:

			if (!flip_h && !flip_v) {
				for (unsigned int i = 0; i < (map->chunk_width * map->chunk_height); i++) {
					bool flip_x = (map->chunks[chunk_id][i] >> 15) & 1;
					bool flip_y = (map->chunks[chunk_id][i] >> 14) & 1;

					mask = ((1 << 14) - 1) << 0;
					int chunk_final = map->chunks[chunk_id][i] & mask;

					int chunk_id_x = (chunk_final % map->sheet_width) * map->tile_width;
					int chunk_id_y = (chunk_final / map->sheet_width) * map->tile_height;

					if (chunk_id_x == 0 && chunk_id_y == 0) {
						//do nothing!
					}
					else {
						cr_screen_delegate(screen_delegate);
						cr_section_draw(tileset, (float)(draw_x + (map->tile_width * (i % map->chunk_width))), (float)(draw_y + (map->tile_height * (i / map->chunk_height))), chunk_id_x, chunk_id_y, map->tile_width, map->tile_height, (flip_x | (flip_y * 2)), draw_layer);
						cr_screen_undelegate();
					}
				}
			}

			if (flip_h && !flip_v) {
				for (unsigned int i = 0; i < (map->chunk_width * map->chunk_height); i++) {

					int inverse = ((map->chunk_width * map->chunk_height - 1)) - i;
					bool flip_x = (map->chunks[chunk_id][inverse] >> 15) & 1;
					bool flip_y = (map->chunks[chunk_id][inverse] >> 14) & 1;

					flip_x = !flip_x;

					mask = ((1 << 14) - 1) << 0;
					int chunk_final = map->chunks[chunk_id][inverse] & mask;

					int chunk_id_x = (chunk_final % map->sheet_width) * map->tile_width;
					int chunk_id_y = (chunk_final / map->sheet_width) * map->tile_height;

					if (chunk_id_x == 0 && chunk_id_y == 0) {

					}
					else {
						cr_screen_delegate(screen_delegate);
						cr_section_draw(tileset, (float)(draw_x + (map->tile_width * (i % map->chunk_width))), (float)(draw_y + (map->tile_height * (inverse / map->chunk_height))), chunk_id_x, chunk_id_y, map->tile_width, map->tile_height, (flip_x | (flip_y * 2)), draw_layer);
						cr_screen_undelegate();
					}
				}
			}

			if (!flip_h && flip_v) {
				for (unsigned int i = 0; i < (map->chunk_width * map->chunk_height); i++) {

					int inverse = ((map->chunk_width * map->chunk_height - 1)) - i;
					bool flip_x = (map->chunks[chunk_id][inverse] >> 15) & 1;
					bool flip_y = (map->chunks[chunk_id][inverse] >> 14) & 1;

					flip_y = !flip_y;

					mask = ((1 << 14) - 1) << 0;
					int chunk_final = map->chunks[chunk_id][inverse] & mask;

					int chunk_id_x = (chunk_final % map->sheet_width) * map->tile_width;
					int chunk_id_y = (chunk_final / map->sheet_width) * map->tile_height;

					if (chunk_id_x == 0 && chunk_id_y == 0) {

					}
					else {
						cr_screen_delegate(screen_delegate);
						cr_section_draw(tileset, (float)(draw_x + (map->tile_width * (inverse % map->chunk_width))), (float)(draw_y + (map->tile_height * (i / map->chunk_height))), chunk_id_x, chunk_id_y, map->tile_width, map->tile_height, (flip_x | (flip_y * 2)), draw_layer);
						cr_screen_undelegate();
					}
				}
			}

			if (flip_h && flip_v) {
				for (unsigned int i = 0; i < (map->chunk_width * map->chunk_height); i++) {

					int inverse = ((map->chunk_width * map->chunk_height - 1)) - i;
					bool flip_x = (map->chunks[chunk_id][inverse] >> 15) & 1;
					bool flip_y = (map->chunks[chunk_id][inverse] >> 14) & 1;

					flip_x = !flip_x;
					flip_y = !flip_y;

					mask = ((1 << 14) - 1) << 0;
					int chunk_final = map->chunks[chunk_id][inverse] & mask;

					int chunk_id_x = (chunk_final % map->sheet_width) * map->tile_width;
					int chunk_id_y = (chunk_final / map->sheet_width) * map->tile_height;

					if (chunk_id_x == 0 && chunk_id_y == 0) {

					}
					else {
						cr_screen_delegate(screen_delegate);
						cr_section_draw(tileset, (float)(draw_x + (map->tile_width * (i % map->chunk_width))), (float)(draw_y + (map->tile_height * (i / map->chunk_height))), chunk_id_x, chunk_id_y, map->tile_width, map->tile_height, (flip_x | (flip_y * 2)), draw_layer);
						cr_screen_undelegate();
					}
				}
			}
			break;

		case CR_TILE_DRAW_ONECHUNK:

			draw_x = layout_x;
			draw_y = layout_y;

			for (unsigned int h = draw_y; h < draw_y + (crimson.game_height / map->tile_height) + 2; h++) {
				for (unsigned int w = draw_x; w < draw_x + (crimson.game_width / map->tile_width) + 2; w++) {

					int chunk_n = (h * (map->chunk_width)) + w;

					bool flip_x = (map->chunks[0][chunk_n] >> 15) & 1;
					bool flip_y = (map->chunks[0][chunk_n] >> 14) & 1;

					mask = ((1 << 14) - 1) << 0;
					int chunk_final = map->chunks[chunk_id][chunk_n] & mask;

					int chunk_id_x = (chunk_final % map->sheet_width) * map->tile_width;
					int chunk_id_y = (chunk_final / map->sheet_width) * map->tile_height;

					if (chunk_id_x == 0 && chunk_id_y == 0) {

					}
					else {
						cr_screen_delegate(screen_delegate);
						cr_section_draw(tileset, (float)(w * map->tile_width), (float)(h * map->tile_height), chunk_id_x, chunk_id_y, map->tile_width, map->tile_height, (flip_x | (flip_y * 2)), draw_layer);
						cr_screen_undelegate();
					}
				}
			}

			break;
		}
		//cr_screen_delegate(0);
		//cr_screen_undelegate();
	}
}

void cr_stage_draw(CR_TEXTURE *tileset, CR_STAGE *stage)
{
	for (unsigned int i = 0; i < stage->tilemap_count; i++) {
		cr_tilemap_draw(tileset, &stage->tilemaps[i], stage->tilemaps[i].layer);
	}
}
