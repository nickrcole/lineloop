/**
 * @file animations.c
 * @date 12/30/23
 * @brief Animation functions for components
 * @author Nicholas Cole https://nicholascole.dev
 */

#include <stdio.h>
#include "./../include/render_toolkit.h"
#include "./../include/animations.h"

double* bands_ptr;
int anim_iter = 0;

Point scroll_forward( Point pos, double speed ) {
    Point new_pos;
    new_pos.y = pos.y;        // No change in the y direction
    if ( (int) pos.x == 0 ) {       // Decrement x and account for rollover
        new_pos.x = 294; /// speed;
    } else {
        new_pos.x = pos.x - (1 * speed);
    }
    return new_pos;
}

Point scroll_backward( Point pos, double speed ) {
    Point new_pos;
    new_pos.y = pos.y;        // No change in the y direction
    if ( (int) pos.x == 294 ) {       // Decrement x and account for rollover
        new_pos.x = 0;
    } else {
        new_pos.x = pos.x + (1 * speed);
    }
    return new_pos;
}

Point bar_jitter( Point pos, double speed ) {
    if (!bands_ptr) {
        return pos;
    }
    double max_val = 0;
    for (int i = 0; i < NUM_BARS; i++) {
        // printf("%f ", bands_ptr[i]);
        if (bands_ptr[i] > max_val) {
            max_val = bands_ptr[i];
        }
    }
    // printf("\n");
    // printf("Max: %f\n", max_val);
    int pix_cutoff = (max_val / NUM_BARS);
    Point new_pos;
    new_pos.x = pos.x;
    new_pos.y = (int) (bands_ptr[anim_iter] / max_val) * 14;
    printf("%f\n", (bands_ptr[0] / max_val) * 14);
    anim_iter++;
    return new_pos;
}

Point blank( Point pos, double speed ) {
    return pos;
}

void audio_reactive_anim_init(double* band_buf) {
    bands_ptr = band_buf;
}

void anim_iter_reset(void) {
    anim_iter = 0;
}