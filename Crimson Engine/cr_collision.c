#include "crimson.h"

bool cr_bbox_check(CR_BOX_2D *bbox1, CR_BOX_2D *bbox2)
{
	//First check on X axis
	if ((bbox1->pos.x + bbox1->size.x) > bbox2->pos.x) {
		//First check on Y axis
		if ((bbox1->pos.y + bbox1->size.y) > bbox2->pos.y) {
			//Second check on X axis
			if (bbox1->pos.x < (bbox2->pos.x + bbox2->size.x)) {
				//Second check on Y axis
				if (bbox1->pos.y < (bbox2->pos.y + bbox2->size.y)) {
					return true;
				}
			}
		}
	}
	return false;
}

bool cr_colpnt_findfloor(CR_STAGE *stage, unsigned char col_priority, CR_COLLISION_POINT *point, CR_SOLIDITY solid_test, int range, signed int range_disp, bool rev_x, bool rev_y)
{
	bool col = cr_colpnt_findfloor_sep(&stage->colmaps[col_priority], &stage->blocksets[stage->blockset_id], &stage->anglemaps[stage->anglemap_id], point, solid_test, range, range_disp, rev_x, rev_y);
	return col;
}

bool cr_colpnt_findwall(CR_STAGE *stage, unsigned char col_priority, CR_COLLISION_POINT *point, CR_SOLIDITY solid_test, int range, signed int range_disp, bool rev_x, bool rev_y)
{
	bool col = cr_colpnt_findwall_sep(&stage->colmaps[col_priority], &stage->blocksets[stage->blockset_id], &stage->anglemaps[stage->anglemap_id], point, solid_test, range, range_disp, rev_x, rev_y);
	return col;
}

