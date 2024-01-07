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

void center_image(frame_buf* input, frame_buf* frame, double scale_fact) {
  memset(frame, 0, FRAME_BUF_SIZE);
  int y_adjust = 1 / scale_fact;
  int x_adjust = ((1 - scale_fact) / 2) * 294;
  for (int i = 0; i < (FRAME_BUF_WIDTH * scale_fact); i++) {
        for (int j = 0; j < FRAME_BUF_HEIGHT; j++) {
            (*frame)[i + x_adjust][j + y_adjust] = (*input)[i][j];
        }
    }
}

void scale_image(frame_buf* input, frame_buf* frame, double scale_fact) {
    memset(frame, 0, FRAME_BUF_SIZE);
    int scaled_width = (int) (294 * scale_fact);
    int scaled_height = (int) (14 * scale_fact);

    for (int i = 0; i < scaled_width; i++) {
        for (int j = 0; j < scaled_height; j++) {
            int originalX = (int) (j / scale_fact);
            int originalY = (int) (i / scale_fact);
            // printf("(%d, %d)\n", originalX, originalY);
            (*frame)[i][j] = (*input)[originalY][originalX];
        }
    }
}

void layer(Component* comp, frame_buf* rast, frame_buf* rendered_frame) {
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
    frame_buf* component_frame = malloc(FRAME_BUF_SIZE);
    for (int i = 0; i < MAX_COMPONENTS; i++) {
        if (!comp[i].rast) {
            continue;
        }
        memcpy(component_frame, comp[i].rast, FRAME_BUF_SIZE);
        switch(comp[i].type) {
            case BAR:
                rasterize_bars(&comp[i], component_frame);
                break;
            case TEXT:
                frame_buf* temp_buf = malloc(FRAME_BUF_SIZE);
                // memcpy(temp_buf, component_frame, FRAME_BUF_SIZE);
                // scale_image(temp_buf, component_frame, scale);
                // memcpy(temp_buf, component_frame, FRAME_BUF_SIZE);
                // center_image(temp_buf, component_frame, scale);
                // free(temp_buf);
                break;
        }
        for (int j = 0; j < MAX_ANIMATIONS; j++) {
            if (!comp[i].animation[j]) {
                break;
            }
            Point new_pos = comp[i].animation[j](comp[i].position, comp[i].speed);
            comp[i].position = new_pos;
        }

        layer(&comp[i], component_frame, rendered_frame);
    }
    free(component_frame);
}

