#ifndef CR_STAGE_H
#define CR_STAGE_H

typedef enum {
	CR_TILE_DRAW_CHUNKS,
	CR_TILE_DRAW_ONECHUNK,
	CR_TILE_DRAW_BIGCHUNKS,
} CR_TILE_DRAW_MODE;

typedef struct {

	//How many blocks the tilesheet is
	unsigned int sheet_width;
	unsigned int sheet_height;

	//Tiles are the visual representation of a level.
	unsigned int tile_width;		//Size of a visual tile in pixels
	unsigned int tile_height;

	//Chunks are groupings of tiles.
	unsigned int chunk_width;		//Size of a chunk in tiles
	unsigned int chunk_height;

	//The "tilemap" is the visible equivalent of above.
	unsigned int tilemap_width;
	unsigned int tilemap_height;

	//Assembly stuff
	unsigned int chunk_count;

	unsigned int **chunks;
	unsigned int **layout;

	bool loaded;

	bool sequence_mode; //If true, display from separate images instead of a sheet

	unsigned char draw_mode; //As defined by the CR_TILE_DRAW_MODE enum

	unsigned char layer;

} CR_TILE_MAP;

typedef struct {

	//COLLISION BLOCKS

	//Collision blocks represent height data to be used for collision.
	unsigned int block_width;		//Size of collision block in pixels
	unsigned int block_height;

	//Metablocks are groupings of collision blocks.
	unsigned int metablock_width;	//Size of a metablock in collision tiles (how many collision blocks per metablock)
	unsigned int metablock_height;

	//The "level" is the grouping of blocks and metablocks to make a map with collision
	unsigned int level_width;		//How many metablocks wide/tall a level is
	unsigned int level_height;

	unsigned int metablock_count;

	unsigned int **metablocks;
	unsigned int **layout;

	unsigned char priority;

	bool loaded;

} CR_COLLISION_MAP;

typedef struct {

	unsigned int width; //In pixels
	unsigned int height;

	unsigned int count;

	int **heightmasks;
	int **rot_heightmasks;

	bool loaded;

} CR_BLOCKSET;

typedef struct {

	unsigned short count;
	unsigned char *angles;
	bool loaded;
} CR_ANGLEMAP;

typedef struct {

	//FUNCTIONS
	void(*update)(void);
	void(*init)(void);
	void(*load)(void);
	void(*unload)(void);

	//DATA
	CR_BOUNDS level_limit;
	CR_BOUNDS cam_limit;

	unsigned char blockset_count;
	CR_BLOCKSET *blocksets;
	unsigned char blockset_id;

	unsigned char anglemap_count;
	CR_ANGLEMAP *anglemaps;
	unsigned char anglemap_id;

	unsigned char tilemap_count;
	CR_TILE_MAP *tilemaps;

	unsigned char colmap_count;
	CR_COLLISION_MAP *colmaps;

	//INFO
	char *name;
	bool name_loaded;
	bool info_loaded;
	bool loaded;
	bool initialized;

	char bgmode;

	unsigned short id;

} CR_STAGE;

typedef struct {

	CR_STAGE *current;
	CR_STAGE *previous;

	unsigned short stage_count;
	CR_STAGE **stage_pointers;
	bool initialized;

} CR_STAGE_MANAGER;

CR_STAGE_MANAGER cr_stage;

void cr_stage_goto(CR_STAGE *stage);

//MANAGER FUNCTIONS

void cr_stage_add(CR_STAGE *stage, const char *const name, void(*load)(void), void(*unload)(void), void(*init)(void), void(*update)(void));
void cr_stage_manager_reset(void);
void cr_stage_unload_all(void);

//Soon to be added!
void cr_stage_load_info(CR_STAGE *stage, const char *filepath);
void cr_stage_unload_info(CR_STAGE *stage);

//FUNCTIONS

void cr_stage_load_tilemap(const char *filepath, CR_TILE_MAP *map);
void cr_stage_unload_tilemap(CR_TILE_MAP *map);

void cr_stage_load_collision(const char *filepath, CR_COLLISION_MAP *map);
void cr_stage_unload_collision(CR_COLLISION_MAP *map);

void cr_stage_load_blockset(const char *filepath, CR_BLOCKSET *blocks);
void cr_stage_unload_blockset(CR_BLOCKSET *blocks);

void cr_stage_load_anglemap(const char *filepath, CR_ANGLEMAP *angles);
void cr_stage_unload_anglemap(CR_ANGLEMAP *angles);

void cr_tilemap_draw(CR_TEXTURE *tileset, CR_TILE_MAP *map, unsigned char draw_layer);
void cr_tilemap_chunk_draw(CR_TEXTURE *tileset, CR_TILE_MAP *map, int layout_x, int layout_y, unsigned char screen_delegate, unsigned char draw_layer);

void cr_stage_draw(CR_TEXTURE *tileset, CR_STAGE *stage);

#endif // !CR_STAGE_H