// Function for getting distance from a floor or ceiling
// Outputs information about the collision of the current floor block, as well as the distance from the floor or ceiling relative to the collision point
bool cr_colpnt_findfloor_sep(CR_COLLISION_MAP *map, CR_BLOCKSET *blocks, CR_ANGLEMAP *anglemap, CR_COLLISION_POINT *point, CR_SOLIDITY solid_test, int range, signed int range_disp, bool rev_x, bool rev_y)
{
	int pos_x = point->pos_x;
	int pos_y = point->pos_y;
	int disp = 0;

	for (int r = range; r > 0; r--) {
		// get nearest block and info here
		point->metablock_index = map->layout[cr_get_metablock_y(map, pos_y)][cr_get_metablock_x(map, pos_x)];
		point->block_bitmask = map->metablocks[point->metablock_index][cr_get_metablock_block_n(map, pos_x, pos_y)];

		int block_rel_pix_x = cr_get_pixel_x(map, pos_x);		// Get the relative X-position within the block
		int block_rel_pix_y = cr_get_pixel_y(map, pos_y);		// Get the relative Y-position within the block (used later)

		point->block_index = cr_get_block_index(map, point->block_bitmask);

		point->flip_x = cr_get_block_flip_x(map, point->block_bitmask);
		point->flip_y = cr_get_block_flip_y(map, point->block_bitmask);

		point->block_solidity = cr_get_block_solidity(map, point->block_bitmask);

		// If the block is not blank and is solid...
		if (point->block_index > 0 && (point->block_solidity & solid_test) == solid_test) {
			point->angle = anglemap->angles[point->block_index];

			// If the block is X-flipped, adjust to get the appropriate height
			if (point->flip_x) {
				point->block_height = cr_get_block_height_horiz(blocks, point->block_index, (map->block_width - 1) - block_rel_pix_x);
				point->angle = -(point->angle);
			}
			else {
				point->block_height = cr_get_block_height_horiz(blocks, point->block_index, block_rel_pix_x);
			}

			// If the block is Y-flipped, negate the height
			if (point->flip_y) {
				point->angle = -(point->angle + 0x40) - 0x40;
			}

			if (rev_x) {
				point->flip_x = point->flip_x ^ 1;			// invert the X-flip flag
			}

			if (rev_y) {
				point->flip_y = point->flip_y ^ 1;			// invert the Y-flip flag
			}

			if (point->flip_y) {
				point->block_height = -(point->block_height);
			}

			// Handling the given heightmask
			// If the heightmask is zero...
			if (point->block_height == 0) {
				// look at block below
				if (r > 1) {
					pos_y += range_disp;
					disp += map->block_height;
					continue;
				}
				else {
					// do handling before handing off the final heightmask
					point->dist = (map->block_height - block_rel_pix_y) + disp;
					break;
				}
			}

			// If the heightmask is negative...
			else if (point->block_height < 0) {
				// negative heightmask handling (TODO later, i'm too tired rn)
				if ((block_rel_pix_y + point->block_height) > 0) {
					// look at block below
					if (r > 1) {
						pos_y += range_disp;
						disp += map->block_height;
						continue;
					}
					else {
						// do handling before handing off the final heightmask
						point->dist = (map->block_height - block_rel_pix_y) + disp;
						break;
					}
				}

				else {
					if (r > 1) {
						pos_y -= range_disp;				// Proceed to check one block above
						disp -= map->block_height;
						continue;
					}
					else {
						// do handling before handing off the final heightmask
						point->dist = (map->block_height - (block_rel_pix_y + abs(point->block_height))) + disp;
						break;
					}
				}
			}

			// If the heightmask is the maximum height...
			else if (point->block_height == map->block_height || point->block_height == 0 - map->block_height) {
				if (r > 1) {
					pos_y -= range_disp;				// Proceed to check one block above
					disp -= map->block_height;
					continue;
				}
				else {
					// do handling before handing off the final heightmask
					point->dist = (map->block_height - (block_rel_pix_y + abs(point->block_height))) + disp;
					break;
				}
			}
			// If the heightmask is otherwise normal
			else {
				// normal heightmask handling, heightmask isn't max or min nor negative
				point->dist = (map->block_height - (block_rel_pix_y + point->block_height)) + disp;
				break;
			}
		}

		else {
			// check if range has been exceeded
			// if not, look at block below
			if (r > 1) {
				pos_y += range_disp;	// modify Y to point to block below
				disp += map->block_height;
				continue;
			}
			else {
				// Do whatever is needed to clean up before handing off the final distance
				point->dist = (map->block_height - block_rel_pix_y) + disp;
				break;
			}
		}
	}

	if (cr_debug.game_debug) {
		vec2i pos;
		pos.x = point->pos_x;
		pos.y = point->pos_y;
		if (rev_y) { pos.y ^= 0xF; }
		if (rev_x) { pos.x ^= 0xF; }
		cr_sprite_draw(cr_debug.sprites[cr_debug_sensorpoint], pos.x, pos.y, point->flip_x, 7);
	}

	switch (point->block_solidity) {
	case CR_NO_SOLID: point->collision = false; break;				// Not solid
	case CR_TOP_SOLID: point->collision = true; break;				// Top Solid
	case CR_LRB_SOLID: point->collision = true; break;				// Left/Right/Bottom Solid
	case CR_ALL_SOLID: point->collision = true; break; // All solid
	default: point->collision = false;
	}

	return point->collision;
}

