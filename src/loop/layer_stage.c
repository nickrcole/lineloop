/**
 * @file layer_stage.c
 * @date 12/30/23
 * @brief This stage is responsible for layering components together based on their layering priority to create a final frame to be sent to the display
 * @author Nicholas Cole https://nicholascole.dev
 */

#include <stdio.h>
#include <stdlib.h>
#include "./../../include/render_toolkit.h"

frame_buf* layer_components(Component* comp) {
    frame_buf* rendered_frame = malloc(FRAME_BUF_SIZE);
    frame_buf* frame;
    for (int j = MAX_COMPONENTS - 1; j >= 0; j--) {
        frame = comp[j].frame;
        if (!frame) {
            continue;
        }
        for (int i = 0; i < FRAME_BUF_WIDTH; i++) {
            for (int k = 0; k < FRAME_BUF_HEIGHT; k++) {
                if ((*frame)[i][k] == 0x0) {
                    continue;
                }
                (*rendered_frame)[j][k] = (*frame)[i][k];
            }
        }
    }
    return rendered_frame;
}