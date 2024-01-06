/**
 * @file render.c
 * @date 12/30/23
 * @brief Render
 * @author Nicholas Cole https://nicholascole.dev
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../../include/render_toolkit.h"

void layer(Component* comp, frame_buf* rendered_frame) {
    frame_buf* rast = comp->rast;
    int i, j, k;
    int offset = comp->position.x;
    int y = comp->position.y;

    for (i = 0; i < FRAME_BUF_WIDTH; i++) {
        j = (int) (i + offset) % FRAME_BUF_WIDTH;
        for (k = 0; k < FRAME_BUF_HEIGHT; k++) {
            if (k + y >= FRAME_BUF_HEIGHT || k + y < 0) {
                continue;
            }
            if (!(*rast)[i][k]) {
                continue;
            }
            (*rendered_frame)[j][k + y] = (*rast)[i][k];
        }
    }
}

void render(Component* comp, frame_buf* rendered_frame) {
    for (int i = 0; i < MAX_COMPONENTS; i++) {
        if (!comp[i].rast) {
            continue;
        }
        if (strcmp(comp[i].content, "bars_component") == 0) {
            rasterize_bars(&comp[i]);
        }
        for (int j = 0; j < MAX_ANIMATIONS; j++) {
            if (!comp[i].animation[j]) {
                break;
            }
            Point new_pos = comp[i].animation[j](comp[i].position, comp[i].speed);
            comp[i].position = new_pos;
        }

        layer(&comp[i], rendered_frame);
    }
}

