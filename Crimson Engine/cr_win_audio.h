#ifndef CR_WIN_AUDIO_H
#define CR_WIN_AUDIO_H

//LOAD/UNLOAD
CR_MUSIC cr_windows_music_load(const char *filepath);
void cr_windows_music_unload(CR_MUSIC music_id);

CR_SOUND cr_windows_sound_load(const char *filepath);
void cr_windows_sound_unload(void *sample_id);

//MAIN FUNCTIONS
void cr_windows_sound_play(void *sample_id, int loop_count);
void cr_windows_sound_channel_play(void *sample_id, int loop_count, unsigned int channel);
void cr_windows_music_play(void *music_id, int loop_count);
void cr_windows_music_stop(void);
void cr_windows_music_pause(void);
void cr_windows_music_resume(void);
void cr_windows_music_finished_hook(void(*music_finished)());
void cr_windows_channel_stop(int channel);
void cr_windows_channel_pause(int channel);
void cr_windows_channel_resume(int channel);

//FADING
void cr_windows_music_volume(int volume);


#endif // !CR_WIN_AUDIO_H