// Function for getting distance from a wall
// Outputs information about the collision of the current wall block, as well as the distance from the wall relative to the collision point
bool cr_colpnt_findwall_sep(CR_COLLISION_MAP *map, CR_BLOCKSET *blocks, CR_ANGLEMAP *anglemap, CR_COLLISION_POINT *point, CR_SOLIDITY solid_test, int range, signed int range_disp, bool rev_x, bool rev_y) {
	int pos_x = point->pos_x;
	int pos_y = point->pos_y;
	int disp = 0;

	for (int r = range; r > 0; r--) {
		// get nearest block and info here
		point->metablock_index = map->layout[cr_get_metablock_y(map, pos_y)][cr_get_metablock_x(map, pos_x)];
		point->block_bitmask = map->metablocks[point->metablock_index][cr_get_metablock_block_n(map, pos_x, pos_y)];

		int debug_x = pos_x - ((pos_x / (map->metablock_width * map->block_width))*(map->metablock_width * map->block_width));
		int debug_y = pos_y - ((pos_y / (map->metablock_height * map->block_height))*(map->metablock_height * map->block_height));

		int block_rel_pix_x = cr_get_pixel_x(map, pos_x);		// Get the relative X-position within the block
		int block_rel_pix_y = cr_get_pixel_y(map, pos_y);		// Get the relative Y-position within the block (used later)

		point->block_index = cr_get_block_index(map, point->block_bitmask);

		point->flip_x = cr_get_block_flip_x(map, point->block_bitmask);
		point->flip_y = cr_get_block_flip_y(map, point->block_bitmask);

		point->block_solidity = cr_get_block_solidity(map, point->block_bitmask);

		// If the block is not blank and is solid...
		if (point->block_index > 0 && (point->block_solidity & solid_test) == solid_test) {
			point->angle = anglemap->angles[point->block_index];

			// If the block is Y-flipped, adjust to get the appropriate height
			if (point->flip_y) {
				point->block_height = cr_get_block_height_verti(blocks, point->block_index, (map->block_height - 1) - block_rel_pix_y);
				point->angle = -(point->angle + 0x40) - 0x40;
			}
			else {
				point->block_height = cr_get_block_height_verti(blocks, point->block_index, block_rel_pix_y);
			}

			// If the block is X-flipped, negate the height
			if (point->flip_x) {
				//point->block_height = -(point->block_height);
				point->angle = -(point->angle);
			}

			if (rev_x) {
				point->flip_x = point->flip_x ^ 1;			// invert the X-flip flag
			}

			if (rev_y) {
				point->flip_y = point->flip_y ^ 1;			// invert the Y-flip flag
			}

			if (point->flip_x) {
				point->block_height = -(point->block_height);
			}

			// Handling the given heightmask
			// If the heightmask is zero...
			if (point->block_height == 0) {
				// look at block below
				if (r > 1) {
					pos_x += range_disp;
					disp += map->block_width;
					continue;
				}
				else {
					// do handling before handing off the final heightmask
					point->dist = (map->block_width - block_rel_pix_x) + disp;
					break;
				}
			}

			// If the heightmask is negative...
			else if (point->block_height < 0) {
				if ((block_rel_pix_x + point->block_height) > 0) {
					// look at block below
					if (r > 1) {
						pos_x += range_disp;
						disp += map->block_width;
						continue;
					}
					else {
						// do handling before handing off the final heightmask
						point->dist = (map->block_width - block_rel_pix_x) + disp;
						break;
					}
				}

				else {
					if (r > 1) {
						pos_x -= range_disp;				// Proceed to check one block above
						disp -= map->block_width;
						continue;
					}
					else {
						// do handling before handing off the final heightmask
						point->dist = (map->block_height - (block_rel_pix_x + abs(point->block_height))) + disp;
						break;
					}
				}
			}

			// If the heightmask is the maximum height...
			else if (point->block_height == map->block_width || point->block_height == 0 - map->block_width) {
				if (r > 1) {
					pos_x -= range_disp;				// Proceed to check one block above
					disp -= map->block_width;
					continue;
				}
				else {
					// do handling before handing off the final heightmask
					point->dist = (map->block_height - (block_rel_pix_x + point->block_height)) + disp;
					break;
				}
			}
			// If the heightmask is otherwise normal
			else {
				// normal heightmask handling, heightmask isn't max or min nor negative
				point->dist = (map->block_height - (block_rel_pix_x + point->block_height)) + disp;
				break;
			}
		}

		else {
			// check if range has been exceeded
			// if not, look at block to the right
			if (r > 1) {
				pos_x += range_disp;
				disp += map->block_width;
				continue;
			}
			else {
				// Do whatever is needed to clean up before handing off the final distance
				point->dist = (map->block_width - block_rel_pix_x) + disp;
				break;
			}
		}
	}

	if (cr_debug.game_debug) {
		vec2i pos;
		pos.x = point->pos_x;
		pos.y = point->pos_y;
		if (rev_y) { pos.y ^= 0xF; }
		if (rev_x) { pos.x ^= 0xF; }
		cr_sprite_draw(cr_debug.sprites[cr_debug_sensorpoint], pos.x, pos.y, point->flip_x, 7);
	}

	switch (point->block_solidity) {
	case CR_NO_SOLID: point->collision = false; break;				// Not solid
	case CR_TOP_SOLID: point->collision = true; break;				// Top Solid
	case CR_LRB_SOLID: point->collision = true; break;				// Left/Right/Bottom Solid
	case CR_ALL_SOLID: point->collision = true; break; // All solid
	default: point->collision = false;
	}

	return point->collision;
}

//GET SYSTEM

