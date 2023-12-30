/*
Definitions for use by the rendering pipeline (not the driver)
*/

// Buffer that stores a frame
typedef int frame_buf[294][15];

// Renders given text to a frame buffer
int render_text( char*      filename,
                 char*      text,
                 frame_buf* frame );

typedef struct Point {
    int x;
    int y;
} Point;

// Animation that defines a component's movement
typedef Point (*Animation)(Point);

typedef struct {
    char* content;
    Point position;
    Animation animation;
    int layer;
} Component;