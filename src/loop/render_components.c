/*

This stage is responsible for rendering each individual
component

Nicholas Cole — 12/30/23

*/

#include <stdio.h>
#include <stdlib.h>
#include "./../../include/render_toolkit.h"

void render_frame(Component* comp, frame_buf* rendered_frame) {
    // frame_buf* rendered_frame = malloc(FRAME_BUF_SIZE);
    frame_buf* rast = comp->rast;
    int i, j, k;
    int offset = comp->position.x;
    int y = comp->position.y;

    if (comp->apply_color) {
        double brightness = comp->brightness;
        Color color_overlay; 
        color_overlay.red = comp->color_overlay.red * brightness;
        color_overlay.green = comp->color_overlay.green * brightness;
        color_overlay.blue = comp->color_overlay.blue * brightness;

        for (i = 0; i < FRAME_BUF_WIDTH; i++) {
            j = (int) (i + comp->speed * offset) % FRAME_BUF_WIDTH;  // Increment 'j' by 2
            for (k = 0; k < FRAME_BUF_HEIGHT; k++) {
                if (k + y >= FRAME_BUF_HEIGHT || k + y < 0) {
                    continue;
                }
                float alpha = (float) (*rast)[i][k] / 0xFF;
                int pixel = (color_overlay.red * alpha);
                pixel <<= 8;
                pixel += (color_overlay.green * alpha);
                pixel <<= 8;
                pixel += (color_overlay.blue * alpha);
                if ((*rast)[i][k]) {
                    (*rendered_frame)[j][k + y] = pixel;
                }
            }
        }
    } else {
        for (i = 0; i < FRAME_BUF_WIDTH; i++) {
            j = (int) (i + comp->speed * offset) % FRAME_BUF_WIDTH;  // Increment 'j' by 2
            for (k = 0; k < FRAME_BUF_HEIGHT; k++) {
                if (k + y >= FRAME_BUF_HEIGHT || k + y < 0) {
                    continue;
                }
                double brightness = comp->brightness;
                Color current_color;
                current_color.red = ((*rast)[i][k] & 0xFF0000) >> 16;
                current_color.green = ((*rast)[i][k] & 0x00FF00) >> 8;
                current_color.blue = ((*rast)[i][k] & 0x0000FF);
                Color adjusted_color; 
                adjusted_color.red = current_color.red * brightness;
                adjusted_color.green = current_color.green * brightness;
                adjusted_color.blue = current_color.blue * brightness;
                int pixel = adjusted_color.red;
                pixel <<= 8;
                pixel += adjusted_color.green;
                pixel <<= 8;
                pixel += adjusted_color.blue;
                if ((*rast)[i][k]) {
                    (*rendered_frame)[j][k + y] = (*rast)[i][k] * comp->brightness;
                }
            }
        }
    }

    return rendered_frame;
}