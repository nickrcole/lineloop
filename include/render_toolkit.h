/*

Definitions for use by the rendering pipeline (not the driver)

*/

#ifndef RENDER_TOOLKIT_H
#define RENDER_TOOLKIT_H

#define FRAME_BUF_WIDTH   294
#define FRAME_BUF_HEIGHT   15
#define MAX_FRAMERATE      80
#define LIMIT_FRAMERATE     1
#define MAX_ANIMATIONS      2
#define MAX_COMPONENTS    150
#define NUM_BARS           21

// Buffer that stores a frame
typedef int frame_buf[294][15];

extern const int FRAME_BUF_SIZE;

extern const int COMPONENT_SIZE;

typedef enum {
    TEXT,
    IMAGE,
    BAR
} COMP_TYPE;

void rasterize_image( frame_buf* frame,
                      char *     image );

typedef struct Point
{
    double x;
    double y;
} Point;

// Animation that defines a component's movement
typedef Point (*Animation)(Point, double);

typedef struct
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Color;

typedef struct 
{
    frame_buf*  rast;
    Animation*  animation;
    double      speed;
    Color       color_overlay;
    int         apply_color;
    double      brightness;
    Point       position;
    char*       content;
    COMP_TYPE   type;
    void*       comp_data;
} Component;

typedef struct
{
    float        height;
    float        last_height;
} BarData;

Component* initialize_component( COMP_TYPE  type, 
                                 char*      content, 
                                 Animation* animation, 
                                 int        num_anims, 
                                 Color*     color, 
                                 Point*     pos );

int rasterize_text( char*      filename,
                    char*      text,
                    frame_buf* frame,
                    Color      color,
                    double     brightness );

void rasterize_bars(Component* comp, frame_buf* frame);

void render(Component* comp, frame_buf* rendered_frame);

void show_image( frame_buf* frame );

void display_frame( frame_buf* frame );

void audio_reactive_init(double* bands);

void attach_bar_components(BarData* bars_buf);

int driver_init( void );

#endif