//Metablocks
unsigned int cr_get_metablock_x(CR_COLLISION_MAP *map, int pos_x)
{
	while (pos_x < 0) {
		pos_x += ((map->level_width * (map->metablock_width * map->block_width)));
	}
	while (pos_x >= ((int)map->level_width * ((int)map->metablock_width * (int)map->block_width))) {
		pos_x -= ((map->level_width * (map->metablock_width * map->block_width)));
	}
	unsigned int metablock_x;
	metablock_x = pos_x / (map->metablock_width * map->block_width);
	return metablock_x;
}
unsigned int cr_get_metablock_y(CR_COLLISION_MAP *map, int pos_y)
{
	while (pos_y < 0) {
		pos_y += ((map->level_height * (map->metablock_height * map->block_height)));
	}
	while (pos_y >= ((int)map->level_height * ((int)map->metablock_height * (int)map->block_height))) {
		pos_y -= ((map->level_height * (map->metablock_height * map->block_height)));
	}
	unsigned int metablock_y;
	metablock_y = pos_y / (map->metablock_height * map->block_height);
	return metablock_y;
}
unsigned int cr_get_metablock_n(CR_COLLISION_MAP *map, int pos_x, int pos_y)
{
	unsigned int metablock_n;
	metablock_n = (cr_get_metablock_y(map, pos_y) * (map->level_width)) + cr_get_metablock_x(map, pos_x);
	return metablock_n;
}

//Block numbers within map
unsigned int cr_get_block_x(CR_COLLISION_MAP *map, int pos_x)
{
	while (pos_x < 0) {
		pos_x += ((map->level_width * (map->metablock_width * map->block_width)));
	}
	while (pos_x >= ((int)map->level_width * ((int)map->metablock_width * (int)map->block_width))) {
		pos_x -= ((map->level_width * (map->metablock_width * map->block_width)));
	}
	unsigned int block_x;
	block_x = pos_x / map->block_width;
	return block_x;
}
unsigned int cr_get_block_y(CR_COLLISION_MAP *map, int pos_y)
{
	while (pos_y < 0) {
		pos_y += ((map->level_height * (map->metablock_height * map->block_height)));
	}
	while (pos_y >= ((int)map->level_height * ((int)map->metablock_height * (int)map->block_height))) {
		pos_y -= ((map->level_height * (map->metablock_height * map->block_height)));
	}
	unsigned int block_y;
	block_y = pos_y / map->block_height;
	return block_y;
}
unsigned int cr_get_block_n(CR_COLLISION_MAP *map, int pos_x, int pos_y)
{
	unsigned int block_n;
	block_n = (cr_get_block_y(map, pos_y) * (map->level_width * map->block_width)) + cr_get_block_x(map, pos_x);
	return block_n;
}

//Block numbers within metablock
unsigned int cr_get_metablock_block_x(CR_COLLISION_MAP *map, int pos_x)
{
	while (pos_x < 0) {
		pos_x += ((map->level_width * (map->metablock_width * map->block_width)));
	}
	while (pos_x >= ((int)map->level_width * ((int)map->metablock_width * (int)map->block_width))) {
		pos_x -= ((map->level_width * (map->metablock_width * map->block_width)));
	}
	unsigned int block_x;
	block_x = CR_MOD(pos_x, (map->metablock_width * map->block_width)) / map->block_width;
	return block_x;
}
unsigned int cr_get_metablock_block_y(CR_COLLISION_MAP *map, int pos_y)
{
	while (pos_y < 0) {
		pos_y += ((map->level_height * (map->metablock_height * map->block_height)));
	}
	while (pos_y >= ((int)map->level_height * ((int)map->metablock_height * (int)map->block_height))) {
		pos_y -= ((map->level_height * (map->metablock_height * map->block_height)));
	}
	unsigned int block_y;
	block_y = CR_MOD(pos_y, (map->metablock_height * map->block_height)) / map->block_height;
	return block_y;
}
unsigned int cr_get_metablock_block_n(CR_COLLISION_MAP *map, int pos_x, int pos_y)
{
	//Thanks to Jaxson Trotti for helping me think of the way to get the block number!
	unsigned int block_n;
	block_n = (cr_get_metablock_block_y(map, pos_y) * (map->metablock_width)) + cr_get_metablock_block_x(map, pos_x);
	return block_n;
}

unsigned int cr_get_pixel_x(CR_COLLISION_MAP *map, int pos_x)
{
	unsigned int pixel_x;
	pixel_x = CR_MOD(pos_x, map->block_width);
	return pixel_x;
}
unsigned int cr_get_pixel_y(CR_COLLISION_MAP *map, int pos_y)
{
	unsigned int pixel_y;
	pixel_y = CR_MOD(pos_y, map->block_height);
	return pixel_y;
}
unsigned int cr_get_pixel_n(CR_COLLISION_MAP *map, int pos_x, int pos_y)
{
	unsigned int pixel_n;
	pixel_n = (cr_get_pixel_y(map, pos_y) * (map->block_width)) + cr_get_pixel_x(map, pos_x);
	return pixel_n;
}

