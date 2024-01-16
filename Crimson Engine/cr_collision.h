#ifndef CR_COLLISION_H
#define CR_COLLISION_H

typedef enum {
	CR_COL_FLOOR,
	CR_COL_CEIL,
	CR_COL_LWALL,
	CR_COL_RWALL,
} CR_COLLISION_TYPES;

typedef enum {
	CR_NO_SOLID,
	CR_TOP_SOLID,
	CR_LRB_SOLID,
	CR_ALL_SOLID,
} CR_SOLIDITY;

typedef struct {
	vec2i pos;
	vec2us size;
} CR_BOX_2D;

typedef struct {
	vec3i pos;
	vec3us size;
} CR_BOX_3D;


typedef struct {

	bool collision; //This is TRUE if there is a collision

	bool is_checking_up;
	bool is_checking_vert;

	int pos_x; //What position will we retrieve collision at?
	int pos_y;

	int offset_x;
	int offset_y;

	//Index stuff
	unsigned int metablock_index;
	unsigned int block_index;

	unsigned int block_bitmask;

	unsigned char block_solidity;
	bool flip_x; //X and Y flip flags
	bool flip_y;
	char block_height;

	char angle;
	short dist;

} CR_COLLISION_POINT;

bool cr_bbox_check(CR_BOX_2D *bbox1, CR_BOX_2D *bbox2);

//UPDATE COLLISION
bool cr_colpnt_findfloor(CR_STAGE *stage, unsigned char col_priority, CR_COLLISION_POINT *point, CR_SOLIDITY solid_test, int range, signed int range_disp, bool rev_x, bool rev_y);
bool cr_colpnt_findwall(CR_STAGE *stage, unsigned char col_priority, CR_COLLISION_POINT *point, CR_SOLIDITY solid_test, int range, signed int range_disp, bool rev_x, bool rev_y);

bool cr_colpnt_findfloor_sep(CR_COLLISION_MAP *map, CR_BLOCKSET *blocks, CR_ANGLEMAP *anglemap, CR_COLLISION_POINT *point, CR_SOLIDITY solid_test, int range, signed int range_disp, bool rev_x, bool rev_y);
bool cr_colpnt_findwall_sep(CR_COLLISION_MAP *map, CR_BLOCKSET *blocks, CR_ANGLEMAP *anglemap, CR_COLLISION_POINT *point, CR_SOLIDITY solid_test, int range, signed int range_disp, bool rev_x, bool rev_y);

//GET SYSTEM
unsigned int cr_get_metablock_x(CR_COLLISION_MAP *map, int pos_x);
unsigned int cr_get_metablock_y(CR_COLLISION_MAP *map, int pos_y);
unsigned int cr_get_metablock_n(CR_COLLISION_MAP *map, int pos_x, int pos_y);

unsigned int cr_get_block_x(CR_COLLISION_MAP *map, int pos_x);
unsigned int cr_get_block_y(CR_COLLISION_MAP *map, int pos_y);
unsigned int cr_get_block_n(CR_COLLISION_MAP *map, int pos_x, int pos_y);

unsigned int cr_get_metablock_block_x(CR_COLLISION_MAP *map, int pos_x);
unsigned int cr_get_metablock_block_y(CR_COLLISION_MAP *map, int pos_y);
unsigned int cr_get_metablock_block_n(CR_COLLISION_MAP *map, int pos_x, int pos_y);

unsigned int cr_get_pixel_x(CR_COLLISION_MAP *map, int pos_x);
unsigned int cr_get_pixel_y(CR_COLLISION_MAP *map, int pos_y);
unsigned int cr_get_pixel_n(CR_COLLISION_MAP *map, int pos_x, int pos_y);

unsigned int cr_get_block_index(CR_COLLISION_MAP *map, unsigned int tile_bitmask);
unsigned char cr_get_block_solidity(CR_COLLISION_MAP *map, unsigned int tile_bitmask);

bool cr_get_block_flip_x(CR_COLLISION_MAP *map, unsigned int tile_bitmask);
bool cr_get_block_flip_y(CR_COLLISION_MAP *map, unsigned int tile_bitmask);

bool cr_get_metablock_flip_x(CR_COLLISION_MAP *map, unsigned int metablock_bitmask);
bool cr_get_metablock_flip_y(CR_COLLISION_MAP *map, unsigned int metablock_bitmask);

unsigned int cr_get_block_height_horiz(CR_BLOCKSET *blocks, unsigned int block_index, int pixel_x);
unsigned int cr_get_block_height_verti(CR_BLOCKSET *blocks, unsigned int block_index, int pixel_y);

// ================================
// EASY COLLISION FUNCTIONS
// ================================
int cr_colpnt_update(CR_COLLISION_POINT* col, unsigned char priority, unsigned char col_type);

int cr_get_floordist(float pos_x, float pos_y, unsigned char priority);
int cr_get_ceildist(float pos_x, float pos_y, unsigned char priority);
int cr_get_lwalldist(float pos_x, float pos_y, unsigned char priority);
int cr_get_rwalldist(float pos_x, float pos_y, unsigned char priority);
int cr_get_floorheight(float pos_x, float pos_y, unsigned char priority);

int cr_get_floordist_sep(float pos_x, float pos_y, CR_COLLISION_MAP* map, CR_BLOCKSET* blocks, CR_ANGLEMAP* anglemap);
int cr_get_ceildist_sep(float pos_x, float pos_y, CR_COLLISION_MAP* map, CR_BLOCKSET* blocks, CR_ANGLEMAP* anglemap);
int cr_get_walldist_sep(float pos_x, float pos_y, CR_COLLISION_MAP* map, CR_BLOCKSET* blocks, CR_ANGLEMAP* anglemap);
int cr_get_floorheight_sep(float pos_x, float pos_y, CR_COLLISION_MAP* map, CR_BLOCKSET* blocks, CR_ANGLEMAP* anglemap);

#endif // !CR_COLLISION_H
