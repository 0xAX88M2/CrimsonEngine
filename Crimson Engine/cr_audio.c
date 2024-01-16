#include "crimson.h"

#if CR_PLATFORM == CR_WINDOWS
#include "cr_win_audio.h"
#endif // CR_PLATFORM == CR_WINDOWS

//LOAD/UNLOAD
CR_MUSIC cr_music_load(const char *filepath)
{
#if CR_PLATFORM == CR_WINDOWS
	return cr_windows_music_load(filepath);
#endif // CR_PLATFORM == CR_WINDOWS
}
void cr_music_unload(CR_MUSIC music_id)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_music_unload(music_id);
#endif // CR_PLATFORM == CR_WINDOWS
}

CR_SOUND cr_sound_load(const char *filepath)
{
#if CR_PLATFORM == CR_WINDOWS
	return cr_windows_sound_load(filepath);
#endif // CR_PLATFORM == CR_WINDOWS
}
void cr_sound_unload(void *sample_id)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_sound_unload(sample_id);
#endif // CR_PLATFORM == CR_WINDOWS
}

//MAIN FUNCTIONS
void cr_sound_play(void *sample_id, int loop_count)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_sound_play(sample_id, loop_count);
#endif // CR_PLATFORM == CR_WINDOWS
}
void cr_sound_channel_play(void *sample_id, int loop_count, unsigned int channel)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_sound_channel_play(sample_id, loop_count, channel);
#endif // CR_PLATFORM == CR_WINDOWS
}

void cr_music_play(void *music_id, int loop_count)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_music_play(music_id, loop_count);
#endif // CR_PLATFORM == CR_WINDOWS
}

void cr_music_stop(void)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_music_stop();
#endif // CR_PLATFORM == CR_WINDOWS
}

void cr_music_pause(void)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_music_pause();
#endif // CR_PLATFORM == CR_WINDOWS
}

void cr_music_resume(void)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_music_resume();
#endif // CR_PLATFORM == CR_WINDOWS
}

void cr_music_finished_hook(void(*music_finished)())
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_music_finished_hook(music_finished);
#endif // CR_PLATFORM == CR_WINDOWS
}

void cr_channel_stop(int channel)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_channel_stop(channel);
#endif // CR_PLATFORM == CR_WINDOWS
}

void cr_channel_pause(int channel)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_channel_pause(channel);
#endif // CR_PLATFORM == CR_WINDOWS
}

void cr_channel_resume(int channel)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_channel_resume(channel);
#endif // CR_PLATFORM == CR_WINDOWS
}

//FADING

//Max volume value is 128.
void cr_music_volume(int volume)
{
#if CR_PLATFORM == CR_WINDOWS
	cr_windows_music_volume(volume);
#endif // CR_PLATFORM == CR_WINDOWS
}