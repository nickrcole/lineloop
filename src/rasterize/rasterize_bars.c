/**
 * @file rasterize_bars.c
 * @date 12/30/23
 * @brief Rasterizes the bars in an audio-reactive way
 * @author Nicholas Cole https://nicholascole.dev
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../../include/render_toolkit.h"
#include "./../../include/audio.h"

// Can only be one of these factors of 294:
// 1, 2, 3, 6, 7, 14, 21, 42, 98, 147, 294
#define BAR_WIDTH 7

double* bands_buf;
int iter_label;

unsigned int get_pixel(int height) {
    Color color;
    color.red = 0x0 + height;
    color.green = 0x0;
    color.blue = 0x14 - height;
    unsigned int pixel = color.red;
    pixel <<= 8;
    pixel += color.green;
    pixel <<= 8;
    pixel += color.blue;
    return pixel;
}

int get_max(int a, int b) {
    return (a > b) ? a : b;
}

void rasterize_bars(Component* bars, frame_buf* rast) {
    int height;
    memset(rast, 0, sizeof(FRAME_BUF_SIZE));
    for (int i = 0; i < NUM_BARS; i++) {
        double new_height = ((BarData*) bars->comp_data)[i].height;
        double last_height = ((BarData*) bars->comp_data)[i].last_height;
        height = (int) (last_height + (new_height - last_height) / 4);
        // height = ((BarData*) bars->comp_data)[i].height;
        for (int k = 0; k < FRAME_BUF_HEIGHT; k++) {
            if (k > 14 - height) {
                unsigned int pixel = get_pixel(height);
                for (int j = 0; j < BAR_WIDTH; j++) {
                    (*rast)[2*BAR_WIDTH*i + j][k] = pixel;
                }
            } else {
                for (int j = 0; j < BAR_WIDTH; j++) {
                    (*rast)[2*BAR_WIDTH*i + j][k] = 0x0;
                }
            }
            for (int j = BAR_WIDTH; j < 2 * BAR_WIDTH; j++) {
                (*rast)[2* BAR_WIDTH*i + j][k] = 0x0;
            }
        }
    }
    iter_label++;
}

void audio_reactive_init(double* bands) {
    bands_buf = bands;
    iter_label = 0;
}