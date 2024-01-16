#ifndef CR_CAMERA_H
#define CR_CAMERA_H

typedef struct {

	//This is where the camera will point at on screen.
	vec3f pos;

	// The camera's coordinates from the last frame, used to check how far the camera has moved
	vec3f last;

	//This is the camera's "true" coordinate.
	vec3i tru;

	//This is how you tell the camera where to point. 
	vec3i target;

	//Use this if you want to point the camera at something, but at an offset.
	vec3i offset;

	//Camera properties
	unsigned char mode;

	unsigned short step_limit; //Max pixels per step the camera can move

	CR_BOUNDS limit;
	CR_BOUNDS bound;

	vec3f bg;				// BG Camera position when the BG Cam mode is set to be used for drawing

	bool locked;
	bool unbound;

} CR_CAMERA;

typedef struct {
	
	CR_CAMERA **cams;

	unsigned char count;

	CR_CAMERA *target;

	bool initialized;

} CR_CAMERA_MANAGER;

CR_CAMERA cr_defaultcam;
CR_CAMERA_MANAGER cr_camera;

// FUNCTIONS
void cr_camera_init(CR_CAMERA *cam);
void cr_camera_apply(CR_CAMERA *target_camera);
void cr_camera_update(void);
void cr_camera_set_target_cam(CR_CAMERA *cam);

// Splitscreen
void cr_camera_manager_reset(void);
void cr_camera_add(CR_CAMERA *cam);
void cr_camera_remove(void);
void cr_screen_add(void);
void cr_camera_replace(CR_CAMERA *cam, unsigned char index);
void cr_screen_remove(void);

// Helpers
void cr_camera_set_pos(unsigned char cam_id, float pos_x, float pos_y);
void cr_camera_add_pos(unsigned char cam_id, float inc_x, float inc_y);

void cr_camera_set_pos_p(CR_CAMERA* cam, float pos_x, float pos_y);
void cr_camera_add_pos_p(CR_CAMERA* cam, float inc_x, float inc_y);

#endif // !CR_CAMERA_H