unsigned int cr_get_block_index(CR_COLLISION_MAP *map, unsigned int block_bitmask)
{
	unsigned int result;
	result = (block_bitmask & ((1 << 11) - 1));
	return result;
}

unsigned char cr_get_block_solidity(CR_COLLISION_MAP *map, unsigned int block_bitmask)
{
	bool bit1;
	bool bit2;
	unsigned char result;
	bit1 = (block_bitmask & 1 << 13);
	bit2 = (block_bitmask & 1 << 12);
	result = (bit1 * 2) + bit2;
	return result;
}

//Get flags
bool cr_get_block_flip_x(CR_COLLISION_MAP *map, unsigned int block_bitmask)
{
	bool result;
	result = (block_bitmask & 1 << 15);
	return result;
}
bool cr_get_block_flip_y(CR_COLLISION_MAP *map, unsigned int block_bitmask)
{
	bool result;
	result = (block_bitmask & 1 << 14);
	return result;
}

bool cr_get_metablock_flip_x(CR_COLLISION_MAP *map, unsigned int metablock_bitmask)
{
	bool result;
	result = (metablock_bitmask & 1 << 15);
	return result;
}
bool cr_get_metablock_flip_y(CR_COLLISION_MAP *map, unsigned int metablock_bitmask)
{
	bool result;
	result = (metablock_bitmask & 1 << 14);
	return result;
}

//Get height values
// (Used for interaction with floors/ceilings)
unsigned int cr_get_block_height_horiz(CR_BLOCKSET* blocks, unsigned int block_index, int pixel_x)
{
	unsigned char result;
	result = blocks->heightmasks[block_index][pixel_x];
	return result;
}

// Get height value from a rotated version of a heightmask
// (Used for interaction with walls)
unsigned int cr_get_block_height_verti(CR_BLOCKSET* blocks, unsigned int block_index, int pixel_y)
{
	unsigned char result;
	result = blocks->rot_heightmasks[block_index][pixel_y];
	return result;
}

// ================================
// EASY COLLISION FUNCTIONS
// ================================

int cr_colpnt_update(CR_COLLISION_POINT* colpnt, unsigned char priority, unsigned char col_type)
{
	int prev = 0;

	switch (col_type)
	{
	case CR_COL_FLOOR:
		cr_colpnt_findfloor_sep(&cr_stage.current->colmaps[priority], &cr_stage.current->blocksets[0], &cr_stage.current->anglemaps[0], colpnt, CR_TOP_SOLID, 2, cr_stage.current->colmaps[priority].block_height, false, false);
		break;

	case CR_COL_CEIL:
		prev = colpnt->pos_y;
		colpnt->pos_y ^= 0xF;
		cr_colpnt_findfloor_sep(&cr_stage.current->colmaps[priority], &cr_stage.current->blocksets[0], &cr_stage.current->anglemaps[0], colpnt, CR_TOP_SOLID, 2, 0 - cr_stage.current->colmaps[priority].block_height, false, true);
		colpnt->pos_y = prev;
		break;

	case CR_COL_LWALL:
		cr_colpnt_findwall_sep(&cr_stage.current->colmaps[priority], &cr_stage.current->blocksets[0], &cr_stage.current->anglemaps[0], colpnt, CR_TOP_SOLID, 2, 0 - cr_stage.current->colmaps[priority].block_height, true, false);
		break;

	case CR_COL_RWALL:
		cr_colpnt_findwall_sep(&cr_stage.current->colmaps[priority], &cr_stage.current->blocksets[0], &cr_stage.current->anglemaps[0], colpnt, CR_TOP_SOLID, 2, cr_stage.current->colmaps[priority].block_height, false, false);
		break;
	}

	return(colpnt->dist);
}


int cr_get_floordist(float pos_x, float pos_y, unsigned char priority)
{
	CR_COLLISION_POINT colpnt;
	colpnt.pos_x = pos_x;
	colpnt.pos_y = pos_y;
	cr_colpnt_findfloor_sep(&cr_stage.current->colmaps[priority], &cr_stage.current->blocksets[0], &cr_stage.current->anglemaps[0], &colpnt, CR_TOP_SOLID, 2, cr_stage.current->colmaps[priority].block_height, false, false);
	return(colpnt.dist);
}

