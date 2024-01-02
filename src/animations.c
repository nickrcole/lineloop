/*

Animation functions for components
Nicholas Cole — 12/30/23

*/

#include <stdio.h>
#include "./../include/render_toolkit.h"
#include "./../include/animations.h"

const int BOB_INTENSITY = 10;
int bob_status = BOB_INTENSITY;

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

Point scroll_backward( Point pos, double speed ) {
    Point new_pos;
    new_pos.y = pos.y;        // No change in the y direction
    if ( pos.x == 294 / speed ) {       // Decrement x and account for rollover
        new_pos.x = 0;
    } else {
        new_pos.x = pos.x + 1;
    }
    return new_pos;
}

Point bob( Point pos, double speed ) {
    if (bob_status % 2 != 0) {
        bob_status--;
        return pos;
    }
    if (bob_status < -BOB_INTENSITY) {
        bob_status = BOB_INTENSITY;
    }
    Point new_pos;
    new_pos.x = pos.x;
    new_pos.y = pos.y;
    if (bob_status > 0) {
        new_pos.y++;
    }
    if (bob_status < 0) {
        new_pos.y--;
    }
    bob_status--;
    return new_pos;
}

Point blank( Point pos, double speed ) {
    return pos;
}