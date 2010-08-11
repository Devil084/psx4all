#include "spu.h"
#include <minimal.h>

#ifndef NOSOUND
#define OSS_MODE_STEREO	1
#define OSS_MODE_MONO	0
#define OSS_SPEED_44100	44100
static int oss_audio_fd = -1;

/* Sound Thread variables */
#define DEFAULT_SAMPLE_NUM_BUFF 16 								// Number of sound buffers
//pthread_t gp2x_sound_thread=0;									// Thread for gp2x_sound_thread_play()
volatile int gp2x_sound_thread_exit=0;								// Flag to end gp2x_sound_thread_play() thread
volatile int gp2x_sound_buffer=0;								// Current sound buffer
short gp2x_sound_buffers[32768*DEFAULT_SAMPLE_NUM_BUFF];					// Sound buffers
volatile int sndlen=32768;									// Current sound buffer length
#endif

/* Sound Thread*/
static void *gp2x_sound_thread_play(void *none)
{
	return NULL;
}

/* Mute Sound Thread*/
void gp2x_sound_thread_mute(void)
{
}

/* Start Sound Core */
void SetupSound(void)
{

}

/* Stop Sound Core */
void RemoveSound(void)
{

}

/* Feed Sound Data */
void SoundFeedStreamData(unsigned char* pSound,long lBytes)
{
}
