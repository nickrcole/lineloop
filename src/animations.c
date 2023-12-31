/*

Animation functions for components
Nicholas Cole — 12/30/23

*/

#include "./../include/render_toolkit.h"
#include "./../include/animations.h"

Point scroll_forward( Point pos, double speed ) {
    Point new_pos;
    new_pos.y = pos.y;        // No change in the y direction
    if ( pos.x == 0 ) {       // Decrement x and account for rollover
        new_pos.x = 294 / speed;
    } else {
        new_pos.x = pos.x - 1;
    }
    return new_pos;
}