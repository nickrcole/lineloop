/*

Definitions for all LineLoop animations
Nicholas Cole — 12/30/23

*/

#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "render_toolkit.h"

Point scroll_forward( Point point, double speed );

Point scroll_backward( Point point, double speed );

Point blank( Point pos, double speed );

Point bar_jitter( Point pos, double speed );

#endif