/**
 * @file audio_controller.c
 * @date 1/2/24
 * @brief Manages audio_recorder and audio_processor and maintains audio buffer
 * @author Nicholas Cole https://nicholascole.dev
 */

#include "./../../include/audio.h"
#define SAMPLE_RATE      (44100)
#define MAX_BUFFER_TIME       1

void audio_init(void) {
    audio_recorder_init();

    /* Initialize audio buffer */
    int max_samples = MAX_BUFFER_TIME * SAMPLE_RATE;
    buf_size = max_samples * sizeof(SAMPLE);
    audio_buf = malloc(buf_size);
    memset(audio_buf, 0, buf_size);
}

void audio_cease(void) {
    audio_recorder_stop();
}