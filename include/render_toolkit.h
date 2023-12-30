/*
Definitions for use by the rendering pipeline (not the driver)
*/

// Buffer that stores a frame
typedef int frame_buf[294][15];

// Renders given text to a frame buffer
int render_text( char*      filename,
                 char*      text,
                 frame_buf* frame );