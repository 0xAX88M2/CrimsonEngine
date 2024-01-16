#include "crimson.h"
#include "cr_win_audio.h"

CR_MUSIC cr_windows_music_load(const char *filepath)
{
#if CR_DEBUG_AUDIO == 1
	printf("Sample loaded from %s\n", filepath);
#endif
	return Mix_LoadMUS(filepath);
}

void cr_windows_music_unload(CR_MUSIC music_id)
{
	Mix_FreeMusic(music_id);
}

CR_SOUND cr_windows_sound_load(const char *filepath)
{
#if CR_DEBUG_AUDIO == 1
	printf("Sample loaded from %s\n", filepath);
#endif
	Mix_AllocateChannels(16);
	Mix_ReserveChannels(2);
	return Mix_LoadWAV(filepath);
}

void cr_windows_sound_unload(void *sample_id)
{
	Mix_FreeChunk(sample_id);
}

//MAIN FUNCTIONS
void cr_windows_sound_play(void *sample_id, int loop_count)
{
	Mix_PlayChannelTimed(-1, sample_id, loop_count, -1);
}

void cr_windows_sound_channel_play(void *sample_id, int loop_count, unsigned int channel)
{
	Mix_PlayChannelTimed(channel, sample_id, loop_count, -1);
}

void cr_windows_music_play(void *music_id, int loop_count)
{
	Mix_PlayMusic(music_id, loop_count);
}

void cr_windows_music_stop(void)
{
	Mix_HaltMusic();
}

void cr_windows_music_pause(void)
{
	Mix_PauseMusic();
}

void cr_windows_music_resume(void)
{
	Mix_ResumeMusic();
}

void cr_windows_music_finished_hook(void(*music_finished)())
{
	Mix_HookMusicFinished(music_finished);
}

void cr_windows_channel_stop(int channel)
{
	Mix_HaltChannel(channel);
}

void cr_windows_channel_pause(int channel)
{
	Mix_Pause(channel);
}

void cr_windows_channel_resume(int channel)
{
	Mix_Resume(channel);
}

//FADING

//Max volume value is 128.
void cr_windows_music_volume(int volume)
{
	if (volume < 0) {
		volume = 0;
	}

	if (volume > 128) {
		volume = 128;
	}

	Mix_VolumeMusic(volume);
}