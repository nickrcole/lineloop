
#ifndef AUDIO
#define AUDIO

#include "/usr/include/portaudio.h"

#define FRAMES_PER_BUFFER 2205

typedef float SAMPLE;

typedef struct {
    SAMPLE*  audio_buf;
    SAMPLE*  fft_buf;
} AUDIO_PACKAGE;

void audio_driver_init( double* bands );

int begin_audio_monitoring(AUDIO_PACKAGE* package);

void fft(SAMPLE* fft_buf);

void audio_processor_init(double* bands_buf, int barNum);

#endif