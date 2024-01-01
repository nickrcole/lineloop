/*

This stage is responsible for rendering each individual
component

Nicholas Cole — 12/30/23

*/

#include <stdio.h>
#include <stdlib.h>
#include "./../../include/render_toolkit.h"

frame_buf* render_frame(Component* comp) {
    frame_buf* rendered_frame = malloc(FRAME_BUF_SIZE);
    frame_buf* rast = comp->rast;
    int i, j, k;
    int offset = comp->position.x;

    if (1) {
        double brightness = comp->brightness;
        Color color_overlay; 
        color_overlay.red = comp->color_overlay.red * brightness;
        color_overlay.green = comp->color_overlay.green * brightness;
        color_overlay.blue = comp->color_overlay.blue * brightness;

        for (i = 0; i < FRAME_BUF_WIDTH; i++) {
            j = (int) (i + comp->speed * offset) % FRAME_BUF_WIDTH;  // Increment 'j' by 2
            for (k = 0; k < FRAME_BUF_HEIGHT; k++) {
                float alpha = (float) (*rast)[i][k] / 0xFF;
                int pixel = (color_overlay.red * alpha);
                pixel <<= 8;
                pixel += (color_overlay.green * alpha);
                pixel <<= 8;
                pixel += (color_overlay.blue * alpha);
                if ((*rast)[i][k]) {
                    (*rendered_frame)[j][k] = pixel;
                } else {
                    (*rendered_frame)[j][k] = 0x0;
                }
            }
        }
    } else {
        for (i = 0; i < FRAME_BUF_WIDTH; i++) {
            j = (int) (i + comp->speed * offset) % FRAME_BUF_WIDTH;  // Increment 'j' by 2
            for (k = 0; k < FRAME_BUF_HEIGHT; k++) {
                (*rendered_frame)[i][k] = (*rast)[i][k];
            }
        }
    }

    return rendered_frame;
}