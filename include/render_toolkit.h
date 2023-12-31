/*

Definitions for use by the rendering pipeline (not the driver)

*/

#ifndef RENDER_TOOLKIT_H
#define RENDER_TOOLKIT_H

#define FRAME_BUF_WIDTH   294
#define FRAME_BUF_HEIGHT   15
#define LIMIT_FRAMERATE     0

// Buffer that stores a frame
typedef int frame_buf[294][15];

extern const int FRAME_BUF_SIZE;

extern const int COMPONENT_SIZE;

// Renders given text to a frame buffer
int rasterize_text( char*      filename,
                    char*      text,
                    frame_buf* frame );

typedef struct Point
{
    int x;
    int y;
} Point;

// Animation that defines a component's movement
typedef Point (*Animation)(Point, double);

typedef struct 
{
    frame_buf*  rast;
    Animation   animation;
    double      speed;
    Point       position;
    char*       content;
    int         layer;
} Component;

Component* initialize_text_component( char* content );

frame_buf* render_frame( Component* comp );

void show_image( frame_buf* frame );

void display_frame( frame_buf* frame );

int driver_init( void );

#endif