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

    for (i = 0; i < FRAME_BUF_WIDTH; i++) {
        j = (int) (i + comp->speed * offset) % FRAME_BUF_WIDTH;  // Increment 'j' by 2
        for (k = 0; k < FRAME_BUF_HEIGHT; k++) {
            (*rendered_frame)[j][k] = (*rast)[i][k];
        }
    }

    return rendered_frame;
}