int cr_get_ceildist(float pos_x, float pos_y, unsigned char priority)
{
	CR_COLLISION_POINT colpnt;
	colpnt.pos_x = pos_x;
	colpnt.pos_x ^= 0xF;
	colpnt.pos_y = pos_y;
	cr_colpnt_findfloor_sep(&cr_stage.current->colmaps[priority], &cr_stage.current->blocksets[0], &cr_stage.current->anglemaps[0], &colpnt, CR_TOP_SOLID, 2, 0 - cr_stage.current->colmaps[priority].block_height, false, true);
	return(colpnt.dist);
}

int cr_get_lwalldist(float pos_x, float pos_y, unsigned char priority)
{
	CR_COLLISION_POINT colpnt;
	colpnt.pos_x = pos_x;
	colpnt.pos_x ^= 0xF;
	colpnt.pos_y = pos_y;
	cr_colpnt_findwall_sep(&cr_stage.current->colmaps[priority], &cr_stage.current->blocksets[0], &cr_stage.current->anglemaps[0], &colpnt, CR_LRB_SOLID, 2, 0 - cr_stage.current->colmaps[priority].block_height, true, false);
	return(colpnt.dist);
}

int cr_get_rwalldist(float pos_x, float pos_y, unsigned char priority)
{
	CR_COLLISION_POINT colpnt;
	colpnt.pos_x = pos_x;
	colpnt.pos_y = pos_y;
	cr_colpnt_findwall_sep(&cr_stage.current->colmaps[priority], &cr_stage.current->blocksets[0], &cr_stage.current->anglemaps[0], &colpnt, CR_LRB_SOLID, 2, cr_stage.current->colmaps[priority].block_height, false, false);
	return(colpnt.dist);
}

int cr_get_floorheight(float pos_x, float pos_y, unsigned char priority)
{
	CR_COLLISION_POINT colpnt;
	colpnt.pos_x = pos_x;
	colpnt.pos_y = pos_y;
	cr_colpnt_findfloor_sep(&cr_stage.current->colmaps[priority], &cr_stage.current->blocksets[0], &cr_stage.current->anglemaps[0], &colpnt, CR_TOP_SOLID, 2, cr_stage.current->colmaps[priority].block_height, false, false);
	return(colpnt.block_height);
}

int cr_get_floordist_sep(float pos_x, float pos_y, CR_COLLISION_MAP* map, CR_BLOCKSET* blocks, CR_ANGLEMAP* anglemap)
{
	CR_COLLISION_POINT colpnt;
	colpnt.pos_x = pos_x;
	colpnt.pos_y = pos_y;
	cr_colpnt_findfloor_sep(map, blocks, anglemap, &colpnt, CR_TOP_SOLID, 2, map->block_height, false, false);
	return(colpnt.dist);
}

int cr_get_ceildist_sep(float pos_x, float pos_y, CR_COLLISION_MAP* map, CR_BLOCKSET* blocks, CR_ANGLEMAP* anglemap)
{
	CR_COLLISION_POINT colpnt;
	colpnt.pos_x = pos_x;
	colpnt.pos_y = pos_y;
	cr_colpnt_findfloor_sep(map, blocks, anglemap, &colpnt, CR_TOP_SOLID, 2, map->block_height, false, true);
	return(colpnt.dist);
}

int cr_get_walldist_sep(float pos_x, float pos_y, CR_COLLISION_MAP* map, CR_BLOCKSET* blocks, CR_ANGLEMAP* anglemap)
{
	CR_COLLISION_POINT colpnt;
	colpnt.pos_x = pos_x;
	colpnt.pos_y = pos_y;
	cr_colpnt_findfloor_sep(map, blocks, anglemap, &colpnt, CR_LRB_SOLID, 2, map->block_height, false, false);
	return(colpnt.dist);
}

int cr_get_floorheight_sep(float pos_x, float pos_y, CR_COLLISION_MAP* map, CR_BLOCKSET* blocks, CR_ANGLEMAP* anglemap)
{
	CR_COLLISION_POINT colpnt;
	colpnt.pos_x = pos_x;
	colpnt.pos_y = pos_y;
	cr_colpnt_findfloor_sep(map, blocks, anglemap, &colpnt, CR_TOP_SOLID, 2, map->block_height, false, false);
	return(colpnt.block_height);
}