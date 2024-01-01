/*

Main controller interface for LineLoop
Controls render loop and communicates with driver
Nicholas Cole — 12/30/23

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <sys/time.h>
#include "./../include/render_toolkit.h"
#include "./../include/animations.h"

const int FRAME_BUF_SIZE = sizeof(frame_buf);
const int COMPONENT_SIZE = sizeof(Component);
char* FONT_PATH = "/home/pi/lineloop/fonts/TNR.ttf";
Component* comp;

void show_image( frame_buf* frame )
{
  int  i, j;

  for ( i = 0; i < FRAME_BUF_WIDTH; i++ )
  {
    for ( j = 0; j < FRAME_BUF_HEIGHT; j++ )
      putchar( (*frame)[i][j] == 0 ? ' '
                                   : (*frame)[i][j] < 128 ? '+'
                                                          : '*' );
    putchar( '\n' );
  }
}

void render_loop( void ) {
    struct timeval start, render_end, frame_end;
    double total_frame_time;
    frame_buf* rendered_frame = malloc(FRAME_BUF_SIZE);
    memset(rendered_frame, 0, FRAME_BUF_SIZE);

    while (1) {
        gettimeofday(&start, NULL);

        // Prepare stage
        Point new_pos = comp->animation(comp->position, comp->speed);
        comp->position = new_pos;

        // Render stage
        rendered_frame = render_frame(comp);

        display_frame(rendered_frame);

        gettimeofday(&render_end, NULL);
        #if LIMIT_FRAMERATE
        if (render_end.tv_usec - start.tv_usec >= 0) {
            usleep(16666 - (render_end.tv_usec - start.tv_usec));  // Enforce framerate
        }
        #endif
        gettimeofday(&frame_end, NULL);
        total_frame_time = (frame_end.tv_usec - start.tv_usec);
        // printf("Frame rate: %d\n", 1000000 /(frame_end.tv_usec - start.tv_usec));
    }
}

void clear_display( void ) {
    frame_buf* clear = malloc(FRAME_BUF_SIZE);
    memset(clear, 0, FRAME_BUF_SIZE);
    display_frame(clear);
}

void display_predictions( void ) {
    Color text_color;
    text_color.red = 0;
    text_color.green = 255;
    text_color.blue = 0;
    char* content = "CHAMPS: 20 mins    DOGGIE'S: 15 mins";
    comp = initialize_component( TEXT, content, scroll_forward, &text_color, 0 );
    render_loop();
}

void display_test( void ) {
    // comp = initialize_image_component("./../images/arrow.png");
    render_loop();
}

void close( int signo ) {
    if (signo == SIGINT) {
        printf("Exiting\n");
        // clear_display();
        exit(0);
    }
}

int main(int argc, char** argv) {
    int args=0;
    driver_init();
    signal(SIGINT, close);
    while (argc > ++args)               // Process command-line args
    {
        if (argv[args][0] == '-')
        {
            switch (toupper(argv[args][1]))
            {
            case 'C':                   // -C: clear the display
                clear_display();
                exit(0);
                break;
            case 'P':
                display_predictions();
                break;
            case 'T':
                display_test();
                break;
            default:                    // Otherwise error
                printf("Unrecognised option '%c'\n", argv[args][1]);
                printf("Options:\n"
                       "  -c     clear display\n"\
                       "  -p     diplay comp\n"\
                      );
                return(1);
            }
        }
    }
}

Component* initialize_component( COMP_TYPE type, char* content, Animation animation, Color* color, int layer ) {
    Component* comp = malloc(COMPONENT_SIZE);
    comp->rast = malloc(FRAME_BUF_SIZE);
    memset(comp->rast, 0, FRAME_BUF_SIZE);
    comp->animation = animation;
    comp->speed = 0.45;
    comp->brightness = 0.5;
    Point pos;
    pos.x = 0;
    pos.y = 0;
    comp->position = pos;
    comp->content = content;
    comp->layer = layer;
    switch (type) {
        case TEXT:
            rasterize_text(FONT_PATH, content, comp->rast);
            comp->color_overlay = *color;
            break;
        case IMAGE:
            rasterize_image(comp->rast, content);
            break;
        case BAR:
            break;
    }
    return comp;
}