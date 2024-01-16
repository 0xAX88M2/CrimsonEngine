#ifndef CR_AUDIO_H
#define CR_AUDIO_H

typedef Mix_Music *CR_MUSIC;
typedef Mix_Chunk *CR_SOUND;

typedef struct {
	int volume_music;
} CR_AUDIO_STRUCT;

CR_AUDIO_STRUCT cr_audio;

//LOAD/UNLOAD
CR_MUSIC cr_music_load(const char *filepath);
void cr_music_unload(CR_MUSIC music_id);

CR_SOUND cr_sound_load(const char *filepath);
void cr_sound_unload(void *sample_id);

//MAIN FUNCTIONS
void cr_sound_play(void *sample_id, int loop_count);
void cr_sound_channel_play(void *sample_id, int loop_count, unsigned int channel);
void cr_music_play(void *music_id, int loop_count);
void cr_music_stop(void);
void cr_music_pause(void);
void cr_music_resume(void);
void cr_music_finished_hook(void(*music_finished)());
void cr_channel_stop(int channel);
void cr_channel_pause(int channel);
void cr_channel_resume(int channel);

//FADING
void cr_music_volume(int volume);


#endif // !CR_AUDIO_H
