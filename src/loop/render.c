/**
 * @file render.c
 * @date 12/30/23
 * @brief Updates rast and positions for components
 * @author Nicholas Cole https://nicholascole.dev
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../../include/render_toolkit.h"

void rasterize_bars(frame_buf* rast) {
    int height = 14;
    memset(rast, 0, sizeof(FRAME_BUF_SIZE));
    for (int i = 0; i < NUM_BARS; i++) {
        for (int k = 0; k < FRAME_BUF_HEIGHT; k++) {
            // Cap with white
            if (k == 14 - height) {
                (*rast)[2*i][k] = 0x000010;
                (*rast)[2*i + 1][k] = 0x000010;
            } else if (k > 14 - height) {
                (*rast)[2*i][k] = 0x000010;
                (*rast)[2*i + 1][k] = 0x000010;
            } else {
                (*rast)[2*i][k] = 0x0;
                (*rast)[2*i + 1][k] = 0x0;
            }
        }
    }
}

void render_component(Component* comp) {
    
}

void render(Component* comp) {
    for (int i = 0; i < MAX_COMPONENTS; i++) {
        if (!comp[i].rast) {
            continue;
        }
        if (strcmp(comp[i].content, "bars_component") == 0) {
            rasterize_bars(comp[i].rast);
        }
        for (int j = 0; j < MAX_ANIMATIONS; j++) {
            if (!comp[i].animation[j]) {
                break;
            }
            Point new_pos = comp[i].animation[j](comp[i].position, comp[i].speed);
            comp[i].position = new_pos;
        }
    }
}

