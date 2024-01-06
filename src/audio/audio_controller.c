/**
 * @file audio_controller.c
 * @date 1/2/24
 * @brief Manages audio_recorder and audio_monitor and maintains audio buffer
 * @author Nicholas Cole https://nicholascole.dev
 */

#include <string.h>
#include <stdlib.h>
#include "./../../include/audio.h"
#include "./../../include/animations.h"
#define SAMPLE_RATE        (44100)
#define MAX_BUFFER_TIME          1

void audio_driver_init( double* bands ) {
    // double* bands = malloc(sizeof(double) * NUM_BARS);
    audio_processor_init(bands, NUM_BARS);

    /* Initialize audio buffer */
    int buf_size = FRAMES_PER_BUFFER * sizeof(SAMPLE);
    AUDIO_PACKAGE* package = malloc(sizeof(AUDIO_PACKAGE));
    package->audio_buf = malloc(buf_size);
    package->fft_buf = malloc(buf_size);
    memset(package->audio_buf, 0, buf_size);
    memset(package->fft_buf, 0, buf_size);

    begin_audio_monitoring(package);
}